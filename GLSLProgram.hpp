#ifndef SPRITE_HPP
# define SPRITE_HPP

#include <string>
#include <GL/glew.h>

class GLSLProgram
{
public:
    GLSLProgram();
    ~GLSLProgram();

    void    compileShaders(const std::string& vertexShaderFilePath,const std::string& fragmentShaderFilePath);

    void    linkShaders();
private:
    GLuint _programID;

    GLuint _vertexShaderID;
    GLuint _fragmentShaderID;

    
}
#endif
