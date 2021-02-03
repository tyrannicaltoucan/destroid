#pragma once

#include <glad/gl.h>
#include <glm/fwd.hpp>
#include <string>
#include <unordered_map>

namespace destroid {

class Shader {
public:
    explicit Shader(const std::string& vertSource, const std::string& fragSource);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void setUniform(const std::string& name, const glm::mat4x4& value);
    void bind() const;

private:
    GLint uniformLocation(const std::string& name);

    GLuint m_handle;
    std::unordered_map<std::string, GLint> m_uniformCache;
};

} // namespace destroid
