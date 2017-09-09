#ifndef BASICOBJECT_H
#define BASICOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <GLSLProgram.h>
#include <Camera3D.h>
#include <GLTexture.h>

class BasicObject {
public:
	~BasicObject();

	BasicObject(const std::string & path, const std::string & vertShader,
	            const std::string & fragShader, const std::vector<std::string> & attrs);

	void init(glm::vec3 direction, glm::vec3 position, UntitledEngine::Camera3D* camera);
	void init_shaders(const std::string & vertShader,
	                  const std::string & fragShader,
	                  const std::vector<std::string> & attrs);
	void render();

	// getters
	GLuint get_vertices(void) const;
	GLuint get_texture(void) const;
private:
	BasicObject();

	glm::vec4                   m_direction;
	glm::vec4                   m_position;
	UntitledEngine::Camera3D    *m_camera;

	GLint                       m_mvp_uniform;
	GLint                       m_model_uniform;
	GLint                       m_view_uniform;
	GLint                       m_texture_uniform;
	GLint                       m_lightPos_uniform;

	GLuint                       m_vao;
	GLsizei                     m_nIndices;
	GLuint                      m_vertexbuffer;
	GLuint                      m_uvbuffer;
	GLuint                      m_normalbuffer;
	GLuint                      m_elementbuffer;
	UntitledEngine::GLTexture   m_texture;
	UntitledEngine::GLSLProgram m_shaderProgram;

};

#endif //BASICOBJECT_H
