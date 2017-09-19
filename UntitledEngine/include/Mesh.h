#ifndef BASICOBJECT_H
#define BASICOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <GLSLProgram.h>
#include <Camera3D.h>
#include <GLTexture.h>
#include <Vertex.h>

namespace UntitledEngine {

	void activateAndBindTextures(std::vector<UntitledEngine::GLTexture> &textures, GLuint shaderID);

	class Mesh {
	public:
		/*  Mesh Data  */
		std::vector<UntitledEngine::Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<UntitledEngine::GLTexture> textures;
		size_t m_nVertices;
		GLuint m_vao;

		~Mesh();

		Mesh(std::vector<UntitledEngine::Vertex> &vertices, std::vector<unsigned int> &indices,
		     std::vector<UntitledEngine::GLTexture> &textures);

		// render the mesh
		void draw(UntitledEngine::Camera3D *m_camera, UntitledEngine::GLSLProgram *m_shader);


	private:
		Mesh();

		GLuint m_vbo;   // vertex buffer
		GLuint m_ebo;   // Element/Index buffer

		// initializes all the buffer objects/arrays
		void setupMesh();


	};
}

#endif //BASICOBJECT_H
