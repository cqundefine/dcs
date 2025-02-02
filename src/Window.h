#pragma once

#include "AutoRelease.h"
#include "NonCopyable.h"
#include "WindowEvent.h"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <glm/glm.hpp>
#include <optional>
#include <queue>

namespace DCS
{

class Window
{
    MAKE_NONCOPYABLE(Window);

public:
    Window(std::uint32_t width, std::uint32_t height);
    ~Window();

    bool should_close() const;
    void update() const;

    std::optional<WindowEvent> poll_event();

    glm::ivec2 mouse_position() const;
    bool is_mouse_button_down(int button) const;

    std::uint32_t width() const { return m_width; }
    std::uint32_t height() const { return m_height; }
    glm::mat4 projection() const { return m_projection; }

private:
    AutoRelease<GLFWwindow*, nullptr> m_window;

    std::uint32_t m_width;
    std::uint32_t m_height;
    glm::mat4 m_projection;

    std::queue<WindowEvent> m_event_queue;
};

}
