#pragma once

#include "Human.h"
#include "UntitledEngine/include/InputManager.h"
#include "UntitledEngine/include/Camera3D.h"
#include "Bullet.h"

class Gun;

class Player : public Human
{
public:
    Player();
    ~Player();

    void init(float speed, glm::vec3 pos, UntitledEngine::InputManager* inputManager, UntitledEngine::Camera3D* camera, std::vector<Bullet>* bullets);

    void addGun(Gun* gun);

    void update(const std::vector<std::string>& levelData,
                std::vector<Human*>& humans,
                std::vector<Zombie*>& zombies,
                float deltaTime) override;
private:
    UntitledEngine::InputManager* _inputManager;

    std::vector<Gun*> _guns;
    int _currentGunIndex;

    UntitledEngine::Camera3D* _camera;
    std::vector<Bullet>* _bullets;

};

