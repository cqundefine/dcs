#include "Texture.h"
#include "Exception.h"

static constexpr GLenum channel_count_to_base_format(std::uint32_t channel_count)
{
    switch (channel_count)
    {
        case 1:  return GL_RED;
        default: throw DCS::Exception{"unsupported channel count"};
    }
}

static constexpr GLenum channel_count_to_sized_format(std::uint32_t channel_count)
{
    switch (channel_count)
    {
        case 1:  return GL_R8;
        default: throw DCS::Exception{"unsupported channel count"};
    }
}

namespace DCS
{

Texture::Texture(std::uint32_t width, std::uint32_t height, std::uint32_t channel_count, const std::byte* data)
    : m_texture{0u, [](GLuint texture) { glDeleteTextures(1, &texture); }}
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);

    glTextureStorage2D(m_texture, 1, channel_count_to_sized_format(channel_count), width, height);
    glTextureSubImage2D(m_texture, 0, 0, 0, width, height, channel_count_to_base_format(channel_count), GL_UNSIGNED_BYTE, data);

    glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::bind(std::uint32_t texture) const
{
    glBindTextureUnit(texture, m_texture);
}

}
