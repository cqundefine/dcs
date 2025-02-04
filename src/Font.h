#pragma once

#include <ft2build.h>
#include <glm/glm.hpp>
#include FT_FREETYPE_H
#include <filesystem>
#include <map>
#include <memory>

#include "AutoRelease.h"
#include "NonCopyable.h"
#include "Texture.h"

namespace DCS
{

struct CharacterData
{
    std::shared_ptr<DCS::Texture> texture;

    glm::ivec2 size;
    glm::ivec2 bearing;

    int advance;
};

class Font
{
    MAKE_NONCOPYABLE(Font);

public:
    Font(const std::filesystem::path& font_path, std::uint32_t font_size);

    const CharacterData* load_character(char c);

    glm::ivec2 get_text_dimensions(std::string_view text);

private:
    AutoRelease<FT_Library> m_ft;
    AutoRelease<FT_Face> m_face;

    std::map<char, CharacterData> m_characters;
};

}
