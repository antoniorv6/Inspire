#include <RenderManager.h>
#include <EntityManager.h>
#include <InputManager.hpp>
#include <SoundManager.h>
#include <CameraManager.hpp>
#include <GUIManager.hpp>
#include <Entity.h>
#include <MainMenuState.hpp>
#include <IrrIMGUI.h>
#include <RaceState.hpp>
#include <SelectorMenuState.hpp>
#include <LobbyState.hpp>
#include <GenericData.hpp>
#include <EventManager.h>
#include <GUI.hpp>

void SinglePlayerChange(Event);
void ExitGameMenu(Event);

MainMenuState::MainMenuState(GenericData* c_gameData)
{
   entityManager = c_gameData->g_entityManager_ptr;
   render = c_gameData->g_renderManager_ptr;
   inputManager = c_gameData->g_inputManager_ptr;
   soundManager = c_gameData->g_soundManager_ptr;
   cameraManager = c_gameData->g_cameraManager_ptr;
   guiManager    = c_gameData->g_guiManager_ptr;
   m_eventManager_ptr = c_gameData->g_eventManager_ptr;
   
   m_gameData = c_gameData;
   m_GUI_ptr = new GUI(m_gameData);
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::Init()
{   
    m_demoCube = entityManager->CreateEntity("DemoCube",glm::vec3(0,0,0));
    render->CreateComponent(m_demoCube, "Cube", "");
    render->AddNode(m_demoCube);
	//soundManager->CreateComponent(player, "Drone", "event:/DroneSounds/Drone", 1);
	inputManager->CreateInputComponent(m_demoCube);
    cameraManager->SetTarget(m_demoCube);

    //guiManager->CreateMenuElement(m_demoCube, "Title", "./assets/git_photo.jpg", 0, 0, 200, 200);
    soundManager->CreateComponent(m_demoCube, "MenuMusic", "event:/Music/Main_Menu", 0);
    //soundManager->CreateComponent(m_demoCube, "RandomMenu", "event:/RandomMusic/RandomMusic", 0);

    soundManager->PlayAllSounds();

    soundManager->CreateComponent(m_demoCube, "MenuSounds", "event:/InterfaceEvents/MenuButtons", 0);

    m_GUI_ptr->SetBackground("menu_fondo.jpg");

    m_GUI_ptr->AddGUIButton(320, 650, 240, 80, Single_Player, 0,"single_not_hovered.png","single_hovered.png");
    m_GUI_ptr->AddGUIButton(620, 650, 240, 80, Single_Player, 0,"multi_not_hovered.png","multi_hovered.png");
    m_GUI_ptr->AddGUIButton(920, 650, 240, 80, Exit_Game, 0,"exit_not_hovered.png","exit_hovered.png");
    
    m_eventManager_ptr->addListener(EventManager::Listener{SinglePlayerChange}, EventType::Single_Player);
    m_eventManager_ptr->addListener(EventManager::Listener{ExitGameMenu}, EventType::Exit_Game);

}

void MainMenuState::Update(float &delta)
{
    soundManager->UpdateSoundEngine();
    m_GUI_ptr->Update();
    render->DrawGUI();
    m_eventManager_ptr->update();
}

void MainMenuState::Draw()
{
    if(m_demoCube)
        render->RenderScene(m_demoCube);
}

void MainMenuState::DrawStateGUI()
{}

void MainMenuState::ChangeState()
{
    delete m_GUI_ptr;
    m_GUI_ptr = nullptr;
    render->Reset();
    entityManager->ClearList();
    inputManager->ClearList();
    guiManager->ClearList();
    cameraManager->ClearList();
    soundManager->StopAllSounds();
    soundManager->ClearList();
    soundManager->UpdateSoundEngine();
    StatesMachine::getInstance()->AddState(std::make_unique<SelectorMenuState>(m_gameData));
    m_demoCube = nullptr;
}

void MainMenuState::ChangetoOnline()
{
    delete m_GUI_ptr;
    m_GUI_ptr = nullptr;
    render->Reset();
    entityManager->ClearList();
    inputManager->ClearList();
    guiManager->ClearList();
    cameraManager->ClearList();
    soundManager->UpdateSoundEngine();
    StatesMachine::getInstance()->AddState(std::make_unique<LobbyState>(m_gameData));
    m_demoCube = nullptr;
}

void MainMenuState::ProcessGUIEvent(int c_eventType, int c_action)
{
    render->CloseWindow();
}

void MainMenuState::CheckForStateChange()
{
    deleteState = true;
}

void MainMenuState::CloseState()
{
    delete m_GUI_ptr;
    m_GUI_ptr = nullptr;
}

void SinglePlayerChange(Event)
{
    StatesMachine::getInstance()->GetActiveState()->ChangeState();
}

void ExitGameMenu(Event)
{
    StatesMachine::getInstance()->GetActiveState()->ProcessGUIEvent(0,0);
}