#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "UntitledEngine/include/SpriteBatch.h"

class Human;
class Zombie;
class Agent;

const int BULLET_RADIUS = 5;

class Bullet
{
public:
    Bullet(glm::vec3 position, glm::vec3 direction, float damage, float speed);
    ~Bullet();

    // When update returns true, delete bullet
    bool update(const std::vector<std::string>& levelData, float deltaTime);

    void draw(UntitledEngine::SpriteBatch& spriteBatch);

    bool collideWithAgent(Agent* agent);

    float getDamage() const { return _damage; }

    glm::vec2 getPosition() const { return _position; }

private:

    bool collideWithWorld(const std::vector<std::string>& levelData);

    glm::vec3 _position;
    glm::vec3 _direction;
    float _damage;
    float _speed;
};
