#pragma once

#include <string>
#include <GL/glew.h>

namespace UntitledEngine {

	//This class handles the compilation, linking, and usage of a GLSL shader program.
	//Reference: http://www.opengl.org/wiki/Shader_Compilation
	class GLSLProgram {
	public:
		GLSLProgram();

		~GLSLProgram();

		void compileShaders(const std::string &vertexShaderFilePath, const std::string &fragmentShaderFilePath);

		void compileShadersFromSource(const char *vertexSource, const char *fragmentSource);

		void linkShaders();

		void addAttribute(const std::string &attributeName);

		GLint getUniformLocation(const std::string &uniformName);

		void use();

		void unuse();

		void dispose();

		int get_numAttributes() const;

		GLuint get_programID() const;

	private:
		void compileShader(const char *source, const std::string &name, GLuint id);

		GLuint _programID;
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
		int _numAttributes;
	};

}
