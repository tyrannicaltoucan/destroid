#pragma once

#include "shader.hpp"
#include "texture.hpp"
#include "math/rectangle.hpp"
#include <glad/gl.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

namespace destroid {

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
    struct Vertex {
        glm::vec2 position;
        glm::vec2 texcoord;
    };

    Shader m_shader;
    std::vector<Vertex> m_vertices;
    GLsizei m_batchCount;
    GLuint m_boundTexureHandle;

    GLuint m_vbo;
    GLuint m_ibo;
    GLuint m_vao;
};

} // namespace destroid
