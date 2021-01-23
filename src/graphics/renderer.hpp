#pragma once

#include "shader.hpp"
#include "texture.hpp"
#include "base/rectangle.hpp"
#include <glad/gl.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

namespace destroid {

struct Vertex {
    glm::vec2 position;
    glm::vec2 texcoord;
};

class Renderer {
public:
    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void setViewBounds(float width, float height);
    void clearColor(const glm::vec4& color) const;

    void draw(
        const Texture& texture,
        const Rectangle& region,
        const glm::vec2& position,
        float angle,
        float scale);

    void finish();

private:
    std::vector<Vertex> m_vertices;
    GLuint m_boundTexureHandle = 0;
    GLsizei m_batchCount = 0;
    Shader m_shader;

    GLuint m_vbo = 0;
    GLuint m_ibo = 0;
    GLuint m_vao = 0;
};

} // namespace destroid
