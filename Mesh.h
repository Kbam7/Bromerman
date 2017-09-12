#ifndef BASICOBJECT_H
#define BASICOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <GLSLProgram.h>
#include <Camera3D.h>
#include <GLTexture.h>

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

/*struct Texture {
	GLuint      id;
	std::string type;
	aiString    path;
};*/

class Mesh {
public:
	/*  Mesh Data  */
	std::vector<Vertex>                     vertices;
	std::vector<unsigned int>               indices;
	std::vector<UntitledEngine::GLTexture>  textures;
	GLuint                                  m_vao;

	~Mesh();
	Mesh(std::vector<Vertex> & vertices, std::vector<unsigned int> & indices, std::vector<UntitledEngine::GLTexture> & textures);

	// render the mesh
	void draw(UntitledEngine::Camera3D *m_camera, UntitledEngine::GLSLProgram *m_shader);



private:
	Mesh();

	GLuint                      m_vbo;   // vertex buffer
	GLuint                      m_ebo;   // Element/Index buffer

	// initializes all the buffer objects/arrays
	void setupMesh();


};

#endif //BASICOBJECT_H
