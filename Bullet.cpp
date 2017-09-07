#include "Bullet.h"
#include "UntitledEngine/include/ResourceManager.h"

#include "Agent.h"
#include "Human.h"
#include "Zombie.h"
#include "Level.h"

Bullet::Bullet(glm::vec3 position, glm::vec3 direction, float damage, float speed) :
    _position(position),
    _direction(direction),
    _damage(damage),
    _speed(speed) {
    // Empty
}


Bullet::~Bullet() {
    // Empty
}


bool Bullet::update(const std::vector<std::string>& levelData, float deltaTime) {
    _position += _direction * _speed * deltaTime;
    return collideWithWorld(levelData);
}

void Bullet::draw(UntitledEngine::SpriteBatch& spriteBatch) {
    glm::vec4 destRect(_position.x,
                       _position.y,
                       BULLET_RADIUS * 2,
                       BULLET_RADIUS * 2);
    const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

    UntitledEngine::ColorRGBA8 color;
    color.r = 75;
    color.g = 75;
    color.b = 75;
    color.a = 255;

    spriteBatch.draw(destRect, uvRect, UntitledEngine::ResourceManager::getTexture("../Textures/circle.png").id, 0.0f, color);
}

bool Bullet::collideWithAgent(Agent* agent) {
    const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

    glm::vec3 centerPosA = _position;
    glm::vec3 centerPosB = agent->getPosition() + glm::vec3(AGENT_RADIUS, AGENT_RADIUS, 0.0f);

    glm::vec3 distVec = centerPosA - centerPosB;

    float distance = glm::length(distVec);

    float collisionDepth = MIN_DISTANCE - distance;
    if (collisionDepth > 0) { 
        return true;
    }
    return false;
}

bool Bullet::collideWithWorld(const std::vector<std::string>& levelData) {
    glm::ivec2 gridPosition;
    gridPosition.x = floor(_position.x / (float)TILE_WIDTH);
    gridPosition.y = floor(_position.y / (float)TILE_WIDTH);

    // If we are outside the world, just return
    if (gridPosition.x < 0 || gridPosition.x >= (int)levelData[0].size() ||
        gridPosition.y < 0 || gridPosition.y >= (int)levelData.size()) {
        return true;
    }

    return (levelData[gridPosition.y][gridPosition.x] != '.');
}