#pragma once

#include <cstdint>
#include <filesystem>
#include <glad/gl.h>
#include <string_view>

#include "AutoRelease.h"
#include "Buffer.h"
#include "Font.h"
#include "Shader.h"
#include "Util.h"
#include "Window.h"

namespace DCS
{

class Renderer
{
public:
    Renderer(Ref<Window> window, const std::filesystem::path& assets_path);

    void clear() const;

    void draw_text(Ref<Font> font, std::string_view text, std::uint32_t x, std::uint32_t y, float scale, const glm::vec4& color) const;
    void draw_text_centered(Ref<Font> font, std::string_view text, std::uint32_t x, std::uint32_t y, float scale, const glm::vec4& color)
        const;

    void draw_rect(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height, const glm::vec4& color) const;
    void draw_rect_between_points(std::uint32_t x1, std::uint32_t y1, std::uint32_t x2, std::uint32_t y2, const glm::vec4& color) const;

    void draw_grid(
        std::uint32_t x,
        std::uint32_t y,
        std::uint32_t grid_size,
        std::uint32_t cell_size,
        float grid_width,
        const glm::vec4& color) const;

    Ref<Window> target_window() const { return m_window; }
    Ref<Font> default_font() const { return m_default_font; }

private:
    Ref<Window> m_window;
    Ref<Font> m_default_font;

    AutoRelease<GLuint> m_rect_vertex_array;
    Own<DCS::Buffer> m_rect_vertex_buffer;

    Own<DCS::Shader> m_text_shader;
    Own<DCS::Shader> m_rect_shader;
    Own<DCS::Shader> m_grid_shader;
};

}
