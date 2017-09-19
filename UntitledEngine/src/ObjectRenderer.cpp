#include "ObjectRenderer.h"

//#include <algorithm>

namespace UntitledEngine {

	ObjectRenderer::ObjectRenderer() : _totalNumVertices(0), m_vao(0), m_vbo(0), m_ebo(0)
	{
	}

	ObjectRenderer::~ObjectRenderer()
	{
	}

	void ObjectRenderer::init() {
		createBufferObjects();
	}

	void ObjectRenderer::dispose() {
		if (m_vao != 0) {
			glDeleteVertexArrays(1, &m_vao);
			m_vao = 0;
		}
		if (m_vbo != 0) {
			glDeleteBuffers(1, &m_vbo);
			m_vbo = 0;
		}
		if (m_ebo != 0) {
			glDeleteBuffers(1, &m_ebo);
			m_ebo = 0;
		}
	}

	void ObjectRenderer::begin(MeshSortType sortType /* MeshSortType::TEXTURE */) {
		_sortType = sortType;
		_mesh_ptrs.clear();
		_renderBatches.clear();
	}

	void ObjectRenderer::end() {
		// Sort and create batches
		//sortGlyphs();
		createRenderBatches();
	}

	void ObjectRenderer::add(Model *model) {
		//_glyphs.emplace_back(destRect, uvRect, texture, depth, color);

		// Store pointers for fast sorting before creating render batches
		for (Mesh mesh : model->meshes) {
			_mesh_ptrs.push_back(&mesh);
		}
		this->_totalNumVertices += model->m_totalNumVertices;
	}

	void ObjectRenderer::renderBatches(GLuint shaderID) {

		// Bind our VAO. This sets up the opengl state we need, including the 
		// vertex attribute pointers and it binds the VBO
		glBindVertexArray(m_vao);

		for (size_t i = 0; i < _renderBatches.size(); i++) {
			//glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
			activateAndBindTextures(_renderBatches[i].textures, shaderID);

			//glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
			glDrawElements(GL_TRIANGLES, _renderBatches[i].numIndices, GL_UNSIGNED_INT, &_indices[_renderBatches[i].index_offset]);
		}

		glBindVertexArray(0);
	}

	void ObjectRenderer::createRenderBatches() {

		std::cout << "---createRenderBatches()-START---\n"; // debug

		// This will store all the vertices that we need to upload
		std::vector <Vertex> vertices;
		// Resize the buffer to the exact size we need so we can treat
		// it like an array
		vertices.resize(_totalNumVertices);

		std::cout << "---createRenderBatches()-HERE-1---\n"; // debug

		if (_mesh_ptrs.empty())
			return;

		GLuint vert_offset = 0;     // current vertex offset
		GLuint index_offset = 0;    // current index offset

		std::cout << "---createRenderBatches()-HERE-2---\n"; // debug

		//Add the first mesh to batch
		/*_renderBatches.emplace_back(vert_offset, 6*//*_mesh_ptrs[0]->m_nVertices*//*,
		                            index_offset, 6*//*_mesh_ptrs[0]->indices.size()*//*,
		                            _mesh_ptrs[0]->textures);
		*/
		//_renderBatches.emplace_back(vert_offset, 6/*_mesh_ptrs[0]->m_nVertices*/,
		//                            index_offset, 2/*_mesh_ptrs[0]->indices.size()*/,
		//                            _mesh_ptrs[0]->textures);

		/*ObjectRenderBatch batch(vert_offset, (GLuint)_mesh_ptrs[0]->m_nVertices,
		                        index_offset, (GLuint)_mesh_ptrs[0]->indices.size(),
		                        _mesh_ptrs[0]->textures);

		_renderBatches.push_back(batch);*/

		std::cout << "---createRenderBatches()-HERE-3---\n"; // debug

		// Add first meshes vertices and indices to collection
		//auto beg =  _mesh_ptrs[0]->vertices.begin();
		//auto end =  _mesh_ptrs[0]->vertices.end();
		//vertices.insert(vertices.end(), beg, end);
		for (auto vert : _mesh_ptrs[0]->vertices) {
			//vertices.push_back(vert);
			std::cout << "POS: " << vert.Position.x << std::endl; // debug
		}

		std::cout << "---createRenderBatches()-HERE-3-1---\n"; // debug

		// Increase offsets
		vert_offset += _mesh_ptrs[0]->m_nVertices;

		std::cout << "---createRenderBatches()-HERE-3-2---\n"; // debug

		index_offset += _mesh_ptrs[0]->indices.size();

		std::cout << "---createRenderBatches()-HERE-4---\n"; // debug

		//Add the rest of the meshes
		for (size_t i = 1; i < _mesh_ptrs.size(); i++) {

			/*
			 * WARNING: Need to compare texture arrays below. Testing out '!=' operator for std::vector
			 */
			// Check if this mesh can be part of the current batch
			//if (!checkTextures(_mesh_ptrs[i]->textures, _mesh_ptrs[i - 1]->textures)) {
			if (_mesh_ptrs[i]->textures[0].id !=_mesh_ptrs[i - 1]->textures[0].id) {
				// Make a new batch
				_renderBatches.emplace_back(vert_offset, _mesh_ptrs[i]->m_nVertices,
				                            index_offset, _mesh_ptrs[i]->indices.size(),
				                            _mesh_ptrs[i]->textures);
			} else {
				// If its part of the current batch, just increase the index and vertex count
				_renderBatches.back().numVertices += _mesh_ptrs[i]->m_nVertices;
				_renderBatches.back().numIndices += _mesh_ptrs[i]->indices.size();
			}

			// Add mesh vertices to collection
			vertices.insert(vertices.end(), _mesh_ptrs[i]->vertices.begin(), _mesh_ptrs[i]->vertices.end());
			// Add mesh indices to collection
			_indices.insert(_indices.end(), _mesh_ptrs[i]->indices.begin(), _mesh_ptrs[i]->indices.end());

			// Increase offsets
			vert_offset += _mesh_ptrs[i]->m_nVertices;
			index_offset += _mesh_ptrs[i]->indices.size();
		}

		std::cout << "---createRenderBatches()-HERE-5---\n"; // debug

		// Bind our VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		// Orphan the buffer (for speed)
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		// Bind EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		// Setup indices for vertex data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_DYNAMIC_DRAW);

		// Unind the EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		// Unbind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		std::cout << "---createRenderBatches()-END---\n"; // debug

	}

	void ObjectRenderer::createBufferObjects() {

		// create buffers/arrays
		if (m_vao == 0)
			glGenVertexArrays(1, &m_vao);
		if (m_vao == 0)
			glGenBuffers(1, &m_vbo);
		if (m_vao == 0)
			glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(UntitledEngine::Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(UntitledEngine::Vertex), (void*)offsetof(UntitledEngine::Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(UntitledEngine::Vertex), (void*)offsetof(UntitledEngine::Vertex, TexCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(UntitledEngine::Vertex), (void*)offsetof(UntitledEngine::Vertex, Tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(UntitledEngine::Vertex), (void*)offsetof(UntitledEngine::Vertex, Bitangent));

		glBindVertexArray(0);


/*
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

		//This is the position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, position));
		//This is the color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, color));
		//This is the UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, uv));

		glBindVertexArray(0);
*/

	}

	void ObjectRenderer::sortGlyphs() {

		switch (_sortType) {
			case MeshSortType::nVERTICES_DESC:
				std::stable_sort(_mesh_ptrs.begin(), _mesh_ptrs.end(), nVerticesDescending);
				break;
			case MeshSortType::nVERTICES_ASC:
				std::stable_sort(_mesh_ptrs.begin(), _mesh_ptrs.end(), nVerticesAscending);
				break;
			case MeshSortType::TEXTURES:
				std::stable_sort(_mesh_ptrs.begin(), _mesh_ptrs.end(), compareTextures);
				break;
			case MeshSortType::NONE:
			default:
				break;
		}
	}

	bool ObjectRenderer::nVerticesDescending(Mesh* a, Mesh* b) {
		return (a->m_nVertices < b->m_nVertices);
	}

	bool ObjectRenderer::nVerticesAscending(Mesh* a, Mesh* b) {
		return (a->m_nVertices > b->m_nVertices);
	}

	bool ObjectRenderer::compareTextures(Mesh* a, Mesh* b) {
		return (a->textures[0].id < b->textures[0].id);
	}

/*	bool checkTextures(std::vector<UntitledEngine::GLTexture> a, std::vector<UntitledEngine::GLTexture> b) {
		bool flag = false;

		for (size_t i = 0; i < a.size(); ++i) {

			for (size_t j = 0; j < b.size(); ++j) {
				if (a[i].id == b[j].id) {
					flag = true;
					break;
				}
			}
			if (j == b.size() && flag == true)


		}

		return (flag);
	}*/

}