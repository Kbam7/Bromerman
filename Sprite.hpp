#ifndef SPRITE_HPP
# define SPRITE_HPP

#include "GL/glew.h"

class Sprite 
{
public:
    Sprite();
    ~Sprite();

    void    init(float x, float y, float widthl, float height);

    void    draw();

private:

    float     _x;
    float     _y;
    float     _width;
    float     _height;
    GLuint   _vboID;

};

#endif
