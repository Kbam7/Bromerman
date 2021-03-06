#include "MainGame.hpp"
#include "Errors.hpp"
#include <iostream>
#include <cstring>


MainGame::MainGame()
{
    _window = nullptr;
    _screenWidth = 500; 
    _screenHeight = 500;
    _gameState = GameState::PLAY;
}

MainGame::~MainGame()
{

}

void MainGame::run(){
    initSystems();

    _sprite.init(-1.0f, -1.0f , 1.0f , 1.0f);

    gameLoop();
}

void MainGame::initSystems(){
    // initialise SDL

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_Init(SDL_INIT_EVERYTHING);
    _window = SDL_CreateWindow("Bomberman engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
    if (_window == nullptr)
    {
        fatalError("SDL Window could not be created");
    }

    SDL_GLContext context = SDL_GL_CreateContext(_window);
    if (context == nullptr)
    {
        fatalError("SDL_GL context could not be created");
    }
    GLenum error = glewInit();
    if (error != GLEW_OK)
   {
       fatalError("Could not initialise GLEW");
   }
   
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

   glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

   initShaders();
}

void    MainGame::initShaders() 
{
    _colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
    _colorProgram.addAttribute("vertexPosition");
    _colorProgram.linkShaders();
}

void MainGame::gameLoop()
{
    while (_gameState != GameState::EXIT)
    {
        processInput();
        drawGame();
    }
} 

void MainGame::processInput()
{
    SDL_Event evnt;

    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                _gameState = GameState::EXIT;
            case SDL_MOUSEMOTION:
                std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
            break;
        }
    }
}

void MainGame::drawGame(){

    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _colorProgram.use();

    _sprite.draw();

    _colorProgram.unuse();

    SDL_GL_SwapWindow(_window);

}
