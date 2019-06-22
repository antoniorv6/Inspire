#pragma once

#include <PhysicsManager.h>
#include <PUMagnetComponent.hpp>
#include <PUShieldComponent.hpp>
#include <PUCyberattackComponent.hpp>
#include <PUThiefComponent.hpp>
#include <PUFreezingComponent.hpp>
#include <PUReflectorComponent.hpp>
#include <TransformComponent.h>
#include <PositionComponent.hpp>
#include <EntityManager.h>
#include <glm.hpp>
#include <vector>
#include <iostream>

class GenericData;
class Event;
class RenderManager;
class PositionManager;
class SoundManager;
class HUD;

class PowerUpManager{

    friend class PUCyberattackComponent;
    friend class PUFreezingComponent;
    friend class PUMagnetComponent;
    friend class PUShieldComponent;
    friend class PUReflectorComponent;
    friend class PUThiefComponent;
    friend class PowerUpComponent;
    
    public:
        PowerUpManager();
        ~PowerUpManager();

        void Init(GenericData *&c_gameData);
        void InitHUD(HUD*& c_HUD_ptr);
        
        void Update();
        void ClearList();
        bool CreatePowerUp(TypePU l_type, Entity* l_drone_ptr);
        void CreatePUEvent(int l_droneId_n, int l_numberPowerUp_n);

        bool CheckIfRivalHasReflector(Entity*);
        bool CheckIfRivalHasShield(Entity*);
        bool CheckIfRivalHasShield(int);

        void UpdateActiveShields();
        void UpdateActiveCyberattacks();
        void UpdateActiveReflectors();
        
        void UpdateBillboardsPU();

        int GetDroneAttackValue(int l_entityId_n);
        int GetPowerUpTypeValue(int l_entityId_n, int l_numPU);

        int GetSprite(int i);
        void SetSprite(int texture, PowerUpComponent* component);
        void CheckPowerUpSprites();
        void SetHudPowerUpSprites();

        void PauseClocks();
        void PlayClocks();

        void SetEventData(GenericData *&l_eventData){ m_eventData_ptr = l_eventData; };

    private:

        EntityManager*   m_entityManager_ptr      = nullptr;
        GenericData*     m_eventData_ptr          = nullptr;
        RenderManager*   m_renderManager_ptr      = nullptr;
        PhysicsManager*  m_physicsManager_ptr     = nullptr;
        EventManager*    m_eventManager_ptr       = nullptr;
        PositionManager* m_positionManager_ptr    = nullptr;
        SoundManager*    m_soundManager_ptr       = nullptr;
        HUD*             m_HUD_ptr                = nullptr;

        //=====BILLBOARDS=====

        // Rival Billboard
        int billboard = -1;
        Entity* m_billboard_rival = nullptr;//billboard rival

        // Shields Billbaords
        std::vector<Entity*> m_entityShieldBB_v;
        int m_numShieldBB = 3;

        //====================

        std::vector<PowerUpComponent*> m_powerUps_v; //list of all power ups components
        std::vector<PUShieldComponent*> m_activeShields_v; //list of the activated shields
        std::vector<PUCyberattackComponent*> m_activeCyberattack_v; // list of the activated cyberattacks
        std::vector<PUReflectorComponent*> m_activeReflectors_v; // list of the activated reflectors
        std::vector<PowerUpComponent*> m_playerPU_v; //list of the player power ups

};