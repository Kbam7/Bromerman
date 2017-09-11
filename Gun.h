#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
//#include "UntitledEngine/include/AudioEngine.h"

#include "Bullet.h"

class Gun
{
public:
    Gun(std::string name, int fireRate, int bulletsPerShot, 
        float spread, float bulletDamage, float bulletSpeed/*, UntitledEngine::SoundEffect fireEffect*/);
    ~Gun();

    void update(bool isMouseDown, const glm::vec3& position, const glm::vec3& direction, std::vector<Bullet>& bullets, float deltaTime);

private:

    //UntitledEngine::SoundEffect m_fireEffect;

    void fire(const glm::vec3& direction, const glm::vec3& position, std::vector<Bullet>& bullets);

    std::string _name;

    int _fireRate; ///< Fire rate in terms of frames

    int _bulletsPerShot; ///< How many bullets are fired at at time

    float _spread; ///< Accuracy

    float _bulletSpeed;

    int _bulletDamage;

    float _frameCounter; ///< Counts frames so we know when to shoot bullets

};
