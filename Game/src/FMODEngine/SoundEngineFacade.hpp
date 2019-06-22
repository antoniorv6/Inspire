
#include <string>
#include <glm.hpp>

enum SoundBanks
{
    Main = 0,
    Music = 1,
    FX = 2,
    Voices = 3
};

class SoundEngineFacade
{
    public:
        SoundEngineFacade(){};
        virtual ~SoundEngineFacade(){};
        virtual void Init(const std::string&, const std::string&)  = 0;
        virtual void LoadSoundBank(SoundBanks, const std::string&) = 0;
        virtual void UnLoadSoundBank(SoundBanks) = 0;
        virtual void AddSoundEvent(const std::string&, const std::string&) = 0;
        virtual void PlaySound(const std::string&) = 0;
        virtual void PauseSound(const std::string&) = 0;
        virtual void ResumeSound(const std::string&) = 0;
        virtual void StopSound(const std::string&) = 0;
        virtual void ChangeEventVariable(const std::string&, const std::string&, float) = 0;
        virtual void SetListenerPosition(const glm::vec3&) = 0;
        virtual void SetEventPosition(const std::string&, const glm::vec3&, const glm::vec3&) = 0;
        virtual void Update() = 0;
        virtual void ReleaseAllSounds() = 0;
};