#ifndef MAINGAME_HPP
# define MAINGAME_HPP

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <OpenGL/GL.h>
#include "Sprite.hpp"
#include "GLSLProgram.hpp"

enum class GameState {PLAY, EXIT};


class MainGame
{
public: 

    MainGame();
    ~MainGame();

    void run();
    
private:
    void initSystems();
    void initShaders();
    void processInput();
    void gameLoop();
    void drawGame();

    SDL_Window* _window;
    int         _screenWidth;
    int         _screenHeight;
    GameState   _gameState;
    
    Sprite  _sprite;
    GLSLProgram _colorProgram;
};

#endif