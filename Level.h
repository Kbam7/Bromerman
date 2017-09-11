#pragma once

#include <string>
#include <vector>

#include "UntitledEngine/include/SpriteBatch.h"

const int TILE_WIDTH = 64;

class Level
{
public:
    // Load the level
    Level(const std::string& fileName);
    ~Level();

    void draw();

    // Getters
    int getWidth() const { return _levelData[0].size(); }
    int getHeight() const { return _levelData.size(); }
    int getNumHumans() const { return _numHumans; }
    const std::vector<std::string>& getLevelData() const { return _levelData; }
    glm::vec3 getStartPlayerPos() const { return _startPlayerPos; }
    const std::vector<glm::vec3>& getZombieStartPositions() const { return _zombieStartPositions; }

private:
    std::vector<std::string> _levelData;
    int _numHumans;
    UntitledEngine::SpriteBatch _spriteBatch;

    glm::vec3 _startPlayerPos;
    std::vector<glm::vec3> _zombieStartPositions;
};
