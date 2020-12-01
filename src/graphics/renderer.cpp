#include "renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <string_view>

namespace destroid {

namespace {

    constexpr std::size_t MAX_BATCHES = 128;
    constexpr std::size_t VERTICES_PER_QUAD = 4;
    constexpr std::size_t INDICES_PER_QUAD = 6;
    constexpr std::size_t MAX_VERTICES = VERTICES_PER_QUAD * MAX_BATCHES;
    constexpr std::size_t MAX_INDICES = INDICES_PER_QUAD * MAX_BATCHES;

    constexpr std::string_view VERTEX_SOURCE = R"(
        #version 330

        layout (location = 0) in vec2 position;
        layout (location = 1) in vec2 in_texcoord;

        out vec2 texcoord;

        uniform mat4 projection;

        void main() {
            gl_Position = projection * vec4(position, 0.0, 1.0);
            texcoord = vec2(in_texcoord.x, 1.0 - in_texcoord.y);
        }
    )";

    constexpr std::string_view FRAGMENT_SOURCE = R"(
        #version 330

        in vec2 texcoord;

        out vec4 color;

        uniform sampler2D tex_sampler;

        void main() {
            color = texture(tex_sampler, texcoord);
        }
    )";

    void rotatePoint(glm::vec2& point, float sin, float cos)
    {
        point = glm::vec2{
            point.x * cos - point.y * sin,
            point.x * sin + point.y * cos
        };
    }

} // namespace

Renderer::Renderer()
    : m_shader(VERTEX_SOURCE, FRAGMENT_SOURCE)
    , m_batchCount(0)
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

    // TL       TR
    //  0 ----- 1   ASCII Art Time!
    //  | A   / |   Quads are assembled from four vertices in the following order:
    //  |   /   |   0, 1, 2 = Triangle A
    //  | /   B |   2, 1, 3 = Triangle B
    //  2 ----- 3
    // BL       BR

    std::array<GLushort, MAX_INDICES> indices;
    for (unsigned short i = 0; i < MAX_BATCHES; i++) {
        indices[i * INDICES_PER_QUAD + 0] = i * VERTICES_PER_QUAD + 0;
        indices[i * INDICES_PER_QUAD + 1] = i * VERTICES_PER_QUAD + 1;
        indices[i * INDICES_PER_QUAD + 2] = i * VERTICES_PER_QUAD + 2;
        indices[i * INDICES_PER_QUAD + 3] = i * VERTICES_PER_QUAD + 2;
        indices[i * INDICES_PER_QUAD + 4] = i * VERTICES_PER_QUAD + 1;
        indices[i * INDICES_PER_QUAD + 5] = i * VERTICES_PER_QUAD + 3;
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

    const glm::vec2 origin{ region.width * 0.5F, region.height * 0.5F };

    // Vertex coordinates
    glm::vec2 positionTL = glm::vec2{ -origin.x, -origin.y } * scale;
    glm::vec2 positionTR = glm::vec2{ origin.x, -origin.y } * scale;
    glm::vec2 positionBL = glm::vec2{ -origin.x, origin.y } * scale;
    glm::vec2 positionBR = glm::vec2{ origin.x, origin.y } * scale;

    if (angle != 0.F) {
        const float sin = glm::sin(glm::radians(-angle));
        const float cos = glm::cos(glm::radians(-angle));

        rotatePoint(positionTL, sin, cos);
        rotatePoint(positionTR, sin, cos);
        rotatePoint(positionBL, sin, cos);
        rotatePoint(positionBR, sin, cos);
    }

    const glm::vec2 worldPosition = position + origin;

    positionTL += worldPosition;
    positionTR += worldPosition;
    positionBL += worldPosition;
    positionBR += worldPosition;

    // Texture atlas coordinates
    const float left = region.left() / texture.width();
    const float right = region.right() / texture.width();
    const float top = region.top() / texture.height();
    const float bottom = region.bottom() / texture.height();

    m_vertices.emplace_back(Vertex { positionTL, glm::vec2{ left, top } });
    m_vertices.emplace_back(Vertex { positionTR, glm::vec2{ right, top } });
    m_vertices.emplace_back(Vertex { positionBL, glm::vec2{ left, bottom } });
    m_vertices.emplace_back(Vertex { positionBR, glm::vec2{ right, bottom } });

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
