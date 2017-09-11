#include "Zombie.h"
#include "UntitledEngine/include/ResourceManager.h"

#include "Human.h"

Zombie::Zombie()
{
}

Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec3 pos) {
    _speed = speed;
    _position = pos;
    _health = 50;
    // Set Green Color
    _color = UntitledEngine::ColorRGBA8(255, 255, 255, 255);
    m_textureID = UntitledEngine::ResourceManager::getTexture("../Textures/zombie.png").id;
}

void Zombie::update(const std::vector<std::string>& levelData,
                    std::vector<Human*>& humans,
                    std::vector<Zombie*>& zombies,
                    float deltaTime) {

    (void)zombies;

    // Find the closest human
    Human* closestHuman = getNearestHuman(humans);

    // If we found a human, move towards him
    if (closestHuman != nullptr) {
        // Get the direction vector twoards the player
        m_direction = glm::normalize(closestHuman->getPosition() - _position);
        _position += m_direction * _speed * deltaTime;
    }

    // Do collision
    collideWithLevel(levelData);
}

Human* Zombie::getNearestHuman(std::vector<Human*>& humans) {
    Human* closestHuman = nullptr;
    float smallestDistance = 9999999.0f;

    for (size_t i = 0; i < humans.size(); i++) {
        // Get the direction vector
        glm::vec3 distVec = humans[i]->getPosition() - _position;
        // Get distance
        float distance = glm::length(distVec);

        // If this person is closer than the closest person, this is the new closest
        if (distance < smallestDistance) {
            smallestDistance = distance;
            closestHuman = humans[i];
        }
    }
    
    return closestHuman;
}