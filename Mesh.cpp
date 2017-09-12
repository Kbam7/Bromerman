#include <UntitledEngineErrors.h>
#include <iostream>
#include <ResourceManager.h>
#include <sstream>
#include <types.h>
#include "Mesh.h"
#include "UntitledEngine/include/ObjectLoader.h"

Mesh::Mesh() {
	// empty
}

Mesh::~Mesh() {
	// empty
}

Mesh::Mesh(std::vector<Vertex> & vertices, std::vector<unsigned int> & indices, std::vector<UntitledEngine::GLTexture> & textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	// now that we have all the required data, set the vertex buffers and its attribute pointers.
	this->setupMesh();
}

/*Mesh::Mesh(const std::string & path, const std::string & vertShader,
                         const std::string & fragShader, const std::vector<std::string> & attrs)
{
	// Create and bind VAO for configuration
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Setup shaders
	if (!vertShader.empty() && !fragShader.empty())
		this->initShaders(vertShader, fragShader, attrs);

	// Load object from file
	std::vector<unsigned short> indices;
	std::vector<glm::vec3>      indexed_vertices;
	std::vector<glm::vec2>      indexed_uvs;
	std::vector<glm::vec3>      indexed_normals;
	//std::vector<std::string>    textures;

	bool res = UntitledEngine::loadAssImp(path.c_str(), indices, indexed_vertices, indexed_uvs, indexed_normals);
	if (!res)
		UntitledEngine::fatalError("Unable to load object " + path);

	//m_texture = UntitledEngine::ResourceManager::getTexture("Textures/player.png");

	// Load data into VBO's
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &m_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glGenBuffers(1, &m_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glGenBuffers(1, &m_normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Index array for vertices
	glGenBuffers(1, &m_elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
	m_nIndices = indices.size();

	// Unbind configured VAO
	glBindVertexArray(0);
}*/

/*void Mesh::render() {
	// Use our shader
	m_shader.use();

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
	m_shader.unuse();
}*/


void Mesh::setupMesh()
{
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
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}

// render the mesh
void Mesh::draw(UntitledEngine::Camera3D *m_camera, UntitledEngine::GLSLProgram *m_shader)
{
	// Use our shader
	m_shader->use();

// -----------[Assign uniform variables]------------
	// Compute the MVP matrix from keyboard and mouse input
	//glm::mat4 ViewMatrix = m_camera->get_viewMatrix();
	//glm::mat4 ModelMatrix = m_camera->get_modelMatrix();
	glm::mat4 MVP = m_camera->get_mvpMatrix();

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	glUniformMatrix4fv(m_shader->getUniformLocation("MVP"), 1, GL_FALSE, &MVP[0][0]);

	/*
	 * WARNING!!!::: Swap "V" and "M" and check for differences. I think the values were messed up originally
	 * */

	//glUniformMatrix4fv(m_shader->getUniformLocation("M"), 1, GL_FALSE, &ModelMatrix[0][0]);
	//glUniformMatrix4fv(m_shader->getUniformLocation("V"), 1, GL_FALSE, &ViewMatrix[0][0]);

	// Calculate lighting position
	//glm::vec3 lightPos = glm::vec3(4,4,4);
	//glUniform3f(m_shader->getUniformLocation("LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);
// ----------------------------


// -----------[Activate and bind textures]------------
	// bind appropriate textures
	unsigned int diffuseNr  = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr   = 1;
	unsigned int heightNr   = 1;
	for(size_t i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::stringstream ss;
		std::string number;
		std::string name = textures[i].type;
		if(name == "texture_diffuse")
			ss << diffuseNr++; // transfer unsigned int to stream
		else if(name == "texture_specular")
			ss << specularNr++; // transfer unsigned int to stream
		else if(name == "texture_normal")
			ss << normalNr++; // transfer unsigned int to stream
		else if(name == "texture_height")
			ss << heightNr++; // transfer unsigned int to stream
		number = ss.str();
		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(m_shader->get_programID(), (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
// ----------------------------


// -----------[Render and cleanup]------------
	// draw mesh
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);

	m_shader->unuse();
// ----------------------------
}