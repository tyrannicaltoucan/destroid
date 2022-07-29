#include "texture.hpp"
#include <stb_image.h>
#include <iostream>

namespace destroid {

Texture::Texture(const std::string& path)
{
    glGenTextures(1, &m_handle);
    glBindTexture(GL_TEXTURE_2D, m_handle);

    unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, nullptr, STBI_rgb_alpha);
    if (!data) {
        std::cerr << "Unable to load texture: " << path << "\n";
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_handle);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

GLuint Texture::handle() const
{
    return m_handle;
}

int Texture::width() const
{
    return m_width;
}

int Texture::height() const
{
    return m_height;
}

} // namespace destroid
