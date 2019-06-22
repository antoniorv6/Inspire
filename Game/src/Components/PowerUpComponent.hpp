#pragma once

#include <glm.hpp>
#include <IComponent.h>
#include <Clock.hpp>
#include <vector>
#include <string>
#include <PICASSO.hpp>


class Entity;
class EntityManager;
class PhysicsManager;
class PositionManager;
class PowerUpManager;
class SoundManager;
class RenderManager;
class GenericData;
class PositionComponent;

enum PowerUpValues
{
    ATTACK_VALUE_CYBERATTACK = 3,
    TYPE_VALUE_CYBERATTACK = 1,

    ATTACK_VALUE_THIEF = 7,
    TYPE_VALUE_THIEF = 3,

    ATTACK_VALUE_SHIELD = 1,
    TYPE_VALUE_SHIELD = 1,

    ATTACK_VALUE_FREEZING = 9,
    TYPE_VALUE_FREEZING = 4,

    ATTACK_VALUE_REFLECTOR = 5,
    TYPE_VALUE_REFLECTOR = 2,
    
    ATTACK_VALUE_MAGNET = 8,
    TYPE_VALUE_MAGNET = 3,
};

enum PowerUpSprites
{
    DISABLED      = 0,
    READY         = 1,
    ONE           = 2, // shield(lifes), freezing(rivals)
    TWO           = 3, // shield(lifes), freezing(rivals)
    THREE         = 4,  // shield(lifes), freezing(rivals)
    BEING_USED    = 2, //cyberattack, mirror
    EFFECTIVE     = 2 //magnet, thief

};

class PowerUpComponent : public IComponent{

    public:
        PowerUpComponent(glm::vec3* c_position, const std::string& id, int id_ent, Entity* d, const int attack_value, const int type_value,  GenericData *&l_genericData_ptr);
        virtual ~PowerUpComponent() = 0;
        virtual void ActivatePowerUp() = 0;
        virtual void SetActive(bool value) = 0;
        virtual void UpdateSprite() = 0;

        std::vector<std::string> GetSpritesVector(){ return m_sprites_vec; };
        void SetHacked(bool value){ m_hacked = value; ChangeSprite(DISABLED); };

        Entity* GetDrone()   const { return m_drone; };
        bool GetActive()     const { return m_active; };
        bool GetHacked()     const { return m_hacked; };
        int GetAttackValue() const { return m_ATTACK_VALUE; };
        int GetTypeValue()   const { return m_TYPE_VALUE; };

        void UpdateBillboard(int value);

        bool CheckIfCloseEnough(Entity* l_rival_drone_ptr, int l_drone_position_n, float l_distance_f);
        bool CheckIfPowerUpReady(bool reset_nitro);

        void ChangeSprite(PowerUpSprites sprite);
        int GetSprite(){ return (int)m_sprite; };

    protected:

        glm::vec3* m_position = nullptr;
        bool m_active = false;
        bool m_hacked = false;
        Entity* m_drone = nullptr; //pointer to drone that own this power up
        int m_ATTACK_VALUE = 0;
        int m_TYPE_VALUE = 0;

        EntityManager*    m_entityManager_ptr     = nullptr;
        PhysicsManager*   m_physicsManager_ptr    = nullptr;
        PositionManager*  m_positionManager_ptr   = nullptr;
        PowerUpManager*   m_powerUpManager_ptr    = nullptr;
        SoundManager*     m_soundManager_ptr      = nullptr;
        RenderManager*    m_renderManager_ptr     = nullptr;

        PICASSO::PhysicsVariables* m_drone_data = nullptr;
        PositionComponent* m_position_drone_ptr = nullptr;
        PowerUpSprites m_sprite = DISABLED;

        std::vector<std::string> m_sprites_vec;
};