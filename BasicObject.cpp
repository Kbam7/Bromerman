#include <UntitledEngineErrors.h>
#include "BasicObject.h"
#include "UntitledEngine/include/ObjectLoader.h"

BasicObject::BasicObject(const std::string & path, const std::string & vertShader,
                         const std::string & fragShader, const std::vector<std::string> & attrs) :
	m_direction({0,0,0,0}),
	m_position({0,0,0,1})
{
	// Setup shaders
	if (!vertShader.empty() && !fragShader.empty())
		init_shaders(vertShader, fragShader, attrs);

	// Load object from file
	std::vector<unsigned short> indices;
	std::vector<glm::vec3>      indexed_vertices;
	std::vector<glm::vec2>      indexed_uvs;
	std::vector<glm::vec3>      indexed_normals;

	bool res = UntitledEngine::loadAssImp(path.c_str(), indices, indexed_vertices, indexed_uvs, indexed_normals);
	if (!res)
		UntitledEngine::fatalError("Unable to load object " + path);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Load data into VBO's
	glGenBuffers(1, &m_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	// Index array for vertices
	glGenBuffers(1, &m_elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
	m_nIndices = indices.size();

	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 3rd attribute buffer : normals
	glBindBuffer(GL_ARRAY_BUFFER, m_normalbuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);

	glBindVertexArray(0);
}

BasicObject::~BasicObject() {

}

void BasicObject::init(glm::vec3 direction, glm::vec3 position, UntitledEngine::Camera3D *camera) {
	m_direction = glm::vec4(direction, 0);
	m_position = glm::vec4(position, 1);
	m_camera = camera;
}

void BasicObject::init_shaders(const std::string & vertShader, const std::string & fragShader,
                               const std::vector<std::string> & attrs) {
	// Compile shader program
	m_shaderProgram.compileShaders(vertShader, fragShader);
	if (!attrs.empty()) {
		for (const auto &attr : attrs)
			m_shaderProgram.addAttribute(attr);
	}
	m_shaderProgram.linkShaders();

	// Get matrix uniform
	m_mvp_uniform = m_shaderProgram.getUniformLocation("MVP");
	m_model_uniform = m_shaderProgram.getUniformLocation("V");
	m_view_uniform = m_shaderProgram.getUniformLocation("M");

	// Get texture uniform
	m_texture_uniform  = m_shaderProgram.getUniformLocation("myTextureSampler");

	// Get light position uniform
	m_lightPos_uniform = m_shaderProgram.getUniformLocation("LightPosition_worldspace");
}

void BasicObject::render() {
	// Use our shader
	m_shaderProgram.use();

	// Compute the MVP matrix from keyboard and mouse input
	glm::mat4 ViewMatrix = m_camera->get_viewMatrix();
	glm::mat4 ModelMatrix = m_camera->get_modelMatrix();
	glm::mat4 MVP = m_camera->get_mvpMatrix();

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	glUniformMatrix4fv(m_mvp_uniform, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(m_model_uniform, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(m_view_uniform, 1, GL_FALSE, &ViewMatrix[0][0]);

	glm::vec3 lightPos = glm::vec3(4,4,4);
	glUniform3f(m_lightPos_uniform, lightPos.x, lightPos.y, lightPos.z);

	glBindVertexArray(m_vao);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture.id);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(m_texture_uniform, 0);

	// Draw the triangles !
	glDrawElements(
			GL_TRIANGLES,      // mode
			m_nIndices,        // count
			GL_UNSIGNED_SHORT, // type
			(void*)0           // element array buffer offset
	);

	glBindVertexArray(0);
	m_shaderProgram.unuse();
}
