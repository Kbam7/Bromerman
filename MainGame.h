#pragma once

#include "UntitledEngine/include/Window.h"
#include "UntitledEngine/include/GLSLProgram.h"
#include "UntitledEngine/include/Camera2D.h"
#include "UntitledEngine/include/InputManager.h"
#include "UntitledEngine/include/SpriteBatch.h"
//#include "UntitledEngine/include/SpriteFont.h"
//#include "UntitledEngine/include/AudioEngine.h"
#include "UntitledEngine/include/ParticleEngine2D.h"
#include "UntitledEngine/include/ParticleBatch2D.h"

#include "Player.h"
#include "Level.h"
#include "Bullet.h"

#include <GLFW/glfw3.h>

class Zombie;

// Input callbacks
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

enum class GameState {
	PLAY,
	EXIT
};

class MainGame {
public:
	MainGame();

	~MainGame();

	/// Runs the game
	void run();

private:
	/// Initializes the core systems
	void initSystems();

	/// Initializes the level and sets up everything
	void initLevel();

	/// Initializes the shaders
	void initShaders();

	/// Main game loop for the program
	void gameLoop();

	/// Updates all agents
	void updateAgents(float deltaTime);

	/// Updates all bullets
	void updateBullets(float deltaTim);

	/// Checks the victory condition
	void checkVictory();

	/// Handles input processing
	void processInput();

	/// Renders the game
	void drawGame();

	/// Draws the HUD
	void drawHud();

	/// Adds blood to the particle engine
	void addBlood(const glm::vec2 &position, int numParticles);

	/// Member Variables
	UntitledEngine::Window m_window; ///< The game window

	UntitledEngine::GLSLProgram m_textureProgram; ///< The shader program

	UntitledEngine::InputManager m_inputManager; ///< Handles input

	UntitledEngine::Camera2D m_camera; ///< Main Camera
	UntitledEngine::Camera2D m_hudCamera; ///< Hud Camera

	UntitledEngine::SpriteBatch m_agentSpriteBatch; ///< Draws all agents
	UntitledEngine::SpriteBatch m_hudSpriteBatch;

	UntitledEngine::ParticleEngine2D m_particleEngine;
	UntitledEngine::ParticleBatch2D *m_bloodParticleBatch;

	std::vector<Level *> m_levels; ///< vector of all levels

	int m_screenWidth = 1024;
	int m_screenHeight = 768;
	float m_fps;
	int m_currentLevel;

	Player *m_player;
	std::vector<Human *> m_humans; ///< Vector of all humans
	std::vector<Zombie *> m_zombies; ///< Vector of all zombies
	std::vector<Bullet> m_bullets;

	int m_numHumansKilled; ///< Humans killed by player
	int m_numZombiesKilled; ///< Zombies killed by player

	//UntitledEngine::SpriteFont*    m_spriteFont;

//    UntitledEngine::AudioEngine m_audioEngine;

	GameState m_gameState;
};

