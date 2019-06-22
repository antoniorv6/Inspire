#include <iostream>
#include <map>
#include <string>
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <Entity.h>
#include <SoundComponent.h>
#include <EntityManager.h>
#include <SoundEngineFacade.hpp>

class SoundEngine : public SoundEngineFacade
{
    public:
        SoundEngine();
        ~SoundEngine();
        void Init(const std::string& c_masterBankLocation, const std::string& c_masterBankStringsLocation);
        
        void LoadSoundBank(SoundBanks sound_bank_id, const std::string& c_bankLocation);
        void UnLoadSoundBank(SoundBanks sound_bank_id);
        
        void AddSoundEvent(const std::string& eventIdentifier, const std::string& fmodIdentifier);
        void PlaySound(const std::string& fmodIdentifier);
        void PauseSound(const std::string& fmodIdentifier);
        void ResumeSound(const std::string& fmodIdentifier);
        void StopSound(const std::string& fmodIdentifier);
        void ChangeEventVariable(const std::string& fmodIdentifier, const std::string& variable, float value);
        void SetListenerPosition(const glm::vec3& c_listenerPosition_v3);
        void SetEventPosition(const std::string& fmodIdentifier, const glm::vec3& c_eventPosition_v3, const glm::vec3& c_velocityVector_v3);
        void Update();
        void ReleaseAllSounds();

        FMOD_VECTOR glm2FMODV(const glm::vec3& c_inVec_v3, bool unitAdapt);
    
    private:
        FMOD::Studio::System * m_sound_system = nullptr;                                    //Pointer to the FMOD Studio API System
        FMOD::System * m_low_level_system = nullptr;                                        //Pointer to the FMOD Studio Low Level API System (not used but important to run the API)
        FMOD::Studio::Bank* m_masterBank = nullptr;                                         //Master bank pointer
        FMOD::Studio::Bank* m_stringBank = nullptr;                                         //Strings bank pointer
        
        std::map<int, FMOD::Studio::Bank*> m_soundBanks_map;                        //Sound banks map of pointers, in case we have to store different sound banks
        std::map<std::string, FMOD::Studio::EventDescription*> m_event_descriptions_map;    //Event Descriptions map, it stores pointers to event descriptions, needed to create event instances
        std::map<std::string, FMOD::Studio::EventInstance*> m_events_map;                   //Event Instances map, it stores pointers to event instances, that are handled by Sound Components
};