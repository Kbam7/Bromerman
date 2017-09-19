#include <UntitledEngineErrors.h>
#include <iostream>
#include <ResourceManager.h>
#include <sstream>
//#include <types.h>
#include "Mesh.h"
#include "ObjectLoader.h"

namespace UntitledEngine {
	void activateAndBindTextures(std::vector<UntitledEngine::GLTexture> &textures, GLuint shaderID) {
		// bind appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (size_t i = 0; i < textures.size(); i++) {
			// active proper texture unit before binding
			glActiveTexture(GL_TEXTURE0 + i);

			// retrieve texture number (the N in diffuse_textureN - see shader files)
			std::stringstream ss;
			std::string number;
			std::string type = textures[i].type;

			// transfer unsigned int to stream
			if (type == "texture_diffuse")
				ss << diffuseNr++;
			else if (type == "texture_specular")
				ss << specularNr++;
			else if (type == "texture_normal")
				ss << normalNr++;
			else if (type == "texture_height")
				ss << heightNr++;
			// Get string value of number
			number = ss.str();

			// now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(shaderID, (type + number).c_str()), i);
			// and finally bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
	}

	Mesh::Mesh() {
		// empty
	}

	Mesh::~Mesh() {
		// empty
	}

	Mesh::Mesh(std::vector<UntitledEngine::Vertex> &vertices, std::vector<unsigned int> &indices,
	           std::vector<UntitledEngine::GLTexture> &textures) {
		this->vertices = vertices;
		this->m_nVertices = vertices.size();
		this->indices = indices;
		this->textures = textures;

		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		this->setupMesh();
	}

	void Mesh::setupMesh() {
		// create buffers/arrays
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, m_nVertices * sizeof(UntitledEngine::Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(UntitledEngine::Vertex), (void *) 0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(UntitledEngine::Vertex),
		                      (void *) offsetof(UntitledEngine::Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(UntitledEngine::Vertex),
		                      (void *) offsetof(UntitledEngine::Vertex, TexCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(UntitledEngine::Vertex),
		                      (void *) offsetof(UntitledEngine::Vertex, Tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(UntitledEngine::Vertex),
		                      (void *) offsetof(UntitledEngine::Vertex, Bitangent));

		glBindVertexArray(0);
	}

// render the mesh
	void Mesh::draw(UntitledEngine::Camera3D *camera, UntitledEngine::GLSLProgram *shader) {
		shader->use();
		// Set camera MVP matrix in shader
		glm::mat4 MVP = camera->get_mvpMatrix();
		glUniformMatrix4fv(shader->getUniformLocation("MVP"), 1, GL_FALSE, &MVP[0][0]);

		// Activate and bind textures
		activateAndBindTextures(textures, shader->get_programID());

		// draw mesh
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		// set everything back to defaults
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(0);

		shader->unuse();
	}
}