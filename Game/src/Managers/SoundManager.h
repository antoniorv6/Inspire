#pragma once
#include <string>
#include <glm.hpp>
#include <vector>

class SoundEngine;
class SoundEngineFacade;
class SoundComponent;
class Entity;
class EntityManager;
class TransformComponent;
class GenericData;
class RandomMusicGen;

class SoundManager{
    public:
        SoundManager();
        
        void Init(GenericData *& c_genericData);
        void ClearList();
        
        void CreateComponent(Entity* entity, const std::string& eventID, const std::string& eventFMOD, unsigned int type);

        void Play(Entity* entity, const std::string& eventID);
        void Play(const std::string& eventID);
        void PlayAllSounds();

        void PauseAllDynamicSounds();
        void ResumeAllDynamicSounds();
        
        void Stop(Entity* entity, const std::string& eventID);
        void StopAllSounds();
        void Pause(Entity* entity, const std::string& eventID);
        void SetVolume(Entity* entity);
        void SetEventVariableValue(Entity* entity, const std::string& eventID, const std::string& variable, float value);
        void SetEventVariableValue(const std::string& eventID, const std::string& variable, float value);
        void SetEvent3DPosition(Entity* entity, const std::string& eventID, const glm::vec3& newPosition);
        void UpdateSoundEngine();

        void Update(const glm::vec3& listenerPosition);
        ~SoundManager();

        SoundEngineFacade* GetSoundEngine() { return soundEngine; }
    private:
        SoundEngineFacade* soundEngine = nullptr;                          //Pointer to the Sound Engine
        std::vector<SoundComponent*> m_dynamic_soundComponents;     //Vector of pointers to the dynamic sound components
        std::vector<SoundComponent*> m_static_soundComponents;      //Vector of pointers to the static sound components

        EntityManager* m_entityManager_ptr = nullptr;
        RandomMusicGen* m_randomMusicEng = nullptr;
};