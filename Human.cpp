#include "Human.h"
#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include "UntitledEngine/include/ResourceManager.h"

Human::Human() :
		_frames(0) {
}


Human::~Human() {
}

void Human::init(float speed, glm::vec3 pos) {

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	_health = 20;

	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
	_color.a = 255;

	_speed = speed;
	_position = pos;
	// Get random direction
	m_direction = glm::vec3(randDir(randomEngine), randDir(randomEngine), 0.0f);
	// Make sure direction isn't zero
	if (m_direction.length() == 0) m_direction = glm::vec3(1.0f, 0.0f, 0.0f);

	m_direction = glm::normalize(m_direction);

	m_textureID = UntitledEngine::ResourceManager::getTexture("../Textures/human.png").id;
}

void Human::update(const std::vector<std::string> &levelData,
                   std::vector<Human *> &humans,
                   std::vector<Zombie *> &zombies,
                   float deltaTime) {

	(void) humans;
	(void) zombies;

	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-40.0f, 40.0f);

	_position += m_direction * _speed * deltaTime;

	// Randomly change direction every 20 frames
	if (_frames == 20) {
		m_direction = glm::rotate(m_direction, randRotate(randomEngine), glm::vec3(0, 0, 1));
		_frames = 0;
	} else {
		_frames++;
	}

	if (collideWithLevel(levelData)) {
		m_direction = glm::rotate(m_direction, randRotate(randomEngine), glm::vec3(0, 0, 1));
	}
}