#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Vertex.h"
#include "Mesh.h"
#include "Model.h"

namespace UntitledEngine {

	enum class MeshSortType {
		NONE,
		nVERTICES_ASC,
		nVERTICES_DESC,
		TEXTURES
	};

class ObjectRenderBatch {
public:
	ObjectRenderBatch(GLuint Vertex_offset, GLuint NumVertices, GLuint Index_offset, GLuint NumIndices, std::vector<UntitledEngine::GLTexture> & Textures) :
		    vertex_offset(Vertex_offset),
            numVertices(NumVertices),
		    index_offset(Index_offset),
		    numIndices(NumIndices),
		    textures(Textures)
    {
	    // empty
    }

    GLuint vertex_offset;
    GLuint numVertices;
	GLuint index_offset;
	GLuint numIndices;
	std::vector<UntitledEngine::GLTexture>  textures;
};

// The ObjectRenderer class is a more efficient way of drawing sprites
class ObjectRenderer
{
public:
	size_t  _totalNumVertices;

    ObjectRenderer();
    ~ObjectRenderer();

    // Initializes the ObjectRenderer
    void init();

	// Cleans up after we're done
    void dispose();

	// Set sort type and clear any existing mesh pointers or render batches
	void begin(MeshSortType sortType = MeshSortType::TEXTURES);

    // Sorts meshes and creates render batches
    void end();

	// Add objects to renderer
	void add(Model *model);

    // Renders the all batches to the screen
    void renderBatches(GLuint shaderID);

private:
	// Generates our VAO, VBO and EBO
	void createBufferObjects();

    // Creates all the needed RenderBatches
    void createRenderBatches();

	// Sorts glyphs according to _sortType
	void sortGlyphs();

	// Comparators used by sortGlyphs()
	static bool nVerticesDescending(Mesh* a, Mesh* b);
	static bool nVerticesAscending(Mesh* a, Mesh* b);
	static bool compareTextures(Mesh* a, Mesh* b);


    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;

	std::vector<Mesh *>         _mesh_ptrs;
	std::vector<unsigned int>   _indices;
	MeshSortType                _sortType;

    std::vector<ObjectRenderBatch> _renderBatches;
};

}