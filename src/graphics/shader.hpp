#pragma once

#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <string>
#include <unordered_map>

namespace destroid {

struct ShaderUniform {
    std::string m_name;
    GLint m_id;
};

class Shader {
public:
    explicit Shader(std::string_view vertSource, std::string_view fragSource);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void setUniform(const std::string& name, const glm::mat4x4& value);
    void bind() const;

private:
    GLuint uniformLocation(const std::string& name);

    GLuint m_handle;
    std::unordered_map<std::string, GLuint> m_uniformCache;
};

} // namespace destroid
