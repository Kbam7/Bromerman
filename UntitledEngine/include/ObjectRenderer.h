#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Vertex.h"

namespace UntitledEngine {

// Each render batch is used for a single draw call
/*
 * @param offset: Offeset in vertex buffer
*/
class RenderBatch {
public:
    RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset),
        numVertices(NumVertices), texture(Texture) {
    }
    GLuint offset;
    GLuint numVertices;
    GLuint texture;
};

// The ObjectRenderer class is a more efficient way of drawing sprites
class ObjectRenderer
{
public:
    ObjectRenderer();
    ~ObjectRenderer();

    // Initializes the ObjectRenderer
    void init();
    void dispose();

	void begin();
    // Ends the ObjectRenderer
    void end();

    // Adds a glyph to the ObjectRenderer
    void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
    // Adds a glyph to the ObjectRenderer with rotation
    void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle);
    // Adds a glyph to the ObjectRenderer with rotation
    void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir);

    // Renders the entire ObjectRenderer to the screen
    void renderBatch();

private:
    // Creates all the needed RenderBatches
    void createRenderBatches();

    // Generates our VAO and VBO
    void createVertexArray();

    GLuint _vbo;
    GLuint _vao;

    std::vector<RenderBatch> _renderBatches;
};

}