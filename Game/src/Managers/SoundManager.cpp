#include "SoundManager.h"
#include "SoundEngine.h"
#include "SoundComponent.h"
#include "EntityManager.h"
#include "Entity.h"
#include "TransformComponent.h"
#include <GenericData.hpp>
#include <RandomMusicGen.hpp>

SoundManager::SoundManager()
{
    soundEngine = new SoundEngine();
    soundEngine->Init("assets/SoundBanks/Master_Bank.bank","assets/SoundBanks/Master_Bank.strings.bank");
    soundEngine->LoadSoundBank(SoundBanks::Music,"assets/SoundBanks/Music.bank");
    soundEngine->LoadSoundBank(SoundBanks::FX,"assets/SoundBanks/FX.bank");

    m_randomMusicEng = new RandomMusicGen(110, 1, 0, "RandomMenu", soundEngine);
}

void SoundManager::Init(GenericData *& c_genericData)
{
    m_entityManager_ptr = c_genericData->g_entityManager_ptr;
}

/**
 * @brief Destroys the Sound Manager and cleans memory from the sound Component vectors
 * 
 */
SoundManager::~SoundManager()
{
    ClearList();
    delete soundEngine;
    soundEngine = nullptr;
    delete m_randomMusicEng;
    m_randomMusicEng = nullptr;
}

/**
 * @brief Frees memory from the dynamic and static sound vectors
 * 
 */
void SoundManager::ClearList()
{
    StopAllSounds();
    //** DYNAMIC VECTOR ERASING **//
    for(unsigned int i = 0; i < m_dynamic_soundComponents.size(); i++)
     {
        delete m_dynamic_soundComponents[i];
        m_dynamic_soundComponents[i] = nullptr;
     }
    
    //** STATIC VECTOR ERASING **//
     for(unsigned int i = 0; i < m_static_soundComponents.size(); i++)
     {
        delete m_static_soundComponents[i];
        m_static_soundComponents[i] = nullptr;
     }
    
     soundEngine->ReleaseAllSounds();
     
    m_dynamic_soundComponents.clear();
    m_static_soundComponents.clear();
}

/**
 * @brief Creates a new sound component for the designed entity
 * 
 * @note -> A single Entity can obtain several sound components, because it will handle different variables from the FMOD events.
 *          In order to make it clear, the entities will manage several FMOD Events wich are identified as SoundComponents -> SoundComponent = FMOD Event Instance
 * 
 * @param entity -> The entity that gets the sound component
 * @param eventID -> The ID that the Sound Component is going to have in our Sound Engine
 * @param eventFMOD -> The ID of the FMOD Event that is going to handle, same as it was defined in the FMOD Studio Project
 * @param type -> The type of the sound ==> 0 = STATIC (Stored in static sound evetns) -- 1 = DYNAMIC (Stored in dynamic events and updated every single Update call)
 */
void SoundManager::CreateComponent(Entity* entity, const std::string& eventID, const std::string& eventFMOD, unsigned int type){
    SoundComponent* sound = new SoundComponent(eventID,eventID, eventFMOD, entity->GetId());
    m_entityManager_ptr->AddComponent(entity->GetId(), (IComponent*) sound);
    soundEngine->AddSoundEvent(eventID, eventFMOD);

    if(type == 0)
        m_static_soundComponents.push_back(sound);
    else
        m_dynamic_soundComponents.push_back(sound);
}

/**
 * @brief Triggers a specific sound event
 * 
 * @param entity Entity that handles de Sound Component
 * @param eventID Id of the Component (Remember!!! Component = FMOD Event)
 */
void SoundManager::Play(Entity* entity, const std::string& eventID){
    IComponent* component =  m_entityManager_ptr->GetComponent(entity->GetId(), eventID);
    if(component != nullptr){

        SoundComponent* soundcomponent = ((SoundComponent*) component);

        soundEngine->PlaySound(soundcomponent->getID());

    }
}

void SoundManager::Play(const std::string& eventID)
{
    soundEngine->PlaySound(eventID);
}

/**
 * @brief Directly play all sound components stored
 * 
 */
void SoundManager::PlayAllSounds()
{
    for (SoundComponent* component : m_dynamic_soundComponents)
    {
        soundEngine->PlaySound(component->getID());
    }

    for (SoundComponent* component : m_static_soundComponents)
    {
        soundEngine->PlaySound(component->getID());
    }
}

/**
 * @brief Stop a specific sound event
 * 
 * @param entity Entity that handles de Sound Component
 * @param eventID Id of the Component (Remember!!! Component = FMOD Event)
 */
void SoundManager::Stop(Entity* entity, const std::string& eventID){
    IComponent* component =  m_entityManager_ptr->GetComponent(entity->GetId(), eventID);
    if(component != nullptr){
        SoundComponent* soundcomponent = ((SoundComponent*) component);
        soundEngine->StopSound(soundcomponent->getID());
    }
}

/**
 * @brief Stops all sound events
 * 
 */
void SoundManager::StopAllSounds()
{
    for (SoundComponent* component : m_dynamic_soundComponents)
    {
        soundEngine->StopSound(component->getID());
    }
    for (SoundComponent* component : m_static_soundComponents)
    {
        soundEngine->StopSound(component->getID());
    }
}

/**
 * @brief Sets an event variable value
 * 
 * @param entity -> Entity that handles the sound event
 * @param eventID -> The specific event that has to change
 * @param variable -> The variable that has to be changed
 * @param value -> The new value of the variable
 */
void SoundManager::SetEventVariableValue(Entity* entity, const std::string& eventID, const std::string& variable, float value)
{
    IComponent* component =  m_entityManager_ptr->GetComponent(entity->GetId(), eventID);
    if(component != nullptr){
        SoundComponent* soundcomponent = ((SoundComponent*) component);
        soundEngine->ChangeEventVariable(soundcomponent->getID(), variable, value);
    }
}

void SoundManager::SetEventVariableValue(const std::string& eventID, const std::string& variable, float value)
{
    soundEngine->ChangeEventVariable(eventID, variable, value);
}

/**
 * @brief Updates the dynamic events positions, the listener's position and the sound engine.
 * 
 * @param listenerPosition -> The position of the listener
 */
void SoundManager::Update(const glm::vec3& listenerPosition)
{
    soundEngine->SetListenerPosition(listenerPosition);
    soundEngine->SetEventPosition(m_dynamic_soundComponents[0]->getID(), glm::vec3(listenerPosition.x, listenerPosition.y + 40, listenerPosition.z - 20), glm::vec3(0,0,0));
    soundEngine->SetEventPosition(m_static_soundComponents[0]->getID(), glm::vec3(listenerPosition.x, listenerPosition.y, listenerPosition.z), glm::vec3(0,0,0));
    TransformComponent* playerComp = ((TransformComponent*)m_entityManager_ptr->GetComponent(m_dynamic_soundComponents[0]->GetEntityId(), "Physics"));
    glm::vec3 velocityVector = playerComp->GetPhysicsData()->v_velocity;
    float velocity = sqrt(velocityVector.x*velocityVector.x + velocityVector.y*velocityVector.y);
    soundEngine->ChangeEventVariable(m_dynamic_soundComponents[0]->getID(),"Speed", velocity);
    
    //Loop that updates all the event positions in space, maybe i design it to have the movable events stored in one vector and the static in another
    for(unsigned int i = 1; i<m_dynamic_soundComponents.size(); i++)
    {
        IComponent* transformable = m_entityManager_ptr->GetComponent(m_dynamic_soundComponents[i]->GetEntityId(), "Physics");
        soundEngine->SetEventPosition(m_dynamic_soundComponents[i]->getID(), m_entityManager_ptr->GetEntity(m_dynamic_soundComponents[i]->GetEntityId())->getPos(), ((TransformComponent*)transformable)->GetPhysicsData()->v_velocity);
    }

    UpdateSoundEngine();
}

void SoundManager::UpdateSoundEngine()
{
    soundEngine->Update();
    //m_randomMusicEng->Play();
}

/**
 * @brief Sets the 3D position of a single sound event
 * 
 * @param entity -> The entity that handles that event
 * @param eventID  -> The ID of the Component that handles that event
 * @param newPosition -> The new position that the Event is going to have
 */
void SoundManager::SetEvent3DPosition(Entity* entity, const std::string& eventID, const glm::vec3& newPosition)
{
    SoundComponent* component = (SoundComponent*) m_entityManager_ptr->GetComponent(entity->GetId(), eventID);
    soundEngine->SetEventPosition(component->getID(), newPosition, glm::vec3(0,0,0));
}

void SoundManager::PauseAllDynamicSounds()
{
    for (SoundComponent* component : m_dynamic_soundComponents)
    {
        soundEngine->PauseSound(component->getID());
    }
}

void SoundManager::ResumeAllDynamicSounds()
{
    for (SoundComponent* component : m_dynamic_soundComponents)
    {
        soundEngine->ResumeSound(component->getID());
    }
}

void SoundManager::Pause(Entity* entity, const std::string& eventID)
{

}

void SoundManager::SetVolume(Entity* entity){

}