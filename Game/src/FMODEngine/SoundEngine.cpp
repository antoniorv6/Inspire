#include <SoundEngine.h>

/**
 * @brief  Override of default ERRCHECK of FMOD API 
 *         It kills the application if the FMOD Sound Engine Fails on it's task, and says why
 * 
 * @param result Result of the operation
 * @param file   File that is affected of this operation
 * @param line   Line where the operation has been triggered
 */
void ERRCHECK_fn (FMOD_RESULT result, const char * file, int line)
{
	if(result != FMOD_OK)
	{
		std::cerr << file << "(" << line << "):FMOD_ERROR " << result << "-" << FMOD_ErrorString(result) << std::endl;
		exit(-1);
	}
}
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

/**
 * @brief Construct a new Sound Engine:: Sound Engine object
 *        It also initializes the FMOD System and Low Level API
 * 
 */
SoundEngine::SoundEngine()
{
    ERRCHECK(FMOD::Studio::System::create(&m_sound_system));
    //std::cout<<"[SUCCESS] - FMOD System created correctly"<<std::endl;
    ERRCHECK(m_sound_system->getLowLevelSystem(&m_low_level_system));
    //std::cout<<"[SUCCESS] - FMOD Low Level system created correctly"<<std::endl;
    m_sound_system->initialize(512, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_3D_RIGHTHANDED, 0);
    m_low_level_system->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);
	m_low_level_system->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);
}

/**
 * @brief Destroy the Sound Engine:: Sound Engine object
 * Unloads all data banks to avoid memory leaks
 */
SoundEngine::~SoundEngine()
{
    ERRCHECK(m_sound_system->unloadAll());
    ERRCHECK(m_sound_system->release());
    //std::cout<<"FMOD system fully released"<<std::endl;
}

/**
 * @brief Initializes the Sound Engine and loads the first banks (because they are mandatory)
 * 
 * @param c_masterBankLocation -> Location of the master bank data
 * @param c_masterBankStringsLocation -> Location of the Strings bank data
 */
void SoundEngine::Init(const std::string& c_masterBankLocation, const std::string& c_masterBankStringsLocation)
{
    ERRCHECK(m_sound_system->loadBankFile(c_masterBankLocation.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &m_masterBank));
    ERRCHECK(m_sound_system->loadBankFile(c_masterBankStringsLocation.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &m_stringBank));
    //std::cout<<"[SUCCESS] - Main sound banks created"<<std::endl;
}

/**
 * @brief Places the listener position on the 3D space
 * 
 * @param c_listenerPosition_v3  -> New position of the listener
 */
void SoundEngine::SetListenerPosition(const glm::vec3& c_listenerPosition_v3)
{
    FMOD_3D_ATTRIBUTES l_listenerAttributes {glm2FMODV(c_listenerPosition_v3, true), FMOD_VECTOR{0,0,0}, FMOD_VECTOR{0,0,1}, FMOD_VECTOR{0,1,0}};
    ERRCHECK(m_sound_system->setListenerAttributes(0, &l_listenerAttributes));
}

/**
 * @brief Sets an event position on the 3D space
 * 
 * @param fmodIdentifier -> ID of the event that has to change
 * @param c_eventPosition_v3 -> New location of the sound event
 */
void SoundEngine::SetEventPosition(const std::string& fmodIdentifier, const glm::vec3& c_eventPosition_v3, const glm::vec3& c_velocityVector_v3)
{
    if(!std::isnan(c_eventPosition_v3.x) && !std::isnan(c_eventPosition_v3.y) && !std::isnan(c_eventPosition_v3.z) && !std::isnan(c_velocityVector_v3.x) && !std::isnan(c_velocityVector_v3.y) && !std::isnan(c_velocityVector_v3.z))
    {
        FMOD_3D_ATTRIBUTES l_eventAttributes {glm2FMODV(c_eventPosition_v3, true), glm2FMODV(c_velocityVector_v3, true), FMOD_VECTOR{0,0,1}, FMOD_VECTOR{0,1,0}};
        ERRCHECK(m_events_map[fmodIdentifier]->set3DAttributes(&l_eventAttributes));
    }
}

/**
 * @brief Update the Sound Engine. It has to be called every time we render, so sound is syncronized with frame drawing rate.
 * 
 */
void SoundEngine::Update()
{
    ERRCHECK(m_sound_system->update());
}

/**
 * @brief 
 * 
 * @param sound_bank_id 
 * @param c_bankLocation 
 */
void SoundEngine::LoadSoundBank(SoundBanks sound_bank_id, const std::string& c_bankLocation)
{
    FMOD::Studio::Bank* l_bankToStore;
    ERRCHECK(m_sound_system->loadBankFile(c_bankLocation.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &l_bankToStore));
    m_soundBanks_map[sound_bank_id] = l_bankToStore;
}

void SoundEngine::UnLoadSoundBank(SoundBanks sound_bank_id)
{
    m_soundBanks_map[sound_bank_id]->unload();
}

/**
 * @brief Creates a Sound Event instance from FMOD API and stores it into memory
 * 
 * @param eventIdentifier -> The ID of the event in the std::map
 * @param fmodIdentifier -> The FMOD Event identifier (It has to be named as we named it in FMOD Studio)
 */
void SoundEngine::AddSoundEvent(const std::string& eventIdentifier, const std::string& fmodIdentifier)
{
    if(!(m_event_descriptions_map.count(eventIdentifier)))
    {
        FMOD::Studio::EventDescription * eventDescription = nullptr;
        ERRCHECK(m_sound_system->getEvent(fmodIdentifier.c_str(), &eventDescription));
        m_event_descriptions_map[eventIdentifier] = eventDescription;
    }
     
    FMOD::Studio::EventInstance * soundInstance = nullptr;
    ERRCHECK(m_event_descriptions_map[eventIdentifier]->createInstance(&soundInstance));
    m_events_map[eventIdentifier] = soundInstance;
}

/**
 * @brief Play a Sound Event
 * 
 * @param fmodIdentifier -> The ID of the event that we want to play
 */
void SoundEngine::PlaySound(const std::string& fmodIdentifier)
{
    ERRCHECK(m_events_map[fmodIdentifier]->start());
} 

/**
 * @brief Stop a Sound Event
 * 
 * @param fmodIdentifier -> The ID of the event that we want to stop
 */
void SoundEngine::StopSound(const std::string& fmodIdentifier)
{
    ERRCHECK(m_events_map[fmodIdentifier]->stop(FMOD_STUDIO_STOP_IMMEDIATE));
}

void SoundEngine::ReleaseAllSounds()
{
    for(auto& fmodEvent: m_events_map)
    {
        ERRCHECK(fmodEvent.second->release());
        fmodEvent.second = nullptr;
    }
    
    m_events_map.clear();
    //std::cout<<"[SUCCESS] - FMOD event map erased successfully"<<std::endl;
    SetListenerPosition(glm::vec3(0,0,0));
}

/**
 * @brief Changes a Sound Event variable value
 * 
 * @param fmodIdentifier -> Identifier on the std::map
 * @param variable -> Name of the variable (as it is settled in FMOD Studio)
 * @param value -> New value of the variable
 */
void SoundEngine::ChangeEventVariable(const std::string& fmodIdentifier, const std::string& variable, float value)
{
    ERRCHECK(m_events_map[fmodIdentifier]->setParameterValue(variable.c_str(), value));
}

void SoundEngine::PauseSound(const std::string& fmodIdentifier)
{
    ERRCHECK(m_events_map[fmodIdentifier]->setPaused(true));
}
void SoundEngine::ResumeSound(const std::string& fmodIdentifier)
{
    ERRCHECK(m_events_map[fmodIdentifier]->setPaused(false));
}

FMOD_VECTOR SoundEngine::glm2FMODV(const glm::vec3& c_inVec_v3, bool unitAdapt)
{
    return FMOD_VECTOR{ -c_inVec_v3.x/10, c_inVec_v3.z/10, c_inVec_v3.y/10};
}