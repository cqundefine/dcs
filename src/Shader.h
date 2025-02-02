#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <string_view>

#include "AutoRelease.h"
#include "NonCopyable.h"
#include "StringMap.h"

namespace DCS
{

class Shader
{
    MAKE_NONCOPYABLE(Shader);

public:
    Shader(std::string_view vertex_shader_source, std::string_view fragment_shader_source);

    void use() const;

    void set_uniform(std::string_view name, int value) const;
    void set_uniform(std::string_view name, float value) const;
    void set_uniform(std::string_view name, glm::vec2 value) const;
    void set_uniform(std::string_view name, const glm::vec4& value) const;
    void set_uniform(std::string_view name, const glm::mat4& value) const;

private:
    void retrieve_uniforms();

    AutoRelease<GLuint> m_program;
    StringMap<GLint> m_uniforms;
};

}
