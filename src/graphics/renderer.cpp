#include "renderer.hpp"
#include "math/rectangle.hpp"
#include "texture.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <string>

namespace destroid {

struct Vertex {
    glm::vec2 position;
    glm::vec2 texcoord;
};

namespace {

constexpr std::size_t totalBatches = 128;
constexpr std::size_t verticesPerQuad = 4;
constexpr std::size_t indicesPerQuad = 6;
constexpr std::size_t totalVertices = verticesPerQuad * totalBatches;
constexpr std::size_t totalIndices = indicesPerQuad * totalBatches;

constexpr const char* vertexSource = R"(
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

constexpr const char* fragmentSource = R"(
    #version 330

    in vec2 frag_texcoord;

    out vec4 color;

    uniform sampler2D tex_sampler;

    void main() {
        color = texture(tex_sampler, frag_texcoord);
    }
)";

std::vector<GLushort> generateIndices()
{
    std::vector<GLushort> indices;
    indices.reserve(totalIndices);

    for (GLushort i = 0; i < totalBatches; i++) {
        indices.emplace_back(i * verticesPerQuad + 0);
        indices.emplace_back(i * verticesPerQuad + 1);
        indices.emplace_back(i * verticesPerQuad + 2);
        indices.emplace_back(i * verticesPerQuad + 2);
        indices.emplace_back(i * verticesPerQuad + 1);
        indices.emplace_back(i * verticesPerQuad + 3);
    }

    return indices;
}

const void* makeOffset(std::uintptr_t size)
{
    return reinterpret_cast<const void*>(size);
}

} // namespace

Renderer::Renderer()
    : m_shader(vertexSource, fragmentSource)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    m_vertices.reserve(totalVertices);
    const GLuint vboSize = totalVertices * sizeof(Vertex);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vboSize, nullptr, GL_STREAM_DRAW);

    const auto indices = generateIndices();
    const GLuint iboSize = totalIndices * sizeof(GLushort);
    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, iboSize, indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), makeOffset(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), makeOffset(sizeof(glm::vec2)));

    m_shader.use();
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
    const glm::mat4x4 view = glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
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

    auto transform = glm::mat3(1.0f);
    transform = glm::translate(transform, position);
    transform = glm::rotate(transform, glm::radians(angle));
    transform = glm::scale(transform, glm::vec2(scale, scale));
    transform = glm::translate(transform, -glm::vec2(region.width, region.height) / 2.0f);

    const float leftUV = region.left() / texture.width();
    const float rightUV = region.right() / texture.width();
    const float topUV = region.top() / texture.height();
    const float bottomUV = region.bottom() / texture.height();

    // clang-format off
    m_vertices.emplace_back(Vertex {
        transform * glm::vec3(0.0f, 0.0f, 1.0f), 
        glm::vec2(leftUV, topUV)});

    m_vertices.emplace_back(Vertex {
        transform * glm::vec3(region.width, 0.0f, 1.0f), 
        glm::vec2(rightUV, topUV)});

    m_vertices.emplace_back(Vertex {
        transform * glm::vec3(0.0f, region.height, 1.0f), 
        glm::vec2(leftUV, bottomUV)});

    m_vertices.emplace_back(Vertex {
        transform * glm::vec3(region.width, region.height, 1.0f), 
        glm::vec2(rightUV, bottomUV)});
    // clang-format on

    m_batchCount += 1;
}

void Renderer::finish()
{
    if (m_vertices.empty()) {
        return;
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(Vertex), m_vertices.data());
    glDrawElements(GL_TRIANGLES, m_batchCount * indicesPerQuad, GL_UNSIGNED_SHORT, nullptr);

    m_vertices.clear();
    m_batchCount = 0;
}

} // namespace destroid
