#include <InGameMenuState.hpp>
#include <RenderManager.h>
#include <SoundManager.h>
#include <EventManager.h>
#include <StatesMachine.hpp>
#include <GenericData.hpp>
#include <IrrIMGUI.h>
#include <GUI.hpp>

void ResumeGame(Event event);
void ExitGame(Event event);

InGameMenuState::InGameMenuState(GenericData* c_gameData, EntityManager *l_entityManager,RenderManager *l_renderManager, SoundManager *l_soundManager, EventManager *l_eventManager)
{
    entityManager = l_entityManager;
    renderManager = l_renderManager;
    soundManager  = l_soundManager; 
    eventManager  = l_eventManager;
    gui           = new GUI(c_gameData);
}
InGameMenuState::~InGameMenuState()
{

}

void InGameMenuState::Init()
{
    statesMachine = StatesMachine::getInstance();

    eventManager->addListener(EventManager::Listener{ResumeGame}, EventType::Resume);
    eventManager->addListener(EventManager::Listener{ExitGame}, EventType::Exit);

    gui->AddGUIButton(350,400,240,80, EventType::Resume, 0, "resume_not_hovered.png", "resume_hovered.png");
    gui->AddGUIButton(950,400,240,80, EventType::Exit, 0, "exit_not_hovered.png", "exit_hovered.png");
}

void InGameMenuState::Update(float &delta)
{
    gui->Update();
    eventManager->update();
}

void InGameMenuState::Draw()
{
    renderManager->RenderScene(entityManager->GetEntity(0));
    soundManager->UpdateSoundEngine();
}

void InGameMenuState::ProcessGUIEvent(int l_exit, int none)
{
    if(l_exit == 1)
    {
        exit = true;
        this->ChangeState();
    }
    else
    {
        exit = false;
        this->ChangeState();
    }
}

void ResumeGame(Event event)
{
    StatesMachine::getInstance()->GetActiveState()->ProcessGUIEvent(0,-1);
}

void ExitGame(Event event)
{
    StatesMachine::getInstance()->GetActiveState()->ProcessGUIEvent(1,-1);
}

void InGameMenuState::ChangeState()
{
    delete gui;
    gui = nullptr;
    statesMachine->RemoveState(exit);
}
