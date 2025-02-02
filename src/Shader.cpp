#include "Shader.h"

#include "AutoRelease.h"
#include "Ensure.h"
#include "Exception.h"
#include <print>
#include <ranges>

static DCS::AutoRelease<GLuint> compile_shader(GLenum type, std::string_view source)
{
    const auto shader = glCreateShader(type);

    const auto source_data = source.data();
    const auto source_size = static_cast<GLint>(source.size());
    glShaderSource(shader, 1, &source_data, &source_size);

    glCompileShader(shader);

    GLint success{};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        std::string info_log(length, '\0');
        glGetShaderInfoLog(shader, length, nullptr, info_log.data());

        throw DCS::Exception{"failed to compile shader: {}", info_log};
    }

    return DCS::AutoRelease<GLuint>{shader, glDeleteShader};
}

namespace DCS
{

Shader::Shader(std::string_view vertex_shader_source, std::string_view fragment_shader_source)
    : m_program(glCreateProgram(), glDeleteProgram)
{
    const auto vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
    const auto fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

    glAttachShader(m_program, vertex_shader);
    glAttachShader(m_program, fragment_shader);
    glLinkProgram(m_program);

    GLint success;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);

    if (!success)
    {
        GLint length;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);

        std::string info_log(length, '\0');
        glGetProgramInfoLog(m_program, length, nullptr, info_log.data());

        throw DCS::Exception{"failed to link program: {}", info_log};
    }

    retrieve_uniforms();
}

void Shader::use() const
{
    glUseProgram(m_program);
}

void Shader::set_uniform(std::string_view name, int value) const
{
    const auto location = m_uniforms.find(name);
    ensure(location != std::ranges::cend(m_uniforms), "missing uniform: {}", name);
    glUniform1i(location->second, value);
}

void Shader::set_uniform(std::string_view name, float value) const
{
    const auto location = m_uniforms.find(name);
    ensure(location != std::ranges::cend(m_uniforms), "missing uniform: {}", name);
    glUniform1f(location->second, value);
}

void Shader::set_uniform(std::string_view name, glm::vec2 value) const
{
    const auto location = m_uniforms.find(name);
    ensure(location != std::ranges::cend(m_uniforms), "missing uniform: {}", name);
    glUniform2f(location->second, value.x, value.y);
}

void Shader::set_uniform(std::string_view name, const glm::vec4& value) const
{
    const auto location = m_uniforms.find(name);
    ensure(location != std::ranges::cend(m_uniforms), "missing uniform: {}", name);
    glUniform4f(location->second, value.x, value.y, value.z, value.w);
}

void Shader::set_uniform(std::string_view name, const glm::mat4& value) const
{
    const auto location = m_uniforms.find(name);
    ensure(location != std::ranges::cend(m_uniforms), "missing uniform: {}", name);
    glUniformMatrix4fv(location->second, 1, GL_FALSE, &value[0][0]);
}

void Shader::retrieve_uniforms()
{
    GLint uniform_count = 0;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &uniform_count);

    if (uniform_count != 0)
    {
        GLint max_name_length = 0;
        GLsizei length = 0;
        GLsizei count = 0;
        GLenum type;
        glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_length);

        auto uniform_name_buffer = std::string(max_name_length, '\0');

        for (GLint i = 0; i < uniform_count; i++)
        {
            glGetActiveUniform(m_program, i, max_name_length, &length, &count, &type, uniform_name_buffer.data());

            const auto name = std::string(uniform_name_buffer.data(), length);

            const auto location = glGetUniformLocation(m_program, name.c_str());
            m_uniforms[name] = location;
        }
    }
}

}
