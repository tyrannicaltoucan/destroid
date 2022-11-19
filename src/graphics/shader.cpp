#include "shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

namespace destroid {
namespace {

GLuint compileShader(GLenum type, const std::string& source)
{
    const GLuint shader = glCreateShader(type);
    const GLchar* sourcePtr = source.c_str();
    glShaderSource(shader, 1, &sourcePtr, nullptr);
    glCompileShader(shader);

    GLint status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        auto typeString = ([type]() -> std::string {
            switch (type) {
            case GL_VERTEX_SHADER:
                return "vertex";
            case GL_FRAGMENT_SHADER:
                return "fragment";
            default:
                return "unknown";
            }
        });

        auto log = std::string(logLength - 1, '\0');
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        throw std::runtime_error("Failed to compile " + typeString() + " shader:\n" + log);
    }

    return shader;
}

void linkShaders(GLuint program, GLuint vertShader, GLuint fragShader)
{
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    GLint status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

        auto log = std::string(logLength - 1, '\0');
        glGetProgramInfoLog(program, logLength, nullptr, log.data());
        throw std::runtime_error("Failed to link shaders:\n" + log);
    }

    // Some drivers ref-count shader handles and will not delete them until all
    // refrences to them are removed (via detaching) first.
    glDetachShader(program, vertShader);
    glDetachShader(program, fragShader);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

} // namespace

Shader::Shader(const std::string& vertSource, const std::string& fragSource)
    : m_handle(glCreateProgram())
{
    const GLuint vertShader = compileShader(GL_VERTEX_SHADER, vertSource);
    const GLuint fragShader = compileShader(GL_FRAGMENT_SHADER, fragSource);
    linkShaders(m_handle, vertShader, fragShader);
}

Shader::~Shader()
{
    glDeleteProgram(m_handle);
}

void Shader::setUniform(const std::string& name, const glm::mat4x4& value)
{
    const GLint location = glGetUniformLocation(m_handle, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::use() const
{
    glUseProgram(m_handle);
}

} // namespace destroid
