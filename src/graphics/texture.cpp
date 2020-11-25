#include "texture.hpp"
#include <stb_image.h>
#include <array>
#include <stdexcept>

namespace destroid {

Texture::Texture(const std::string& path)
{
    glGenTextures(1, &m_handle);
    bind();

    int channels;
    unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &channels, STBI_rgb_alpha);

    if (!data) {
        throw std::runtime_error("Failed to load texture: " + path);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_handle);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_handle);
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
