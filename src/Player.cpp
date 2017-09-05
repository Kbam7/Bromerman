#include "../inc/Player.h"
#include "../UntitledEngine/include/ResourceManager.h"

#include "../inc/Gun.h"

#include <iostream> // debug
#include <GLFW/glfw3.h>

Player::Player() :
    _currentGunIndex(-1) {
    // Empty
}

Player::~Player() {
    // Empty
}

void Player::init(float speed, glm::vec2 pos, UntitledEngine::InputManager* inputManager, UntitledEngine::Camera2D* camera, std::vector<Bullet>* bullets) {
    _speed = speed;
    _position = pos;
    _inputManager = inputManager;
    _bullets = bullets;
    _camera = camera;
    _color.r = 255;
    _color.g = 255;
    _color.b = 255;
    _color.a = 255;
    _health = 150;
    m_textureID = UntitledEngine::ResourceManager::getTexture("../Textures/player.png").id;
}

void Player::addGun(Gun* gun) {
    // Add the gun to his inventory
    _guns.push_back(gun);

    // If no gun equipped, equip gun.
    if (_currentGunIndex == -1) {
        _currentGunIndex = 0;
    }
}

void Player::update(const std::vector<std::string>& levelData,
                    std::vector<Human*>& humans,
                    std::vector<Zombie*>& zombies,
                    float deltaTime) {

    (void)humans;
    (void)zombies;

    if (_inputManager->isKeyDown(GLFW_KEY_W)) {
        _position.y += _speed * deltaTime;
    } else if (_inputManager->isKeyDown(GLFW_KEY_S)) {
        _position.y -= _speed * deltaTime;
    }
    if (_inputManager->isKeyDown(GLFW_KEY_A)) {
        _position.x -= _speed * deltaTime;
    } else if (_inputManager->isKeyDown(GLFW_KEY_D)) {
        _position.x += _speed * deltaTime;
    }

    if (_inputManager->isKeyDown(GLFW_KEY_1)) {
        _currentGunIndex = 0;
    } else if (_inputManager->isKeyDown(GLFW_KEY_2) && _guns.size() >= 1) {
        _currentGunIndex = 1;
    } else if (_inputManager->isKeyDown(GLFW_KEY_3) && _guns.size() >= 2) {
        _currentGunIndex = 2;
    } else if (_inputManager->isKeyDown(GLFW_KEY_4) && _guns.size() >= 3) {
        _currentGunIndex = 3;
    }

    glm::vec2 mouseCoords = _inputManager->getMouseCoords();
    mouseCoords = _camera->convertScreenToWorld(mouseCoords);


    glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);

    m_direction = glm::normalize(mouseCoords - centerPosition);

    //std::cout << "x: " << m_direction.x << " y: " << m_direction.y << " g: " << m_direction.g << " r: " << m_direction.r << " s: " << m_direction.s << " t: " << m_direction.t << std::endl;
    //m_direction = glm::vec2(-1, 0);
    if (_currentGunIndex != -1) {

        _guns[_currentGunIndex]->update(_inputManager->isKeyDown(GLFW_MOUSE_BUTTON_LEFT),
                                        centerPosition,
                                        m_direction,
                                        *_bullets,
                                        deltaTime);
                                        

    }

    collideWithLevel(levelData);
}
