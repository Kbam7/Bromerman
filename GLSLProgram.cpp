#include "GLSLProgram.hpp"
#include "Errors.h"

GLSLProgram::GLSLProgram() : _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
{
}

GLSLProgram::~GLSLProgram(){}

void    GLSLProgram::compileShaders(const std::string& vertexShaderFilePath,const std::string& fragmentShaderFilePath)
{
    _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    if (_vertexShaderID)
    {
        fatalError("Vertex shader failed to be completed!");
    }

    _fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    if (_fragmenthaderID)
    {
        fatalError("fragment shader failed to be completed!");
    }
}

void    GLSLProgram::linkShaders()
{

}