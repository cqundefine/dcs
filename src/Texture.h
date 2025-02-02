#pragma once

#include <cstddef>
#include <cstdint>
#include <glad/gl.h>

#include "AutoRelease.h"
#include "NonCopyable.h"

namespace DCS
{

class Texture
{
    MAKE_NONCOPYABLE(Texture);

public:
    Texture(std::uint32_t width, std::uint32_t height, std::uint32_t channel_count, const std::byte* data);

    void bind(std::uint32_t texture) const;

private:
    AutoRelease<GLuint> m_texture;
};

}
