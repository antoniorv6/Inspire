#include <RenderManager.h>
#include <EntityManager.h>
#include <InputManager.hpp>
#include <SoundManager.h>
#include <CameraManager.hpp>
#include <Entity.h>
#include <SelectorMenuState.hpp>
#include <LoadingScreenState.hpp>
#include <LobbyState.hpp>
#include <GenericData.hpp>
#include <Clock.hpp>
#include <EventManager.h>
#include <GUI.hpp>
#include <CommonData.hpp>
#include <MainMenuState.hpp>

void RightPowerUpHandle(Event event);
void LeftPowerUpHandle(Event event);
void InitGame(Event);
void SwitchLeftDrone(Event);
void SwitchRightDrone(Event);
void GoToPUSelection(Event);
void GoBack(Event);

SelectorMenuState::SelectorMenuState(GenericData* c_gameData)
{
   entityManager    = c_gameData->g_entityManager_ptr;
   render           = c_gameData->g_renderManager_ptr;
   inputManager     = c_gameData->g_inputManager_ptr;
   soundManager     = c_gameData->g_soundManager_ptr;
   cameraManager    = c_gameData->g_cameraManager_ptr;
   m_eventManager   = c_gameData->g_eventManager_ptr;
   m_physicsManager = c_gameData->g_physicsManager_ptr;
   
   m_gameData = c_gameData;

   m_GUI_ptr = new GUI(m_gameData);
   m_clickClock_ptr = new Clock();
   m_clickClock_ptr->Init();
   //m_movementDirection = 0;
   m_actualState = SelectorState::SELECT_DRONE;

   vladimir = nullptr;
   karen = nullptr;
   jack = nullptr;
   lyanna = nullptr;


   vladimir = entityManager->CreateEntity("Vladimir", glm::vec3(53,-4,0));
}

SelectorMenuState::~SelectorMenuState()
{
}

void SelectorMenuState::Init()
{
    m_powerUpsTextures_v.reserve(6);
    m_powerUpsTextures_v.emplace_back("shield_selector.png");
    m_powerUpsTextures_v.emplace_back("magnet_selector.png");
    m_powerUpsTextures_v.emplace_back("thief_selector.png");
    m_powerUpsTextures_v.emplace_back("cyberattack_selector.png");
    m_powerUpsTextures_v.emplace_back("freezer_selector.png");
    m_powerUpsTextures_v.emplace_back("mirror_selector.png");

    m_dronesToChoose.reserve(4);
    m_droneEntitites.reserve(4);

    m_dronesToChoose.emplace_back(DroneData{"Jack", "rocket_1_LOD_1", "Rocket.png", "rocket",01});
    m_dronesToChoose.emplace_back(DroneData{"Karen", "spider_1_LOD_1", "texture_spiderdrone.png", "spider", 12});
    m_dronesToChoose.emplace_back(DroneData{"Lyanna", "science_1_LOD_1", "ScienceDroneTexture.png", "science", 43});
    m_dronesToChoose.emplace_back(DroneData{"Vladimir", "music_1_LOD_1", "texture_dron_musical.png", "music", 52});

    
    m_demoCube = entityManager->CreateEntity("DemoCube",glm::vec3(0,0,0));
    render->CreateComponent(m_demoCube, "Cube", "");
    render->AddNode(m_demoCube); 

    m_billboardPos = glm::vec3(-20, -1000, -1800);

    render->GetRenderFacade()->CreateBillboard(m_demoCube->GetId(), "selectpowerups.jpg", 8000, 4200, m_billboardPos); 
    
    for(size_t i = 0 ; i<m_dronesToChoose.size(); i++)
    {
        Entity* l_newEntity = entityManager->CreateEntity(m_dronesToChoose[i].m_characterName, glm::vec3(0 + 25*i, 0, 0));
        InitDroneToShow(l_newEntity, m_dronesToChoose[i].droneModel, m_dronesToChoose[i].droneTexture);
        m_droneEntitites.emplace_back(l_newEntity);
    }

    //Mansion 
    //mansion = entityManager->CreateEntity("Mansion", glm::vec3(-80, 0, -5));
    //mansion = entityManager->CreateEntity("Mansion", glm::vec3(0, 0, 0));
    //render->CreateComponent(mansion, "Mesh", "mansion1");
    //render->AddNode(mansion, 1);
    //render->AddTexture(mansion, "mansionTexture.png");
    //render->ScaleSceneNode(mansion, 0.015f);
    //Character animations initialization
    vladimir = entityManager->CreateEntity("Vladimir", glm::vec3(4 + 25*3, -4, 0));
    render->CreateComponent(vladimir, "Animated", "Vladimir");
    render->AddNode(vladimir);
	render->AddAnimatedSecuence(vladimir, "Idle", {"idle1_vladimir", "idle2_vladimir", "idle3_vladimir", "idle4_vladimir", "idle5_vladimir", "idle6_vladimir"}, true);
	render->AddAnimatedSecuence(vladimir, "Seleccion", {"seleccion2_vladimir", "seleccion3_vladimir", "seleccion4_vladimir",
    "seleccion5_vladimir", "seleccion6_vladimir", "seleccion7_vladimir", "seleccion8_vladimir", "seleccion9_vladimir", "seleccion10_vladimir",
    "seleccion11_vladimir", "seleccion12_vladimir", "seleccion13_vladimir", "seleccion14_vladimir", "seleccion15_vladimir", "seleccion16_vladimir"}, true);
    render->AddTexture(vladimir, "vladimir_texture.png", "Idle");
    render->AddTexture(vladimir, "vladimir_texture.png", "Seleccion");


    karen = entityManager->CreateEntity("Karen", glm::vec3(28, -4, 0));
    render->CreateComponent(karen, "Animated", "Karen");
    render->AddNode(karen);
	render->AddAnimatedSecuence(karen, "Idle", {"idle1_karen", "idle2_karen", "idle3_karen", "idle4_karen", "idle5_karen", "idle6_karen",
    "idle5_karen", "idle4_karen", "idle3_karen", "idle2_karen", "idle1_karen"}, true);
	render->AddAnimatedSecuence(karen, "Seleccion", {"seleccion2_karen", "seleccion3_karen", "seleccion4_karen",
    "seleccion5_karen", "seleccion6_karen", "seleccion7_karen", "seleccion8_karen", "seleccion9_karen", "seleccion10_karen", "seleccion11_karen",
    "seleccion12_karen", "seleccion13_karen", "seleccion14_karen", "seleccion15_karen", "seleccion16_karen"}, true);
    render->AddTexture(karen, "karen_texture.png", "Idle");
    render->AddTexture(karen, "karen_texture.png", "Seleccion");



    jack = entityManager->CreateEntity("Jack", glm::vec3(3, -4, 0));
    render->CreateComponent(jack, "Animated", "Jack");
    render->AddNode(jack);
	render->AddAnimatedSecuence(jack, "Idle", {"idle1_jack", "idle2_jack", "idle3_jack", "idle4_jack", "idle5_jack", "idle6_jack"}, true);
	render->AddAnimatedSecuence(jack, "Seleccion", {"seleccion2_jack", "seleccion3_jack", "seleccion4_jack", "seleccion5_jack",
    "seleccion6_jack", "seleccion7_jack", "seleccion8_jack", "seleccion9_jack", "seleccion10_jack", "seleccion11_jack", "seleccion12_jack",
    "seleccion13_jack", "seleccion14_jack", "seleccion15_jack", "seleccion16_jack"}, true);
    render->SetSecuence(jack->GetId(), "Idle");
    render->ScaleSceneNode(jack, 0.5f);
    render->AddTexture(jack, "jack_texture.png", "Idle");
    render->AddTexture(jack, "jack_texture.png", "Seleccion");


    lyanna = entityManager->CreateEntity("Lyanna", glm::vec3(3 + 25*2, -4, 0));
    render->CreateComponent(lyanna, "Animated", "Lyanna");
    render->AddNode(lyanna);
    render->ScaleSceneNode(lyanna,1.0f);
	render->AddAnimatedSecuence(lyanna, "Idle", {"idle1_lyanna", "idle2_lyanna", "idle3_lyanna", "idle4_lyanna"}, true);
	render->AddAnimatedSecuence(lyanna, "Seleccion", {"seleccion1_lyanna", "seleccion2_lyanna", "seleccion3_lyanna", "seleccion4_lyanna", "seleccion5_lyanna",
    "seleccion6_lyanna", "seleccion7_lyanna", "seleccion8_lyanna", "seleccion9_lyanna", "seleccion10_lyanna", "seleccion11_lyanna", "seleccion12_lyanna"}, true);
    render->AddTexture(lyanna, "lyanna_texture.png", "Idle");
    render->AddTexture(lyanna, "lyanna_texture.png", "Seleccion");

    //In this moment, all drones have been initialized correctly
	
    soundManager->CreateComponent(m_demoCube,"Voices", "event:/InterfaceEvents/SelectorState", 0);
    soundManager->CreateComponent(m_demoCube, "SelectorMenuMusic", "event:/Music/Selector_menu", 0);
    soundManager->CreateComponent(m_demoCube, "MenuSounds", "event:/InterfaceEvents/MenuButtons", 0);
    inputManager->CreateInputComponent(m_demoCube);

    //CAMERA VARIABLES 
    m_desiredPosition = {0,0,0};
    m_increment = {-1,-1,-1};
    m_perspective = 50;

    //MENU CAMERA
    cameraManager->SetTarget(m_demoCube);
    cameraManager->InitCameraData(3); // 3 = Menu Camera
    cameraManager->CalculateMenuCameraData();

    render->SetPerspectiveAngle(m_perspective);

    render->GetRenderFacade()->AddLight(glm::vec3(0,100,0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 0.0000000000014, 0.000000000000000007));

    //guiManager->CreateMenuElement(m_demoCube, "Title", "./assets/git_photo.jpg", 0, 0, 200, 200);

    //soundManager->CreateComponent(m_demoCube, "MenuMusic", "event:/Music/Main_Menu", 0);
    //soundManager->PlayAllSounds();

    SetDroneSelectionButtons();

    //Setup of all the listeners to handle the possible events that GUI will trigger during this state
    m_eventManager->addListener(EventManager::Listener{RightPowerUpHandle}, EventType::RightPowerUP);
    m_eventManager->addListener(EventManager::Listener{LeftPowerUpHandle},   EventType::LeftPowerUP);
    m_eventManager->addListener(EventManager::Listener{InitGame}, EventType::StartGame);
    m_eventManager->addListener(EventManager::Listener{SwitchLeftDrone}, EventType::LeftDrone);
    m_eventManager->addListener(EventManager::Listener{SwitchRightDrone}, EventType::RightDrone);
    m_eventManager->addListener(EventManager::Listener{GoToPUSelection}, EventType::ChooseDrone);
    m_eventManager->addListener(EventManager::Listener{GoBack}, EventType::Selector_back);


    soundManager->Play(m_demoCube, "SelectorMenuMusic");   

}

void SelectorMenuState::InitDroneToShow(Entity* c_entity, const std::string& c_mesh, const std::string& c_texture)
{
    render->CreateComponent(c_entity, "Mesh", c_mesh);
    render->AddNode(c_entity, 1);
    render->AddTexture(c_entity, c_texture);

}

void SelectorMenuState::ShowPUMenu()
{
    m_GUI_ptr->AddGUIButton(65, 370, 40, 40, LeftPowerUP, -1, "left_not_hovered.png","left_hovered.png");
    m_GUI_ptr->AddGUIButton(400, 370, 40, 40, LeftPowerUP, 1, "right_not_hovered.png","right_hovered.png");
    m_powerUpLeftGUI = m_GUI_ptr->AddGUIButton(230, 375, 350, 350, Void_Event, 0,m_powerUpsTextures_v[m_selected_PowerUpLEFT], m_powerUpsTextures_v[m_selected_PowerUpLEFT]);
    m_GUI_ptr->AddGUIButton(882, 370, 40, 40,   RightPowerUP, -1,"left_not_hovered.png","left_hovered.png");
    m_GUI_ptr->AddGUIButton(1222, 370, 40, 40,  RightPowerUP, 1,"right_not_hovered.png","right_hovered.png");
    m_PowerUpRightGUI = m_GUI_ptr->AddGUIButton(1050, 375, 350, 350, Void_Event, 0,m_powerUpsTextures_v[m_selected_PowerUpRIGHT], m_powerUpsTextures_v[m_selected_PowerUpRIGHT]);
    m_GUI_ptr->AddGUIButton(640, 650, 240, 80, StartGame, 0,"start_race_not_hovered.png","start_race_hovered.png");

    m_GUI_ptr->AddGUIButton(100, 100, 240, 80, Selector_back, 0,"back_not_hovered.png","back_hovered.png");

    m_GUI_ptr->AddGUIButton(640, 100, 260, 100, Void_Event, 0,"choose_pus.png","choose_pus.png");

}

void SelectorMenuState::SetDroneSelectionButtons()
{
    //m_GUI_ptr->SetBackground("selectpowerups.jpg");
    m_GUI_ptr->AddGUIButton(80, 350, 80, 80, LeftDrone, -1, "left_not_hovered.png","left_hovered.png");
    m_GUI_ptr->AddGUIButton(1200, 350, 80, 80, RightDrone, 1, "right_not_hovered.png","right_hovered.png");
    m_GUI_ptr->AddGUIButton(640, 650, 240, 80, ChooseDrone, 0,"select_not_hovered.png","select_hovered.png");
    m_GUI_ptr->AddGUIButton(100, 100, 240, 80, Selector_back, 0,"back_not_hovered.png","back_hovered.png");
    m_GUI_ptr->AddGUIButton(640, 100, 260, 100, Void_Event, 0,"select_your_drone.png","select_your_drone.png");

}

void SelectorMenuState::Update(float &delta)
{
    m_GUI_ptr->Update();
    m_eventManager->update();
    soundManager->UpdateSoundEngine();

    if(m_demoCube)
    {
        MoveTarget();
        RotateDrones();
        UpdatePerspective();
    }
}

void SelectorMenuState::Draw()
{
    if(m_demoCube)
        render->RenderScene(m_demoCube);
}

void SelectorMenuState::ChangeState()
{
    SavePlayerPU();
    delete m_GUI_ptr;
    m_GUI_ptr = nullptr;
    render->Reset();
    entityManager->ClearList();
    
    inputManager->ClearList();
    
    soundManager->StopAllSounds();
    soundManager->ClearList();
    
    cameraManager->ClearList();
    
    soundManager->UpdateSoundEngine();

    m_physicsManager->ClearList();

    delete m_clickClock_ptr;
    m_clickClock_ptr = nullptr;
    m_demoCube = nullptr;
    StatesMachine::getInstance()->AddState(std::make_unique<LoadingScreenState>(m_gameData, m_selected_PowerUp, m_dronesToChoose[m_selectedDrone].m_characterName, m_dronesToChoose[m_selectedDrone].droneModel, m_dronesToChoose[m_selectedDrone].droneTexture, m_dronesToChoose[m_selectedDrone].droneName));

    ClearVectors();

    vladimir = nullptr;
    karen = nullptr;
    jack = nullptr;
    lyanna = nullptr;
    //mansion = nullptr;
}

void SelectorMenuState::GoToMainMenu()
{
    delete m_GUI_ptr;
    m_GUI_ptr = nullptr;
    render->Reset();
    entityManager->ClearList();
    
    inputManager->ClearList();
    
    soundManager->StopAllSounds();
    soundManager->ClearList();
    
    cameraManager->ClearList();
    
    soundManager->UpdateSoundEngine();

    m_physicsManager->ClearList();

    delete m_clickClock_ptr;
    m_clickClock_ptr = nullptr;
    m_demoCube = nullptr;
    StatesMachine::getInstance()->AddState(std::make_unique<MainMenuState>(m_gameData));

    ClearVectors();

    vladimir = nullptr;
    karen = nullptr;
    jack = nullptr;
    lyanna = nullptr;
    //mansion = nullptr;
}

void SelectorMenuState::ClearVectors()
{
    std::vector<std::string>empty;
    std::vector<DroneData>emptyDroneData;
    std::vector<Entity*>emptyDronePointers;
    m_droneEntitites.swap(emptyDronePointers);
    m_dronesToChoose.swap(emptyDroneData);
    m_powerUpsTextures_v.swap(empty);

    m_desiredPosition = {0,0,0};
    m_increment = {-1,-1,-1};
}

void SelectorMenuState::CheckForStateChange()
{
    deleteState = true;
}

void SelectorMenuState::DrawStateGUI()
{
    
}

void SelectorMenuState::SelectPowerUp(int* PowerUp, int direction)
{  
    *PowerUp += direction;
    
    //If it is bigger than the total ammount of power ups, then we put it into 0
    if(*PowerUp > 5)
    {
        *PowerUp = 0;
    }
    if(*PowerUp < 0)
    {
        *PowerUp = 5;
    }

    //Mini recursive way of solving repeated power ups, it will only iterate once (has to be tested)
    if(m_selected_PowerUpLEFT == m_selected_PowerUpRIGHT)
    {
        SelectPowerUp(PowerUp, direction);
    }
}

bool SelectorMenuState::SavePlayerPU()
{
    if(m_selected_PowerUpLEFT != -1 && m_selected_PowerUpRIGHT != -1)
    {
        m_selected_PowerUp = m_selected_PowerUpLEFT * 10 + m_selected_PowerUpRIGHT;
        return true;
    }
    
    return false;
}

void SelectorMenuState::ProcessGUIEvent(int c_eventType, int c_action)
{
    if(m_clickClock_ptr->GetElapsedTime() > 0.1)
    {
        if(c_eventType == 0)
        {
            SelectPowerUp(&m_selected_PowerUpRIGHT, c_action);
            m_GUI_ptr->ChangeButtonTextures(m_PowerUpRightGUI, m_powerUpsTextures_v[m_selected_PowerUpRIGHT], m_powerUpsTextures_v[m_selected_PowerUpRIGHT]);
        }
        
        if (c_eventType == 1)
        {
            SelectPowerUp(&m_selected_PowerUpLEFT, c_action);
            m_GUI_ptr->ChangeButtonTextures(m_powerUpLeftGUI, m_powerUpsTextures_v[m_selected_PowerUpLEFT], m_powerUpsTextures_v[m_selected_PowerUpLEFT]);
        }

        if(c_eventType == 2)
        {
            if(m_desiredPosition == m_demoCube->getPos())
                SelectDrone(-1);
        }

        if(c_eventType == 3)
        {
            if(m_desiredPosition == m_demoCube->getPos())
                SelectDrone(1);
        }

        if(c_eventType == 4)
        {
            m_GUI_ptr->CleanGUI();
            soundManager->SetEventVariableValue(m_demoCube, "Voices", "Character", m_selectedDrone);
            soundManager->Play(m_demoCube, "Voices");

            DroneData l_data = m_dronesToChoose[m_selectedDrone];
            m_selected_PowerUpLEFT = l_data.m_PUPreset/10;
            m_selected_PowerUpRIGHT = l_data.m_PUPreset%10;

            m_actualState = SelectorState::SELECT_POWERUPS;
            
            //Change animation to selection HERE
            CharacterSelected(m_selectedDrone);
            ShowPUMenu();
        }
        if(c_eventType == 5)
        {
            this->ProcessGoBack();
        }

        if(m_clickClock_ptr != nullptr)
        {
            m_clickClock_ptr->Restart();
        }
    }
}

void SelectorMenuState::CharacterSelected(int m_selectedDrone)
{
    if(m_selectedDrone == 0){
        render->SetSecuence(jack->GetId(), "Seleccion");

        render->SetSecuence(vladimir->GetId(), "Empty");
        render->SetSecuence(karen->GetId(), "Empty");
        render->SetSecuence(lyanna->GetId(), "Empty");
        
    }
    if(m_selectedDrone == 1){
        render->SetSecuence(karen->GetId(), "Seleccion");

        render->SetSecuence(jack->GetId(), "Empty");
        render->SetSecuence(vladimir->GetId(), "Empty");
        render->SetSecuence(lyanna->GetId(), "Empty");

    }
    if(m_selectedDrone == 2){
        render->SetSecuence(lyanna->GetId(), "Seleccion");

        render->SetSecuence(karen->GetId(), "Empty");
        render->SetSecuence(vladimir->GetId(), "Empty");
        render->SetSecuence(jack->GetId(), "Empty");

    }
    if(m_selectedDrone == 3){
        render->SetSecuence(vladimir->GetId(), "Seleccion");

        render->SetSecuence(jack->GetId(), "Empty");
        render->SetSecuence(lyanna->GetId(), "Empty");
        render->SetSecuence(karen->GetId(), "Empty");
    }
}

void SelectorMenuState::ProcessGoBack()
{
    if(m_actualState == SelectorState::SELECT_POWERUPS)
    {
        m_GUI_ptr->CleanGUI();
        CheckCharacter(m_dronesToChoose[m_selectedDrone].m_characterName);
        SetDroneSelectionButtons();
        CheckCharacter(m_dronesToChoose[m_selectedDrone].m_characterName);
        m_actualState = SelectorState::SELECT_DRONE;
    }
    else
    {
        this->GoToMainMenu();
    }
}

void SelectorMenuState::RotateDrones()
{   
    glm::vec3 rotation;
    for(auto const& l_entityManaged : m_droneEntitites)
    {
        rotation = l_entityManaged->getRot();
        rotation.z++;
        l_entityManaged->setRot(rotation);
        render->GetRenderFacade()->RotateSceneNode(l_entityManaged->GetId(), l_entityManaged->getRot());
    }
}

void SelectorMenuState::UpdatePerspective()
{
    if(m_actualState == SelectorState::SELECT_DRONE)
    {
        if(m_perspective > 50)
        {
            m_perspective--;
        }
    }
    else if(m_actualState == SelectorState::SELECT_POWERUPS)
    {
        if(m_perspective < 90)
        {
            m_perspective++;
        }
    }

    render->SetPerspectiveAngle(m_perspective);
}

void SelectorMenuState::CheckCharacter(std::string l_characterName)
{
	if(l_characterName.compare("Vladimir") == 0){
        render->SetSecuence(vladimir->GetId(), "Idle");
        render->ScaleSceneNode(vladimir, 0.6f);

        render->SetSecuence(jack->GetId(), "Empty");
        render->SetSecuence(karen->GetId(), "Empty");
        render->SetSecuence(lyanna->GetId(), "Empty");
        
    }
    if(l_characterName.compare("Karen") == 0){
        // std::cout << "Entro a Karen" << std::endl;
        render->SetSecuence(karen->GetId(), "Idle");
        render->ScaleSceneNode(karen, 0.5f);

        render->SetSecuence(jack->GetId(), "Empty");
        render->SetSecuence(vladimir->GetId(), "Empty");
        render->SetSecuence(lyanna->GetId(), "Empty");

    }
    if(l_characterName.compare("Jack") == 0){
        render->SetSecuence(jack->GetId(), "Idle");
        render->ScaleSceneNode(jack, 0.5f);

        render->SetSecuence(karen->GetId(), "Empty");
        render->SetSecuence(vladimir->GetId(), "Empty");
        render->SetSecuence(lyanna->GetId(), "Empty");

    }
    if(l_characterName.compare("Lyanna") == 0){
        render->SetSecuence(lyanna->GetId(), "Idle");
        render->ScaleSceneNode(lyanna, 0.5f);

        render->SetSecuence(jack->GetId(), "Empty");
        render->SetSecuence(vladimir->GetId(), "Empty");
        render->SetSecuence(karen->GetId(), "Empty");
    }
}

void SelectorMenuState::SelectDrone(int c_leftOrRight)
{
    m_selectedDrone += c_leftOrRight;
    if(m_selectedDrone < 0)
    {
        m_selectedDrone = 0;
        // std::cout<<"Drone chosen: "<< m_dronesToChoose[m_selectedDrone].m_characterName <<std::endl;
        CheckCharacter(m_dronesToChoose[m_selectedDrone].m_characterName);
        return;
    }
    if(m_selectedDrone == (int)m_dronesToChoose.size())
    {
        m_selectedDrone = m_dronesToChoose.size() - 1;
        // std::cout<<"Drone chosen: "<< m_dronesToChoose[m_selectedDrone].m_characterName <<std::endl;
        CheckCharacter(m_dronesToChoose[m_selectedDrone].m_characterName);
        return;
    }

    m_desiredPosition = m_droneEntitites[m_selectedDrone]->getPos();

    // std::cout<<"Drone chosen: "<< m_dronesToChoose[m_selectedDrone].m_characterName <<std::endl;
    CheckCharacter(m_dronesToChoose[m_selectedDrone].m_characterName);

    soundManager->SetEventVariableValue("MenuSounds", "Interaction", 0.5);
}

void SelectorMenuState::MoveTarget()
{

    if(m_desiredPosition != m_demoCube->getPos())
    {
        if(m_increment == glm::vec3{-1,-1,-1})
        {
            m_increment = (m_desiredPosition - m_demoCube->getPos()) / 25.0f;
        }
        
        m_billboardPos = m_billboardPos + m_increment;
        render->GetRenderFacade()->TranslateBillboard(m_demoCube->GetId(), m_billboardPos);

        m_demoCube->setPos( m_demoCube->getPos() + m_increment);
        
        cameraManager->CalculateMenuCameraData();
    }
    else
    {
        m_increment = {-1,-1,-1};
    }      
    
}

void SelectorMenuState::CloseState()
{
    delete m_clickClock_ptr;
    delete m_GUI_ptr;
    
    render->Reset();
    entityManager->ClearList();
    inputManager->ClearList();
    soundManager->ClearList();
    cameraManager->ClearList();
    soundManager->UpdateSoundEngine();
    m_physicsManager->ClearList();
    
    //Cleaning vectors. IDK why I have to swap them to not have memory leaks. If I do clear(), doesn't work.
    ClearVectors();

    vladimir = nullptr;
    karen = nullptr;
    jack = nullptr;
    lyanna = nullptr;
    //mansion = nullptr;
}

void InitGame(Event)
{
    StatesMachine::getInstance()->GetActiveState()->ChangeState();
}

void RightPowerUpHandle(Event event)
{
    StatesMachine::getInstance()->GetActiveState()->ProcessGUIEvent(0, event.GUIAction);
}
void LeftPowerUpHandle(Event event)
{
    StatesMachine::getInstance()->GetActiveState()->ProcessGUIEvent(1, event.GUIAction);
}

void SwitchLeftDrone(Event event)
{
    StatesMachine::getInstance()->GetActiveState()->ProcessGUIEvent(2, -1);
}

void SwitchRightDrone(Event event)
{
    StatesMachine::getInstance()->GetActiveState()->ProcessGUIEvent(3, -1);
}

void GoToPUSelection(Event event)
{
    StatesMachine::getInstance()->GetActiveState()->ProcessGUIEvent(4, -1);
}

void GoBack(Event event)
{
    StatesMachine::getInstance()->GetActiveState()->ProcessGUIEvent(5,-1);
}


