#include "MainGame.h"

#include "UntitledEngine/include/UntitledEngine.h"
#include "UntitledEngine/include/Timing.h"
#include "UntitledEngine/include/UntitledEngineErrors.h"
#include "UntitledEngine/include/ResourceManager.h"
#include <random>
#include <ctime>
#include <algorithm>

#include <iostream>
#include <glm/gtx/rotate_vector.hpp>
#include <GLFW/glfw3.h>

//#include "Gun.h"
//#include "Zombie.h"

//const float HUMAN_SPEED = 1.0f;
//const float ZOMBIE_SPEED = 1.3f;
const float PLAYER_SPEED = 5.0f;

// Key callback, set on initialisation
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	(void) scancode;
	(void) mods;
	UntitledEngine::InputManager *inputManager;

	inputManager = static_cast<UntitledEngine::InputManager *>(glfwGetWindowUserPointer(window));
	switch (action) {
		case GLFW_PRESS:
		case GLFW_REPEAT:
			inputManager->pressKey(key);
			break;
		case GLFW_RELEASE:
		default:
			inputManager->releaseKey(key);
			break;
	}
}

// Cursor Position callback, set on initialisation
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
	UntitledEngine::InputManager *inputManager;

	inputManager = static_cast<UntitledEngine::InputManager *>(glfwGetWindowUserPointer(window));
	inputManager->setMouseCoords(xpos, ypos);
}

// Mouse Button callback, set on initialisation
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	(void) mods;
	UntitledEngine::InputManager *inputManager;

	inputManager = static_cast<UntitledEngine::InputManager *>(glfwGetWindowUserPointer(window));
	switch (action) {
		case GLFW_PRESS:
		case GLFW_REPEAT:
			inputManager->pressKey(button);
			break;
		case GLFW_RELEASE:
		default:
			inputManager->releaseKey(button);
			break;

	}
}

MainGame::MainGame() :
		m_screenWidth(1024),
		m_screenHeight(768),
		m_fps(0),
		//m_player(nullptr),
		//m_numHumansKilled(0),
		//m_numZombiesKilled(0),
		m_gameState(GameState::PLAY) {
	// Empty

}

MainGame::~MainGame() {
	// Don't forget to delete the levels!
	for (size_t i = 0; i < m_levels.size(); i++) {
		delete m_levels[i];
	}
/*	// Don't forget to delete the humans and zombies!
	for (size_t i = 0; i < m_humans.size(); i++) {
		delete m_humans[i];
	}
	for (size_t i = 0; i < m_zombies.size(); i++) {
		delete m_zombies[i];
	}*/
}

void MainGame::run() {

	initSystems();

	//initLevel();

	//UntitledEngine::Music music = m_audioEngine.loadMusic("../Sound/XYZ.ogg");
	//music.play(-1);

	// Create new object
	std::vector<std::string> shader_attribs; // empty. Attributes defined in vertex shader
	m_model = new Model("../Objects/drone_lvl_2.obj", &m_camera);
	m_model->initShaders("../Shaders/walls/Shader.vert", "../Shaders/walls/Shader.frag", shader_attribs);

	gameLoop();
}

void MainGame::initSystems() {
	// Initialize the game engine
	UntitledEngine::init();

	// Create our window
	m_window.create("Bromerman", m_screenWidth, m_screenHeight, 0);

	// Set window data for callback functions
	glfwSetWindowUserPointer(m_window.getWindow(), &m_inputManager);
	// Set callbacks
	glfwSetKeyCallback(m_window.getWindow(), &key_callback);
	glfwSetCursorPosCallback(m_window.getWindow(), &cursor_position_callback);
	glfwSetMouseButtonCallback(m_window.getWindow(), &mouse_button_callback);

	// Grey background color
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Set up the shaders
	//initShaders();

	// Set up the camera
	m_camera.init(m_screenWidth, m_screenHeight);

}

void MainGame::initLevel() {
	// Level 1
	m_levels.push_back(new Level("../Levels/level1.txt"));
	m_currentLevel = 0;

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));

	std::uniform_int_distribution<int> randX(2, m_levels[m_currentLevel]->getWidth() - 2);
	std::uniform_int_distribution<int> randY(2, m_levels[m_currentLevel]->getHeight() - 2);
}

void MainGame::initShaders() {
	// Compile our color shader
	m_textureProgram.compileShaders("../Shaders/textureShading.vert", "../Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
}

void MainGame::gameLoop() {

	// Some helpful constants.
	const float DESIRED_FPS = 60.0f; // FPS the game is designed to run at
	const int   MAX_PHYSICS_STEPS = 6; // Max number of physics steps per frame
	const float UNITS_PER_SECOND = 1000;  // Number of units in a second
	const float DESIRED_FRAMETIME = UNITS_PER_SECOND / DESIRED_FPS; // The desired frame time per frame
	const float MAX_DELTA_TIME = 1.0f; // Maximum size of deltaTime

	// Used to cap the FPS
	UntitledEngine::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(DESIRED_FPS);
	float previousTicks = UntitledEngine::getGameTicks();

	// Zoom out the camera by 3x
	const float CAMERA_SCALE = 1.0f /*/ 3.0f*/;
	m_camera.setScale(CAMERA_SCALE);

	// Main loop
	while (m_gameState == GameState::PLAY) {
		fpsLimiter.begin();

		// Calculate the frameTime in milliseconds
		float newTicks = UntitledEngine::getGameTicks();
		float frameTime = ((newTicks - previousTicks));

		previousTicks = newTicks; // Store newTicks in previousTicks so we can use it next frame
		// Get the total delta time
		float totalDeltaTime = (frameTime / DESIRED_FRAMETIME);

		//checkVictory();

		m_inputManager.update();

		processInput();

		int i = 0; // This counter makes sure we don't spiral to death!
		// Loop while we still have steps to process.
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
			// The deltaTime should be the the smaller of the totalDeltaTime and MAX_DELTA_TIME
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

			// Update all physics here and pass in deltaTime
			//updateAgents(deltaTime);
			//updateBullets(deltaTime);
			//m_particleEngine.update(deltaTime);
			// Since we just took a step that is length deltaTime, subtract from totalDeltaTime
			totalDeltaTime -= deltaTime;
			// Increment our frame counter so we can limit steps to MAX_PHYSICS_STEPS
			i++;
		}

		// Make sure the camera is bound to the player position
		//m_camera.setPosition(m_player->getPosition());
		m_camera.update();
		//m_hudCamera.update();

		drawGame();

		// End the frame, limit the FPS, and get the current FPS.
		m_fps = fpsLimiter.end();
		std::cout << m_fps << std::endl;
	}
}

void MainGame::updateAgents(float deltaTime) {
	(void)deltaTime;
	// Update all humans
/*	for (size_t i = 0; i < m_humans.size(); i++) {
		m_humans[i]->update(m_levels[m_currentLevel]->getLevelData(),
		                    m_humans,
		                    m_zombies,
		                    deltaTime);
	}

	// Update all zombies
	for (size_t i = 0; i < m_zombies.size(); i++) {
		m_zombies[i]->update(m_levels[m_currentLevel]->getLevelData(),
		                     m_humans,
		                     m_zombies,
		                     deltaTime);
	}

	// Update Zombie collisions
	for (size_t i = 0; i < m_zombies.size(); i++) {
		// Collide with other zombies
		for (size_t j = i + 1; j < m_zombies.size(); j++) {
			m_zombies[i]->collideWithAgent(m_zombies[j]);
		}
		// Collide with humans
		for (size_t j = 1; j < m_humans.size(); j++) {
			if (m_zombies[i]->collideWithAgent(m_humans[j])) {
				// Add the new zombie
				m_zombies.push_back(new Zombie);
				m_zombies.back()->init(ZOMBIE_SPEED, m_humans[j]->getPosition());
				// Delete the human
				delete m_humans[j];
				m_humans[j] = m_humans.back();
				m_humans.pop_back();
			}
		}

		// Collide with player
		if (m_zombies[i]->collideWithAgent(m_player)) {
			UntitledEngine::fatalError("YOU LOSE");
		}
	}

	// Update Human collisions
	for (size_t i = 0; i < m_humans.size(); i++) {
		// Collide with other humans
		for (size_t j = i + 1; j < m_humans.size(); j++) {
			m_humans[i]->collideWithAgent(m_humans[j]);
		}
	}*/

	// Dont forget to update zombies
}

void MainGame::updateBullets(float deltaTime) {
	(void)deltaTime;
/*	// Update and collide with world
	for (size_t i = 0; i < m_bullets.size();) {
		// If update returns true, the bullet collided with a wall
		if (m_bullets[i].update(m_levels[m_currentLevel]->getLevelData(), deltaTime)) {
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		} else {
			i++;
		}
	}

	bool wasBulletRemoved;

	// Collide with humans and zombies
	for (size_t i = 0; i < m_bullets.size(); i++) {
		wasBulletRemoved = false;
		// Loop through zombies
		for (size_t j = 0; j < m_zombies.size();) {
			// Check collision
			if (m_bullets[i].collideWithAgent(m_zombies[j])) {
				// Add blood
				addBlood(m_bullets[i].getPosition(), 5);

				// Damage zombie, and kill it if its out of health
				if (m_zombies[j]->applyDamage(m_bullets[i].getDamage())) {
					// If the zombie died, remove him
					delete m_zombies[j];
					m_zombies[j] = m_zombies.back();
					m_zombies.pop_back();
					m_numZombiesKilled++;
				} else {
					j++;
				}

				// Remove the bullet
				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
				wasBulletRemoved = true;
				i--; // Make sure we don't skip a bullet
				// Since the bullet died, no need to loop through any more zombies
				break;
			} else {
				j++;
			}
		}
		// Loop through humans
		if (wasBulletRemoved == false) {
			for (size_t j = 1; j < m_humans.size();) {
				// Check collision
				if (m_bullets[i].collideWithAgent(m_humans[j])) {
					// Add blood
					addBlood(m_bullets[i].getPosition(), 5);
					// Damage human, and kill it if its out of health
					if (m_humans[j]->applyDamage(m_bullets[i].getDamage())) {
						// If the human died, remove him
						delete m_humans[j];
						m_humans[j] = m_humans.back();
						m_humans.pop_back();
					} else {
						j++;
					}

					// Remove the bullet
					m_bullets[i] = m_bullets.back();
					m_bullets.pop_back();
					m_numHumansKilled++;
					i--; // Make sure we don't skip a bullet
					// Since the bullet died, no need to loop through any more zombies
					break;
				} else {
					j++;
				}
			}
		}
	}*/
}

void MainGame::checkVictory() {
	// TODO: Support for multiple levels!
	// _currentLevel++; initLevel(...);

	// If all zombies are dead we win!
/*	if (m_zombies.empty()) {
		// Print victory message
		std::printf("*** You win! ***\n You killed %d humans and %d zombies. There are %zu/%d civilians remaining",
		            m_numHumansKilled, m_numZombiesKilled, m_humans.size() - 1,
		            m_levels[m_currentLevel]->getNumHumans());

		// Easy way to end the game :P
		UntitledEngine::fatalError("");
	}*/
}

void MainGame::processInput() {
	glfwPollEvents();

	if (m_inputManager.isKeyDown(GLFW_KEY_Q) || m_inputManager.wasKeyDown(GLFW_KEY_Q)) {
		m_gameState = GameState::EXIT;
		return;
	}

	// y axis
	if (m_inputManager.isKeyDown(GLFW_KEY_W))
		m_camera.offsetPosition(glm::vec3(0, -0.1f * PLAYER_SPEED, 0));
	else if (m_inputManager.isKeyDown(GLFW_KEY_S))
		m_camera.offsetPosition(glm::vec3(0, 0.1f * PLAYER_SPEED, 0));

	// x axis
	if (m_inputManager.isKeyDown(GLFW_KEY_A))
		m_camera.offsetPosition(glm::vec3(0.1f * PLAYER_SPEED, 0, 0));
	else if (m_inputManager.isKeyDown(GLFW_KEY_D))
		m_camera.offsetPosition(glm::vec3(-0.1f * PLAYER_SPEED, 0, 0));

	// z axis
	if (m_inputManager.isKeyDown(GLFW_KEY_Z))
		m_camera.offsetPosition(glm::vec3(0, 0, 0.1f * PLAYER_SPEED));
	else if (m_inputManager.isKeyDown(GLFW_KEY_X))
		m_camera.offsetPosition(glm::vec3(0, 0, -0.1f * PLAYER_SPEED));

	// Zoom
/*	if (m_inputManager.isKeyDown(GLFW_KEY_Z))
		m_camera.offsetScale(0.005f);
	else if (m_inputManager.isKeyDown(GLFW_KEY_X))
		m_camera.offsetScale(-0.005f);*/
}

void MainGame::drawGame() {

	std::cout << "---drawGame() start---\n";

	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the level
	//m_levels[m_currentLevel]->draw();

	// Render basic object
	m_model->draw();

	// Begin drawing objects
	//m_textureProgram.use();

	// Begin drawing agents
	/*m_agentSpriteBatch.begin();

	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	// Draw the humans
	for (size_t i = 0; i < m_humans.size(); i++) {
		if (m_camera.isBoxInView(m_humans[i]->getPosition(), agentDims)) {
			m_humans[i]->draw(m_agentSpriteBatch);
		}
	}

	// Draw the zombies
	for (size_t i = 0; i < m_zombies.size(); i++) {
		if (m_camera.isBoxInView(m_zombies[i]->getPosition(), agentDims)) {
			m_zombies[i]->draw(m_agentSpriteBatch);
		}
	}

	// Draw the bullets
	for (size_t i = 0; i < m_bullets.size(); i++) {
		m_bullets[i].draw(m_agentSpriteBatch);
	}

	// End spritebatch creation
	m_agentSpriteBatch.end();

	// Render to the screen
	m_agentSpriteBatch.renderBatch();

	// Render the particles
	m_particleEngine.draw(&m_agentSpriteBatch);

	// Render the heads up display
	drawHud();
*/
	// Unbind the program
	//m_textureProgram.unuse();

	// Swap our buffer and draw everything to the screen!
	m_window.swapBuffer();

	std::cout << "---drawGame() end---\n";
}

void MainGame::drawHud() {
/*	char buffer[256];

	glm::mat4 projectionMatrix = m_hudCamera.getCameraMatrix();
	GLint mvpUniform = m_textureProgram.getUniformLocation("MVP");
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_hudSpriteBatch.begin();

	std::sprintf(buffer, "Num Humans %zu", m_humans.size());
	*//*m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 0),
					  glm::vec2(0.5), 0.0f, UntitledEngine::ColorRGBA8(255, 255, 255, 255));*//*

	std::sprintf(buffer, "Num Zombies %zu", m_zombies.size());
	*//*m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 36),
					  glm::vec2(0.5), 0.0f, UntitledEngine::ColorRGBA8(255, 255, 255, 255));*//*

	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();*/
}

void MainGame::addBlood(const glm::vec2 &position, int numParticles) {

	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);

	glm::vec2 vel(2.0f, 0.0f);
	UntitledEngine::ColorRGBA8 col(255, 0, 0, 255);

	for (int i = 0; i < numParticles; i++) {
		m_bloodParticleBatch->addParticle(position, glm::rotate(vel, randAngle(randEngine)), col, 30.0f);
	}
}