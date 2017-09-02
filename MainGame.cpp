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

#include "Gun.h"
#include "Zombie.h"

const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float PLAYER_SPEED = 5.0f;

MainGame::MainGame() :
		m_screenWidth(1024),
		m_screenHeight(768),
		m_fps(0),
		m_player(nullptr),
		m_numHumansKilled(0),
		m_numZombiesKilled(0),
		m_gameState(GameState::PLAY) {
	// Empty

}

MainGame::~MainGame() {
	// Don't forget to delete the levels!
	for (size_t i = 0; i < m_levels.size(); i++) {
		delete m_levels[i];
	}
	// Don't forget to delete the humans and zombies!
	for (size_t i = 0; i < m_humans.size(); i++) {
		delete m_humans[i];
	}
	for (size_t i = 0; i < m_zombies.size(); i++) {
		delete m_zombies[i];
	}
}

void MainGame::run() {

	initSystems();

	initLevel();

	//UntitledEngine::Music music = m_audioEngine.loadMusic("../Sound/XYZ.ogg");
	//music.play(-1);

	gameLoop();
}

void MainGame::initSystems() {
	// Initialize the game engine
	UntitledEngine::init();

	// Initialize sound, must happen after UntitledEngine::init
	//m_audioEngine.init();

	// Create our window
	m_window.create("ZombieGame", m_screenWidth, m_screenHeight, 0);
	glfwSetMouseButtonCallback(m_window.getWindow(), mouse_button_callback);

	// Grey background color
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	// Set up the shaders
	initShaders();

	// Initialize our spritebatch
	m_agentSpriteBatch.init();
	m_hudSpriteBatch.init();

	// Initialize sprite font
	//m_spriteFont = new UntitledEngine::SpriteFont("../Fonts/chintzy.ttf", 64);

	// Set up the camera
	m_camera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.setPosition(glm::vec2(m_screenWidth / 2, m_screenHeight / 2));

	// Initialize particles
	m_bloodParticleBatch = new UntitledEngine::ParticleBatch2D;

	// Initialize the particle batch and use a lambda function to define the update
	m_bloodParticleBatch->init(1000, 0.05f,
	                           UntitledEngine::ResourceManager::getTexture("../Textures/particle.png"),
	                           [](UntitledEngine::Particle2D &particle, float deltaTime) {
		                           particle.position += particle.velocity * deltaTime;
		                           particle.color.a = (GLubyte) (particle.life * 255.0f);
	                           });

	m_particleEngine.addParticleBatch(m_bloodParticleBatch);

}

void MainGame::initLevel() {
	// Level 1
	m_levels.push_back(new Level("../Levels/level1.txt"));
	m_currentLevel = 0;

	m_player = new Player();
	m_player->init(PLAYER_SPEED, m_levels[m_currentLevel]->getStartPlayerPos(), &m_inputManager, &m_camera, &m_bullets);

	m_humans.push_back(m_player);

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));

	std::uniform_int_distribution<int> randX(2, m_levels[m_currentLevel]->getWidth() - 2);
	std::uniform_int_distribution<int> randY(2, m_levels[m_currentLevel]->getHeight() - 2);

	// Add all the random humans
	for (int i = 0; i < m_levels[m_currentLevel]->getNumHumans(); i++) {
		m_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		m_humans.back()->init(HUMAN_SPEED, pos);
	}

	// Add the zombies
	const std::vector<glm::vec2> &zombiePositions = m_levels[m_currentLevel]->getZombieStartPositions();
	for (size_t i = 0; i < zombiePositions.size(); i++) {
		m_zombies.push_back(new Zombie);
		m_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}

	// Set up the players guns
	const float BULLET_SPEED = 20.0f;
	m_player->addGun(new Gun("Magnum", 10, 1, 0.1f, 30,
	                         BULLET_SPEED/*, m_audioEngine.loadSoundEffect("../Sound/shots/pistol.wav")*/));
	m_player->addGun(new Gun("Shotgun", 60, 12, 0.8f, 4,
	                         BULLET_SPEED/*, m_audioEngine.loadSoundEffect("../Sound/shots/shotgun.wav")*/));
	m_player->addGun(
			new Gun("MP5", 2, 1, 0.4f, 20, BULLET_SPEED/*, m_audioEngine.loadSoundEffect("../Sound/shots/cg1.wav")*/));
	m_player->addGun(
			new Gun("BOMB", 80, 1, 0.0f, 50, 0/*, m_audioEngine.loadSoundEffect("../Sound/shots/shotgun.wav")*/));
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
	const int MAX_PHYSICS_STEPS = 6; // Max number of physics steps per frame
	const float MS_PER_SECOND = 1000; // Number of milliseconds in a second
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS; // The desired frame time per frame
	const float MAX_DELTA_TIME = 1.0f; // Maximum size of deltaTime
	const float ONE_NANOSEC = 1000000000; // One nano second
	const float GAME_SPEED = ((ONE_NANOSEC / DESIRED_FPS) / 100) * 100;

	// Used to cap the FPS
	UntitledEngine::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(DESIRED_FPS);

	// Zoom out the camera by 3x
	const float CAMERA_SCALE = 1.0f / 3.0f;
	m_camera.setScale(CAMERA_SCALE);

	float previousTicks = clock();

	// Main loop
	while (m_gameState == GameState::PLAY) {
		fpsLimiter.begin();

		// Calculate the frameTime in milliseconds
		float newTicks = clock();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks; // Store newTicks in previousTicks so we can use it next frame
		// Get the total delta time
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		checkVictory();

		m_inputManager.update();

		processInput();

		int i = 0; // This counter makes sure we don't spiral to death!
		// Loop while we still have steps to process.
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
			// The deltaTime should be the the smaller of the totalDeltaTime and MAX_DELTA_TIME
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			// Update all physics here and pass in deltaTime
			updateAgents(deltaTime);
			updateBullets(deltaTime);
			m_particleEngine.update(deltaTime);
			// Since we just took a step that is length deltaTime, subtract from totalDeltaTime
			totalDeltaTime -= deltaTime;
			// Increment our frame counter so we can limit steps to MAX_PHYSICS_STEPS
			i++;
		}

		// Make sure the camera is bound to the player position
		m_camera.setPosition(m_player->getPosition());
		m_camera.update();
		m_hudCamera.update();

		drawGame();

		// End the frame, limit the FPS, and get the current FPS.
		m_fps = fpsLimiter.end();
		std::cout << m_fps << std::endl;
	}
}

void MainGame::updateAgents(float deltaTime) {
	// Update all humans
	for (size_t i = 0; i < m_humans.size(); i++) {
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
			fatalError("YOU LOSE");
		}
	}

	// Update Human collisions
	for (size_t i = 0; i < m_humans.size(); i++) {
		// Collide with other humans
		for (size_t j = i + 1; j < m_humans.size(); j++) {
			m_humans[i]->collideWithAgent(m_humans[j]);
		}
	}

	// Dont forget to update zombies
}

void MainGame::updateBullets(float deltaTime) {
	// Update and collide with world
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
	}
}

void MainGame::checkVictory() {
	// TODO: Support for multiple levels!
	// _currentLevel++; initLevel(...);

	// If all zombies are dead we win!
	if (m_zombies.empty()) {
		// Print victory message
		std::printf("*** You win! ***\n You killed %d humans and %d zombies. There are %zu/%d civilians remaining",
		            m_numHumansKilled, m_numZombiesKilled, m_humans.size() - 1,
		            m_levels[m_currentLevel]->getNumHumans());

		// Easy way to end the game :P
		fatalError("");
	}
}

// Key callback, set on initialisation
void MainGame::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	(void)window;
	(void)scancode;
	(void)mods;

	switch (action) {
		case GLFW_PRESS:
		case GLFW_REPEAT:
			m_inputManager.pressKey(key);
			break;
		case GLFW_RELEASE:
		default:
			m_inputManager.releaseKey(key);
			break;
	}
}

void MainGame::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	m_inputManager.setMouseCoords(xpos, ypos);
}

void MainGame::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	(void)window;
	(void)mods;

	switch (action) {
		case GLFW_PRESS:
		case GLFW_REPEAT:
			m_inputManager.pressKey(button);
			break;
		case GLFW_RELEASE:
		default:
			m_inputManager.releaseKey(button);
			break;

	}
}

void MainGame::processInput() {
	if (m_inputManager.isKeyDown(GLFW_KEY_Q) || m_inputManager.wasKeyDown(GLFW_KEY_Q))
		m_gameState = GameState::EXIT;
}

void MainGame::drawGame() {
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_textureProgram.use();

	// Draw code goes here
	glActiveTexture(GL_TEXTURE0);

	// Make sure the shader uses texture 0
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Grab the camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Draw the level
	m_levels[m_currentLevel]->draw();

	// Begin drawing agents
	m_agentSpriteBatch.begin();

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

	// Unbind the program
	m_textureProgram.unuse();

	// Swap our buffer and draw everything to the screen!
	m_window.swapBuffer();
}

void MainGame::drawHud() {
	char buffer[256];

	glm::mat4 projectionMatrix = m_hudCamera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_hudSpriteBatch.begin();

	std::sprintf(buffer, "Num Humans %zu", m_humans.size());
	/*m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 0),
					  glm::vec2(0.5), 0.0f, UntitledEngine::ColorRGBA8(255, 255, 255, 255));*/

	std::sprintf(buffer, "Num Zombies %zu", m_zombies.size());
	/*m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 36),
					  glm::vec2(0.5), 0.0f, UntitledEngine::ColorRGBA8(255, 255, 255, 255));*/

	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();
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