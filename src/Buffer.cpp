#include "Buffer.h"

static constexpr GLenum buffer_usage_to_gl(DCS::BufferUsage usage)
{
    switch (usage)
    {
        using enum DCS::BufferUsage;
        case StaticDraw:  return GL_STATIC_DRAW;
        case DynamicDraw: return GL_DYNAMIC_DRAW;
        case StreamDraw:  return GL_STREAM_DRAW;
    }
}

namespace DCS
{

Buffer::Buffer()
    : m_buffer{0, [](GLuint buffer) { glDeleteBuffers(1, &buffer); }}
{
    glCreateBuffers(1, &m_buffer);
}

Buffer::Buffer(std::size_t size, BufferUsage usage)
    : Buffer{}
{
    glNamedBufferData(m_buffer, size, nullptr, buffer_usage_to_gl(usage));
}

void Buffer::write(std::span<const std::byte> data) const
{
    glNamedBufferSubData(m_buffer, 0, data.size_bytes(), data.data());
}

}
