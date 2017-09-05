#pragma once

#include "../inc/Human.h"
#include "../UntitledEngine/include/InputManager.h"
#include "../UntitledEngine/include/Camera2D.h"
#include "../inc/Bullet.h"

class Gun;

class Player : public Human
{
public:
    Player();
    ~Player();

    void init(float speed, glm::vec2 pos, UntitledEngine::InputManager* inputManager, UntitledEngine::Camera2D* camera, std::vector<Bullet>* bullets);

    void addGun(Gun* gun);

    void update(const std::vector<std::string>& levelData,
                std::vector<Human*>& humans,
                std::vector<Zombie*>& zombies,
                float deltaTime) override;
private:
    UntitledEngine::InputManager* _inputManager;

    std::vector<Gun*> _guns;
    int _currentGunIndex;

    UntitledEngine::Camera2D* _camera;
    std::vector<Bullet>* _bullets;

};

