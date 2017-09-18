#ifndef BROMERMAN_MODEL_H
#define BROMERMAN_MODEL_H


#include <scene.h>
#include <Importer.hpp>
#include <postprocess.h>
#include <iostream>
#include <ResourceManager.h>
#include "Mesh.h"

class Model {

public:
	std::vector<Mesh>           meshes;
	std::string                 m_directory;
	glm::vec4                   m_direction;
	glm::vec4                   m_position;
	UntitledEngine::GLSLProgram m_shader;
	UntitledEngine::Camera3D    *m_camera;
	size_t                      m_totalNumVertices;

	// constructor, expects a filepath to a 3D model.
	Model(std::string const &path, UntitledEngine::Camera3D *camera);

	// Create GLSL shader using vert and frag shader paths. attrs vector contains names of attributes in vertex shader.
	void initShaders(const std::string & vertShader, const std::string & fragShader, const std::vector<std::string> & attrs);

	// draws the model
	void draw(void);

private:
	// loads a model with supported ASSIMP extensions
	void loadModel(std::string const &path);

	// processes a node in a recursive fashion.
	void processNode(aiNode *node, const aiScene *scene);

	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	std::vector<UntitledEngine::GLTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

};


#endif //BROMERMAN_MODEL_H
