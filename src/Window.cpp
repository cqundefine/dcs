#include "Window.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <print>

#include "Ensure.h"
#include "Exception.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "WindowEvent.h"

static std::uint32_t g_window_count = 0;

static void GLAPIENTRY
gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param)
{
    if (severity == GL_DEBUG_SEVERITY_HIGH)
        throw DCS::Exception("OpenGL debug message: {}", message);
    else
        std::println("OpenGL debug message: {}", message);
}

static void glfw_error_callback(int error, const char* description)
{
    throw DCS::Exception("GLFW error: {}", description);
}

namespace DCS
{

Window::Window(std::uint32_t width, std::uint32_t height)
    : m_window{}
    , m_width{width}
    , m_height{height}
    , m_projection{glm::ortho(0.0f, static_cast<float>(m_width), static_cast<float>(m_height), 0.0f)}
{
    if (g_window_count == 0)
    {
        glfwSetErrorCallback(glfw_error_callback);
        DCS::ensure(glfwInit(), "failed to initialize GLFW");
    }

    g_window_count++;

    DCS::ensure(g_window_count == 1, "more than one window is not supported");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    m_window = DCS::AutoRelease<GLFWwindow*>{glfwCreateWindow(width, height, "DCS", NULL, NULL), glfwDestroyWindow};
    DCS::ensure(m_window, "failed to create the window");

    glfwSetWindowUserPointer(m_window, this);

    glfwMakeContextCurrent(m_window);

    glfwSetWindowSizeCallback(
        m_window,
        [](GLFWwindow* window, int width, int height)
        {
            auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
            self->m_width = width;
            self->m_height = height;
            self->m_projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f);
            glViewport(0, 0, width, height);
        });

    glfwSetKeyCallback(
        m_window,
        [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
            self->m_event_queue.push(KeyEvent{key, KeyboardAction{action}, KeyboardModifiers{mods}});
        });

    glfwSetMouseButtonCallback(
        m_window,
        [](GLFWwindow* window, int button, int action, int mods)
        {
            auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
            self->m_event_queue.push(MouseEvent{MouseButton{button}, MouseAction{action}});
        });

    DCS::ensure(gladLoadGL(glfwGetProcAddress), "failed to load OpenGL");

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_callback, nullptr);

    glViewport(0, 0, width, height);
}

Window::~Window()
{
    g_window_count--;
    // FIXME: We have to make sure that the window is destroyed before glfwTerminate
    // if (g_window_count == 0)
    //     glfwTerminate();
}

bool Window::should_close() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::update() const
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

std::optional<WindowEvent> Window::poll_event()
{
    if (m_event_queue.empty())
        return {};
    const auto event = m_event_queue.front();
    m_event_queue.pop();
    return event;
}

Position Window::mouse_position() const
{
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    return {static_cast<std::uint32_t>(x), static_cast<std::uint32_t>(y)};
}

bool Window::is_mouse_button_down(int button) const
{
    return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
}

}
