#pragma once

#include <glad/gl.h>
#include <span>

#include "AutoRelease.h"

namespace DCS
{

enum class BufferUsage
{
    StaticDraw,
    DynamicDraw,
    StreamDraw,
};

class Buffer
{
public:
    Buffer();
    Buffer(std::size_t size, BufferUsage usage);

    void write(std::span<const std::byte> data) const;

    GLuint native_handle() const { return m_buffer; }

private:
    AutoRelease<GLuint> m_buffer;
};

}
