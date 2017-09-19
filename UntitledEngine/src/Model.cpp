#include "Model.h"
#include <unistd.h>     // getcwd()
#include <sys/param.h>  // MAXPATHLEN
#include <UntitledEngineErrors.h>


namespace UntitledEngine {

	Model::Model(std::string const &path, UntitledEngine::Camera3D *camera) :
			m_direction({0, 0, 0, 0}),
			m_position({0, 0, 0, 1}),
			m_camera(camera),
			m_totalNumVertices(0) {
		loadModel(path);
	}

// Create GLSL shader using vert and frag shader paths. attrs vector contains names of attributes in vertex shader.
	void Model::initShaders(const std::string &vertShader, const std::string &fragShader,
	                        const std::vector<std::string> &attrs) {
		// Compile shader program
		m_shader.compileShaders(vertShader, fragShader);
		if (!attrs.empty()) {
			for (const auto &attr : attrs)
				m_shader.addAttribute(attr);
		}
		m_shader.linkShaders();
	}

// draws the model
	void Model::draw(void) {
		for (auto &mesh : meshes)
			mesh.draw(m_camera, &m_shader);
	}

// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void Model::loadModel(std::string const &path) {
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
		                                               aiProcess_CalcTangentSpace); // aiProcess_FlipUVs |
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}
		// retrieve the directory path of the filepath
		m_directory = getcwd(nullptr, MAXPATHLEN);

		// process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);
	}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void Model::processNode(aiNode *node, const aiScene *scene) {
		std::cout << "---HERE-1--" << std::endl; // debug
		std::cout << "nMeshes: " << node->mNumMeshes << std::endl; // debug
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			std::cout << "---HERE-3--" << std::endl; // debug
			// the node object only contains indices to index the actual objects in the scene.
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

			meshes.push_back(processMesh(mesh, scene));

			this->m_totalNumVertices += meshes.back().m_nVertices;
			std::cout << "---HERE-4--" << std::endl; // debug
		}

		std::cout << "---HERE-2--" << std::endl; // debug
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}

	}

	Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {

		std::cout << "---processMesh-START--" << std::endl; // debug

		// data to fill
		std::vector<UntitledEngine::Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<UntitledEngine::GLTexture> textures;

		std::cout << "---processMesh-HERE-1--" << std::endl; // debug

		// Walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			std::cout << "---processMesh-HERE-1.1--" << std::endl; // debug

			UntitledEngine::Vertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			std::cout << "---processMesh-HERE-1.2--" << std::endl; // debug

			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			} else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			std::cout << "---processMesh-HERE-1.3--" << std::endl; // debug

			// tangent
			if (mesh->HasTangentsAndBitangents()) {
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;

				std::cout << "---processMesh-HERE-1.4--" << std::endl; // debug

				// bitangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector;

				std::cout << "---processMesh-HERE-1.5--" << std::endl; // debug
			}

			vertices.push_back(vertex);
		}

		std::cout << "---processMesh-HERE-2--" << std::endl; // debug

		// now wak through each of the mesh's faces (a face is a triangle on a mesh) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// process materials


/*	std::cout << "hasMaterials?: " << scene->HasMaterials() << std::endl; // debug
	std::cout << "nMaterials: " << scene->mNumMaterials << std::endl; // debug

	aiString str; //mat->GetTexture(type, 0, &str); // debug
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; // debug

	std::cout << "matIndex: " << mesh->mMaterialIndex << "\n"; // debug

	for (size_t k = 0; k < scene->mNumMaterials; ++k) { // debug
		if(scene->mMaterials[k]->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS) { // debug
			std::cout << "YAY\n" << "k: " << k << " path: " << str.C_Str() << "\n"; // debug
		}
		//scene->mMaterials[i]->GetTexture()
	}

	UntitledEngine::fatalError("--HERE--"); // debug*/





		std::cout << "---processMesh-HERE-3--" << std::endl; // debug

		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		std::cout << "---processMesh-HERE-3.1--" << std::endl; // debug

		// 1. diffuse maps
		std::vector<UntitledEngine::GLTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE,
		                                                                          "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::cout << "---processMesh-HERE-3.2--" << std::endl; // debug

		// 2. specular maps
		std::vector<UntitledEngine::GLTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR,
		                                                                           "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::cout << "---processMesh-HERE-3.3--" << std::endl; // debug

		// 3. normal maps
		std::vector<UntitledEngine::GLTexture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS,
		                                                                         "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::cout << "---processMesh-HERE-3.4--" << std::endl; // debug

		// 4. height maps
		std::vector<UntitledEngine::GLTexture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT,
		                                                                         "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		std::cout << "---processMesh-END--" << std::endl; // debug

		// return a mesh object created from the extracted mesh data
		return Mesh(vertices, indices, textures);
	}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
	std::vector<UntitledEngine::GLTexture>
	Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
		std::cout << "---processMesh-loadMsterialTextures-START--" << std::endl; // debug

		std::vector<UntitledEngine::GLTexture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
			std::cout << "---processMesh-loadMsterialTextures-HERE-1--" << std::endl; // debug

			aiString str;
			UntitledEngine::GLTexture texture;

			std::cout << "---processMesh-loadMsterialTextures-HERE-2--" << std::endl; // debug

			// Get texture filepath
			mat->GetTexture(type, i, &str);

			std::cout << "---processMesh-loadMsterialTextures-HERE-2.1--" << std::endl; // debug

			std::string path = str.data;
			path = "../Textures/" + path;

			std::cout << "---processMesh-loadMsterialTextures-HERE-2.2--\npath: '" << path.c_str() << "'"
			          << std::endl; // debug

			path = realpath(path.c_str(), nullptr);

			std::cout << "---processMesh-loadMsterialTextures-HERE-3--\npath: '" << path.c_str() << "'"
			          << std::endl; // debug

			// Load texture from file/texture-cache
			texture = UntitledEngine::ResourceManager::getTexture(path);

			std::cout << "---processMesh-loadMsterialTextures-HERE-4--" << std::endl; // debug

			// Add texture to list
			texture.type = typeName;
			textures.push_back(texture);
			std::cout << "---processMesh-loadMsterialTextures-HERE-5--" << std::endl; // debug
		}

		std::cout << "---processMesh-loadMsterialTextures-END--" << std::endl; // debug

		return textures;
	}
}