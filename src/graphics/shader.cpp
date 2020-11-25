#include "shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <cassert>
#include <iostream>
#include <stdexcept>

namespace destroid {

namespace {

    GLuint compileShader(GLenum type, const char* source)
    {
        const GLuint shaderHandle = glCreateShader(type);
        glShaderSource(shaderHandle, 1, &source, nullptr);
        glCompileShader(shaderHandle);

        GLint status;
        glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE) {
            GLint logSize;
            glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logSize);

            assert(logSize > 0);

            std::string log;
            log.resize(static_cast<std::string::size_type>(logSize - 1));
            glGetShaderInfoLog(shaderHandle, logSize, nullptr, log.data());

            throw std::runtime_error("Failed to compile shader:\n" + log);
        }

        return shaderHandle;
    }

    void checkLinkStatus(GLuint programHandle)
    {
        GLint status;
        glGetProgramiv(programHandle, GL_LINK_STATUS, &status);

        if (status == GL_FALSE) {
            GLint logSize;
            glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logSize);

            assert(logSize > 0);

            std::string log;
            log.resize(static_cast<std::size_t>(logSize - 1));
            glGetProgramInfoLog(programHandle, logSize, nullptr, log.data());

            throw std::runtime_error("Failed to link shaders:\n" + log);
        }
    }

} // namespace

Shader::Shader(std::string_view vertSource, std::string_view fragSource)
    : m_handle(glCreateProgram())
{
    const GLuint vertHandle = compileShader(GL_VERTEX_SHADER, vertSource.data());
    const GLuint fragHandle = compileShader(GL_FRAGMENT_SHADER, fragSource.data());

    glAttachShader(m_handle, vertHandle);
    glAttachShader(m_handle, fragHandle);

    glLinkProgram(m_handle);
    checkLinkStatus(m_handle);

    // No need to keep the shader objects around now that they're linked to the program.
    glDeleteShader(vertHandle);
    glDeleteShader(fragHandle);
}

Shader::~Shader()
{
    glDeleteProgram(m_handle);
}

void Shader::setUniform(const std::string& name, const glm::mat4x4& value)
{
    assert(m_handle != 0);
    glUniformMatrix4fv(uniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::bind() const
{
    assert(m_handle != 0);
    glUseProgram(m_handle);
}

GLuint Shader::uniformLocation(const std::string& name)
{
    auto iter = m_uniformCache.find(name);
    if (iter != m_uniformCache.end()) {
        return iter->second;
    }

    const GLuint location = glGetUniformLocation(m_handle, name.c_str());
    m_uniformCache.emplace(name, location);

    return location;
}

} // namespace destroid
