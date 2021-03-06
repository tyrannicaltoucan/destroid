#pragma once

#include <glad/gl.h>
#include <string>

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
