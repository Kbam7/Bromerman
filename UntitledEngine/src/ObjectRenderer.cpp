#include "ObjectRenderer.h"

#include <algorithm>

namespace UntitledEngine {

	ObjectRenderer::ObjectRenderer() : _vbo(0), _vao(0) {
	}

	ObjectRenderer::~ObjectRenderer() {
	}

	void ObjectRenderer::init() {
		createVertexArray();
	}

	void ObjectRenderer::dispose() {
		if (_vao != 0) {
			glDeleteVertexArrays(1, &_vao);
			_vao = 0;
		}
		if (_vbo != 0) {
			glDeleteBuffers(1, &_vbo);
			_vbo = 0;
		}
	}

	void ObjectRenderer::begin() {

	}

	void ObjectRenderer::end() {
		createRenderBatches();
	}

	void ObjectRenderer::draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth,
	                          const ColorRGBA8 &color) {
		//_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void ObjectRenderer::draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth,
	                          const ColorRGBA8 &color, float angle) {
		//_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void ObjectRenderer::draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth,
	                          const ColorRGBA8 &color, const glm::vec2 &dir) {
		//const glm::vec2 right(1.0f, 0.0f);
		//float angle = acos(glm::dot(right, dir));
		//if (dir.y < 0.0f) angle = -angle;

		//_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void ObjectRenderer::renderBatch() {

		// Bind our VAO. This sets up the opengl state we need, including the
		// vertex attribute pointers and it binds the VBO
		glBindVertexArray(_vao);

		for (size_t i = 0; i < _renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}

		glBindVertexArray(0);
	}

	void ObjectRenderer::createRenderBatches() {
		// This will store all the vertices that we need to upload
		std::vector<Vertex> vertices;
		// Resize the buffer to the exact size we need so we can treat
		// it like an array
		vertices.resize(_glyphPointers.size() * 6);

		if (_glyphPointers.empty()) {
			return;
		}

		int offset = 0; // current offset
		int cv = 0; // current vertex

		//Add the first batch
		_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture);
		/* ... fill other batches */

		// Bind our VBO
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		// Orphan the buffer (for speed)
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		// Upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		// Unbind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void ObjectRenderer::createVertexArray() {

		// Generate the VAO if it isn't already generated
		if (_vao == 0) {
			glGenVertexArrays(1, &_vao);
		}

		// Bind the VAO. All subsequent opengl calls will modify it's state.
		glBindVertexArray(_vao);

		//Generate the VBO if it isn't already generated
		if (_vbo == 0) {
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//Tell opengl what attribute arrays we need
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);


		/* glVertexAttribPointer( attribute, size, type, normalised?, stride, array_buffer_offset ) */
		//This is the position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));
		//This is the color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *) offsetof(Vertex, color));
		//This is the UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, uv));

		glBindVertexArray(0);

	}

	void ObjectRenderer::sortGlyphs() {

		switch (_sortType) {
			case GlyphSortType::BACK_TO_FRONT:
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
				break;
			case GlyphSortType::FRONT_TO_BACK:
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
				break;
			case GlyphSortType::TEXTURE:
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
				break;
			case GlyphSortType::NONE:
			default:
				break;
		}
	}

	bool ObjectRenderer::compareFrontToBack(Glyph *a, Glyph *b) {
		return (a->depth < b->depth);
	}

	bool ObjectRenderer::compareBackToFront(Glyph *a, Glyph *b) {
		return (a->depth > b->depth);
	}

	bool ObjectRenderer::compareTexture(Glyph *a, Glyph *b) {
		return (a->texture < b->texture);
	}

}