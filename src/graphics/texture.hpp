#pragma once

#include <glad/gl.h>
#include <glm/vec2.hpp>
#include <string>
#include <type_traits>

namespace destroid {

class Texture {
public:
    explicit Texture(const std::string& path);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    void bind() const;
    GLuint handle() const;

    int width() const;
    int height() const;

private:
    GLuint m_handle;
    int m_width;
    int m_height;
};

} // namespace destroid
