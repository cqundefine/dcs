#include "Font.h"

#include <filesystem>
#include <memory>
#include <print>
#include <string_view>

#include "Ensure.h"
#include "freetype/freetype.h"

namespace DCS
{

Font::Font(const std::filesystem::path& font_path, std::uint32_t font_size)
    : m_ft{nullptr, FT_Done_FreeType}
    , m_face{nullptr, FT_Done_Face}
{
    ensure(FT_Init_FreeType(&m_ft) == 0, "failed to initialize freetype");
    ensure(FT_New_Face(m_ft, font_path.string().c_str(), 0, &m_face) == 0, "failed to create the face");
    ensure(FT_Set_Pixel_Sizes(m_face, 0, font_size) == 0, "failed to set the pixel size");
}

const CharacterData* Font::load_character(char c)
{
    if (m_characters.contains(c))
        return &m_characters.at(c);

    ensure(FT_Load_Char(m_face, c, FT_LOAD_RENDER | FT_LOAD_TARGET_(FT_RENDER_MODE_SDF)) == 0, "failed to load glyph");

    std::println(
        "Loaded character: '{}', width: {}, height: {}, bearing: {}x{}, advance: {}",
        c,
        m_face.get()->glyph->bitmap.width,
        m_face.get()->glyph->bitmap.rows,
        m_face.get()->glyph->bitmap_left,
        m_face.get()->glyph->bitmap_top,
        m_face.get()->glyph->advance.x);

    std::shared_ptr<Texture> texture{};
    if (m_face.get()->glyph->bitmap.width != 0)
        texture = std::make_shared<Texture>(
            m_face.get()->glyph->bitmap.width,
            m_face.get()->glyph->bitmap.rows,
            1,
            reinterpret_cast<std::byte*>(m_face.get()->glyph->bitmap.buffer));

    const auto characterData = CharacterData{
        .texture = texture,
        .size = {m_face.get()->glyph->bitmap.width, m_face.get()->glyph->bitmap.rows},
        .bearing = {m_face.get()->glyph->bitmap_left, m_face.get()->glyph->bitmap_top},
        .advance = static_cast<int>(m_face.get()->glyph->advance.x / 64)};

    m_characters[c] = characterData;

    return &m_characters.at(c);
}

glm::ivec2 Font::get_text_dimensions(std::string_view text)
{
    std::uint32_t width = 0;
    std::uint32_t height = 0;

    for (char c : text)
    {
        const auto character = load_character(c);
        width += character->advance;
        height = std::max(height, static_cast<std::uint32_t>(character->size.y - character->bearing.y));
    }

    return {width, height};
}

}
