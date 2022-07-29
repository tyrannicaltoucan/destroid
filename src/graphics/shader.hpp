#pragma once

#include <glad/gl.h>
#include <glm/fwd.hpp>
#include <string>

namespace destroid {

class Shader {
public:
    explicit Shader(const std::string& vertSource, const std::string& fragSource);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void setUniform(const std::string& name, const glm::mat4x4& value);
    void use() const;

private:
    GLuint m_handle;
};

} // namespace destroid
