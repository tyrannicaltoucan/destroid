#include "renderer.hpp"
#include "texture.hpp"
#include "base/rectangle.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <string>

namespace destroid {

struct Vertex {
    glm::vec2 position;
    glm::vec2 texcoord;
};

namespace {

    constexpr std::size_t MAX_BATCHES = 128;
    constexpr std::size_t VERTICES_PER_QUAD = 4;
    constexpr std::size_t INDICES_PER_QUAD = 6;
    constexpr std::size_t MAX_VERTICES = VERTICES_PER_QUAD * MAX_BATCHES;
    constexpr std::size_t MAX_INDICES = INDICES_PER_QUAD * MAX_BATCHES;

    const std::string VERTEX_SOURCE = R"(
        #version 330

        layout (location = 0) in vec2 position;
        layout (location = 1) in vec2 texcoord;

        out vec2 frag_texcoord;

        uniform mat4 projection;

        void main() {
            gl_Position = projection * vec4(position, 0.0, 1.0);
            frag_texcoord = vec2(texcoord.x, 1.0 - texcoord.y);
        }
    )";

    const std::string FRAGMENT_SOURCE = R"(
        #version 330

        in vec2 frag_texcoord;

        out vec4 color;

        uniform sampler2D tex_sampler;

        void main() {
            color = texture(tex_sampler, frag_texcoord);
        }
    )";

} // namespace

Renderer::Renderer()
    : m_shader(VERTEX_SOURCE, FRAGMENT_SOURCE)
{
    m_vertices.reserve(MAX_VERTICES);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(Vertex) * m_vertices.capacity(),
        nullptr,
        GL_STREAM_DRAW);

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

    std::vector<GLushort> indices;
    indices.reserve(MAX_INDICES);
    for (GLushort i = 0; i < MAX_BATCHES; i++) {
        indices.emplace_back(i * VERTICES_PER_QUAD + 0);
        indices.emplace_back(i * VERTICES_PER_QUAD + 1);
        indices.emplace_back(i * VERTICES_PER_QUAD + 2);
        indices.emplace_back(i * VERTICES_PER_QUAD + 2);
        indices.emplace_back(i * VERTICES_PER_QUAD + 1);
        indices.emplace_back(i * VERTICES_PER_QUAD + 3);
    }

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLushort) * MAX_INDICES,
        indices.data(),
        GL_STATIC_DRAW);

    const GLuint positionAttribID = 0;
    glEnableVertexAttribArray(positionAttribID);
    glVertexAttribPointer(
        positionAttribID,
        glm::vec2::length(),
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(0));

    const GLuint texcoordAttribID = 1;
    glEnableVertexAttribArray(texcoordAttribID);
    glVertexAttribPointer(
        texcoordAttribID,
        glm::vec2::length(),
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(glm::vec2::length() * sizeof(GLfloat)));
}

Renderer::~Renderer()
{
    glDisable(GL_BLEND);
    glDeleteBuffers(1, &m_ibo);
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void Renderer::setViewBounds(float width, float height)
{
    const glm::mat4x4 view = glm::ortho(0.F, width, 0.F, height, -1.F, 1.F);
    m_shader.bind();
    m_shader.setUniform("projection", view);
}

void Renderer::clearColor(const glm::vec4& color) const
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::draw(
    const Texture& texture,
    const Rectangle& region,
    const glm::vec2& position,
    float angle,
    float scale)
{
    if (m_boundTexureHandle != texture.handle()) {
        finish();
        texture.bind();
        m_boundTexureHandle = texture.handle();
    }

    auto transform = glm::mat3(1.F);
    transform = glm::translate(transform, position);
    transform = glm::rotate(transform, glm::radians(angle));
    transform = glm::scale(transform, glm::vec2(scale, scale));
    transform = glm::translate(transform, -glm::vec2(region.width, region.height) / 2.F);

    const float leftUV = region.left() / texture.width();
    const float rightUV = region.right() / texture.width();
    const float topUV = region.top() / texture.height();
    const float bottomUV = region.bottom() / texture.height();

    m_vertices.emplace_back(Vertex{
        transform * glm::vec3(0.F, 0.F, 1.F),
        glm::vec2(leftUV, topUV)});

    m_vertices.emplace_back(Vertex{
        transform * glm::vec3(region.width, 0.F, 1.F),
        glm::vec2(rightUV, topUV)});

    m_vertices.emplace_back(Vertex{
        transform * glm::vec3(0.F, region.height, 1.F),
        glm::vec2(leftUV, bottomUV)});

    m_vertices.emplace_back(Vertex{
        transform * glm::vec3(region.width, region.height, 1.F),
        glm::vec2(rightUV, bottomUV)});

    m_batchCount += 1;
}

void Renderer::finish()
{
    if (m_vertices.empty()) {
        return;
    }

    glBufferSubData(
        GL_ARRAY_BUFFER,
        0,
        m_vertices.size() * sizeof(Vertex),
        m_vertices.data());

    glDrawElements(
        GL_TRIANGLES,
        m_batchCount * INDICES_PER_QUAD,
        GL_UNSIGNED_SHORT,
        nullptr);

    m_vertices.clear();
    m_batchCount = 0;
}

} // namespace destroid
