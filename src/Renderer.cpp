#include "Renderer.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <print>

#include "AutoRelease.h"
#include "Buffer.h"
#include "Util.h"

static void create_rect_between_points_into_buffer(const DCS::Buffer& buffer, float x1, float y1, float x2, float y2)
{
    const float vertices[] = {
        x1, y2, 0.0f, 1.0f, // Bottom-left
        x1, y1, 0.0f, 0.0f, // Top-left
        x2, y1, 1.0f, 0.0f, // Top-right

        x1, y2, 0.0f, 1.0f, // Bottom-left
        x2, y1, 1.0f, 0.0f, // Top-right
        x2, y2, 1.0f, 1.0f  // Bottom-right
    };

    buffer.write({reinterpret_cast<const std::byte*>(vertices), sizeof(vertices)});
}

static void create_rect_into_buffer(const DCS::Buffer& buffer, float x, float y, float width, float height)
{
    create_rect_between_points_into_buffer(buffer, x, y, x + width, y + height);
}

namespace DCS
{

Renderer::Renderer(Ref<Window> window, const std::filesystem::path& assets_path)
    : m_window(window)
    , m_default_font(DCS::MakeRef<DCS::Font>(assets_path / "JetBrainsMono-Regular.ttf", 16))
    , m_rect_vertex_array(0, [](GLuint vao) { glDeleteVertexArrays(1, &vao); })
    , m_rect_vertex_buffer(MakeOwn<Buffer>(6 * 4 * sizeof(float), DCS::BufferUsage::StreamDraw))
    , m_text_shader(
          MakeOwn<Shader>(read_file_to_string(assets_path / "text.vert.glsl"), read_file_to_string(assets_path / "text.frag.glsl")))
    , m_rect_shader(
          MakeOwn<Shader>(read_file_to_string(assets_path / "rect.vert.glsl"), read_file_to_string(assets_path / "rect.frag.glsl")))
    , m_grid_shader(
          MakeOwn<Shader>(read_file_to_string(assets_path / "grid.vert.glsl"), read_file_to_string(assets_path / "grid.frag.glsl")))
{
    glCreateVertexArrays(1, &m_rect_vertex_array);

    glVertexArrayVertexBuffer(m_rect_vertex_array, 0, m_rect_vertex_buffer->native_handle(), 0, 4 * sizeof(float));

    glEnableVertexArrayAttrib(m_rect_vertex_array, 0);
    glVertexArrayAttribFormat(m_rect_vertex_array, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(m_rect_vertex_array, 0, 0);

    glEnableVertexArrayAttrib(m_rect_vertex_array, 1);
    glVertexArrayAttribFormat(m_rect_vertex_array, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));
    glVertexArrayAttribBinding(m_rect_vertex_array, 1, 0);
}

void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Renderer::push_offset(Position offset)
{
    m_offset_stack.push(offset);
    m_offset += offset;
}

void Renderer::pop_offset()
{
    m_offset -= m_offset_stack.top();
    m_offset_stack.pop();
}

void Renderer::draw_text(Ref<Font> font, std::string_view text, std::uint32_t x, std::uint32_t y, float scale, const glm::vec4& color) const
{
    y += font->get_text_dimensions(text).y * scale;

    m_text_shader->use();
    prepare_shader(m_text_shader.get());
    m_text_shader->set_uniform("color", color);

    glBindVertexArray(m_rect_vertex_array);

    for (char c : text)
    {
        auto character = font->load_character(c);
        if (character->size.x == 0)
        {
            x += character->advance * scale;
            continue;
        }

        const float xpos = x + character->bearing.x * scale;
        const float ypos = y - character->bearing.y * scale;
        const float width = character->size.x * scale;
        const float height = character->size.y * scale;
        create_rect_into_buffer(*m_rect_vertex_buffer, xpos, ypos, width, height);

        character->texture->bind(0);
        m_text_shader->set_uniform("text", 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += character->advance * scale;
    }
}

void Renderer::draw_text_centered(
    Ref<Font> font,
    std::string_view text,
    std::uint32_t x,
    std::uint32_t y,
    float scale,
    const glm::vec4& color) const
{
    const auto dimensions = font->get_text_dimensions(text);
    const auto text_x = x - dimensions.x / 2;
    const auto text_y = y - dimensions.y / 2;
    draw_text(font, text, text_x, text_y, scale, color);
}

void Renderer::draw_rect(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height, const glm::vec4& color) const
{
    m_rect_shader->use();
    prepare_shader(m_rect_shader.get());
    m_rect_shader->set_uniform("color", color);

    glBindVertexArray(m_rect_vertex_array);
    create_rect_into_buffer(*m_rect_vertex_buffer, x, y, width, height);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::draw_rect_between_points(std::uint32_t x1, std::uint32_t y1, std::uint32_t x2, std::uint32_t y2, const glm::vec4& color)
    const
{
    m_rect_shader->use();
    prepare_shader(m_rect_shader.get());
    m_rect_shader->set_uniform("color", color);

    glBindVertexArray(m_rect_vertex_array);
    create_rect_between_points_into_buffer(*m_rect_vertex_buffer, x1, y1, x2, y2);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::draw_grid(
    std::uint32_t x,
    std::uint32_t y,
    std::uint32_t grid_size,
    std::uint32_t cell_size,
    float grid_width,
    const glm::vec4& color) const
{
    m_grid_shader->use();
    prepare_shader(m_grid_shader.get());
    m_grid_shader->set_uniform("color", color);
    m_grid_shader->set_uniform("cellSize", static_cast<float>(cell_size));
    m_grid_shader->set_uniform("gridWidth", grid_width);

    glBindVertexArray(m_rect_vertex_array);
    create_rect_into_buffer(*m_rect_vertex_buffer, x, y, cell_size * grid_size, cell_size * grid_size);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::prepare_shader(DCS::Shader* shader) const
{
    shader->set_uniform("projection", m_window->projection());
    shader->set_uniform("model", glm::translate(glm::mat4{1.0f}, {m_offset.x, m_offset.y, 0.0f}));
}

}
