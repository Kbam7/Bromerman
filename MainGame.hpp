#ifndef MAINGAME_HPP
# define MAINGAME_HPP

#include <SDL2/SDL.h>

#include <OpenGL/GL.h>
enum class GameState {PLAY, EXIT};


class MainGame
{
public: 

    MainGame();
    ~MainGame();

    void run();
    
private:
    void initSystems();
    void processInput();
    void gameLoop();
    void drawGame();

    SDL_Window* _window;
    int         _screenWidth;
    int         _screenHeight;
    GameState   _gameState;
};

#endif