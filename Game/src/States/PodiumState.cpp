#include <RenderManager.h>
#include <EntityManager.h>
#include <InputManager.hpp>
#include <SoundManager.h>
#include <CameraManager.hpp>
#include <Entity.h>
#include <PodiumState.hpp>
#include <RaceState.hpp>
#include <SelectorMenuState.hpp>
#include <MainMenuState.hpp>
#include <LobbyState.hpp>
#include <GenericData.hpp>
#include <EventManager.h>
#include <PositionManager.hpp>
#include <GUI.hpp>
#include <iostream>

void ExitPodiumState(Event);

PodiumState::PodiumState(GenericData* c_gameData, std::string& c_playerName)
{
   entityManager = c_gameData->g_entityManager_ptr;
   render = c_gameData->g_renderManager_ptr;
   inputManager = c_gameData->g_inputManager_ptr;
   soundManager = c_gameData->g_soundManager_ptr;
   cameraManager = c_gameData->g_cameraManager_ptr;
   m_eventManager_ptr = c_gameData->g_eventManager_ptr;
   positionManager  = c_gameData->g_positionManager_ptr;

   m_playerCharacterName = c_playerName;
   
   m_gameData = c_gameData;
   m_GUI_ptr = new GUI(m_gameData);

   jack = nullptr;
   lyanna = nullptr;
   karen = nullptr;
   vladimir = nullptr;
   podium = nullptr;
}

PodiumState::~PodiumState()
{
}

void PodiumState::Init()
{   
    m_demoCube = entityManager->CreateEntity("DemoCube",glm::vec3(0,0,0));
    podium = entityManager->CreateEntity("Podium", glm::vec3(0,-5,-5.5));
    render->CreateComponent(podium,"Animated","Podium");
    render->AddNode(podium);
    
    render->CreateComponent(m_demoCube, "Cube", "");
    render->AddNode(m_demoCube);
	//soundManager->CreateComponent(player, "Drone", "event:/DroneSounds/Drone", 1);
	inputManager->CreateInputComponent(m_demoCube);
    cameraManager->SetTarget(m_demoCube);
    render->GetRenderFacade()->AddLight(glm::vec3(0,100,0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 0.0000000000014, 0.000000000000000007));

 //   m_GUI_ptr->SetBackground("sun_left.jpg");
    //guiManager->CreateMenuElement(m_demoCube, "Title", "./assets/git_photo.jpg", 0, 0, 200, 200);
    soundManager->CreateComponent(m_demoCube, "PositionEvent", "event:/InterfaceEvents/PodiumState", 0);

    m_GUI_ptr->AddGUIButton(630, 600, 240, 80, EventType::ExitPodium, 0,"exit_not_hovered.png","exit_hovered.png");
    
    m_eventManager_ptr->addListener(EventManager::Listener{ExitPodiumState}, EventType::ExitPodium);

    m_finalResults_v = positionManager->GetFinalResults();

    int number = 1;
    m_playerPosition = 0;
    m_characterFirst = 0;

    for(std::string& content : *m_finalResults_v)
    {
        std::string message = std::to_string(number) + "-" + content;
        render->GetRenderFacade()->GUICreateText(number, 150, (650 - 65* number), 1.0, message, "assets/fonts/BigNoodleTooOblique.ttf");

        if(content.compare(m_playerCharacterName) == 0)
            m_playerPosition = number - 1;
        
        if(number == 1)
        {
            if(content.compare("Jack") == 0)
                m_characterFirst = 0;
            if(content.compare("Karen") == 0)
                m_characterFirst = 1;
            if(content.compare("Lyanna") == 0)
                m_characterFirst = 2;
            if(content.compare("Vladimir") == 0)
                m_characterFirst = 3;
        }

        number ++;
    }

    int pos = 0;
    for(std::string& loquesea : *m_finalResults_v)
    {
        //First position
        if(pos == 0)
        {
            if(loquesea.compare("Karen") == 0){
                PlayKarenVictory();  
            }
            if(loquesea.compare("Lyanna") == 0){
                 PlayLyannaVictory();
            }
            if(loquesea.compare("Vladimir") == 0){
                PlayVladimirVictory();  
            }
            if(loquesea.compare("Jack") == 0){
                PlayJackVictory();
            }
        }

        //Second position
        if(pos == 1)
        {
            if(loquesea.compare("Karen") == 0){
                PlayKarenAplausos();  
            }
            if(loquesea.compare("Lyanna") == 0){
                PlayLyannaAplausos(); 
            }
            if(loquesea.compare("Vladimir") == 0){
                PlayVladimirAplausos(); 
            }
            if(loquesea.compare("Jack") == 0){
                PlayJackAplausos();
            }
        }

        //Third position
        if(pos == 2)
        {
            if(loquesea.compare("Karen") == 0){
                PlayKarenLoose();  
            }
            if(loquesea.compare("Lyanna") == 0){
                PlayLyannaLoose();  
            }
            if(loquesea.compare("Vladimir") == 0){
                PlayVladimirLoose();
            }
            if(loquesea.compare("Jack") == 0){
                PlayJackLoose();
            }
        }

        pos++;
    }

    render->AddAnimatedSecuence(podium, "Podio", {"podium"}, true);
	render->SetSecuence(podium->GetId(), "Podio");
    render->AddTexture(podium, "podium_texture.png", "Podio");


    soundManager->SetEventVariableValue(m_demoCube, "PositionEvent", "Winner", m_characterFirst);
    soundManager->SetEventVariableValue(m_demoCube, "PositionEvent", "PlayerPosition", m_playerPosition);

    soundManager->PlayAllSounds();

    soundManager->CreateComponent(m_demoCube, "MenuSounds", "event:/InterfaceEvents/MenuButtons", 0);

    //STATIC MENU CAMERA
    cameraManager->SetTarget(m_demoCube);
    cameraManager->InitCameraData(3); // 3 = Menu Camera
    cameraManager->CalculateMenuCameraData();
}

void PodiumState::Update(float &delta)
{
    soundManager->UpdateSoundEngine();
    m_GUI_ptr->Update();
    m_eventManager_ptr->update();
}

void PodiumState::Draw()
{
    if(m_demoCube)
        render->RenderScene(m_demoCube);
}

void PodiumState::ChangeState()
{
    delete m_GUI_ptr;

    render->Reset();
    entityManager->ClearList();
    inputManager->ClearList();
    soundManager->ClearList();
    cameraManager->ClearList();
    soundManager->UpdateSoundEngine();

    m_demoCube = nullptr;
    m_finalResults_v->clear();

    m_finalResults_v = nullptr;
    m_GUI_ptr        = nullptr;
    vladimir         = nullptr;
    karen            = nullptr;
    jack             = nullptr;
    lyanna           = nullptr;
    podium           = nullptr;

    StatesMachine::getInstance()->AddState(std::make_unique<MainMenuState>(m_gameData));
    
}

void PodiumState::CloseState()
{
    delete m_GUI_ptr;
    m_GUI_ptr = nullptr;

    m_demoCube = nullptr;
    m_finalResults_v->clear();
}

void ExitPodiumState(Event)
{
    StatesMachine::getInstance()->GetActiveState()->ChangeState();
}

void PodiumState::PlayVladimirVictory()
{
    vladimir = entityManager->CreateEntity("Vladimir", glm::vec3(0,-5,-0.5));
    render->CreateComponent(vladimir, "Animated", "Vladimir");
    render->AddNode(vladimir);
    render->ScaleSceneNode(vladimir,0.5f);
	render->AddAnimatedSecuence(vladimir, "Victoria", {"dab1", "dab2", "dab3", "dab4", "dab5", "dab6", "dab7", "dab9"
    , "dab10", "dab11", "dab12", "dab13", "dab14", "dab15", "dab16"}, true);
    render->AddTexture(vladimir, "vladimir_texture.png", "Victoria");
	render->SetSecuence(vladimir->GetId(), "Victoria");
    render->GetRenderFacade()->SetAnimationFrameRate(vladimir->GetId(), (2.0f/2.0f));
}

void PodiumState::PlayVladimirAplausos()

{
    vladimir = entityManager->CreateEntity("Vladimir", glm::vec3(-2.8,-5,-2.3));
    render->CreateComponent(vladimir, "Animated", "Vladimir");
    render->AddNode(vladimir);
    render->ScaleSceneNode(vladimir,0.5f);
    render->AddAnimatedSecuence(vladimir, "Aplausos", {"aplausos1", "aplausos2", "aplausos3", "aplausos4",
    "aplausos5", "aplausos6", "aplausos7", "aplausos8", "aplausos9", "aplausos10", "aplausos11", "aplausos12",
    "aplausos13", "aplausos14", "aplausos15", "aplausos16", "aplausos17"}, true);
    render->SetSecuence(vladimir->GetId(), "Aplausos");
    render->GetRenderFacade()->SetAnimationFrameRate(vladimir->GetId(), (2.5f/2.0f));
	render->AddTexture(vladimir, "vladimir_texture.png", "Aplausos");
}

void PodiumState::PlayVladimirLoose()
{
    vladimir = entityManager->CreateEntity("Vladimir", glm::vec3(2,-5,-3.8));
    render->CreateComponent(vladimir, "Animated", "Vladimir");
    render->AddNode(vladimir);
    render->ScaleSceneNode(vladimir,0.5f);
     render->AddAnimatedSecuence(vladimir, "Perder", {"patadita2", "patadita3", "patadita4",
    "patadita5", "patadita6", "patadita7", "patadita9", "patadita10", "patadita11", "patadita12",
    "patadita13", "patadita14", "patadita15", "patadita16"}, true);
    render->SetSecuence(vladimir->GetId(), "Perder");
    render->GetRenderFacade()->SetAnimationFrameRate(vladimir->GetId(), (2.0f/2.0f));
	render->AddTexture(vladimir, "vladimir_texture.png", "Perder");
}

void PodiumState::PlayKarenVictory()
{

    karen = entityManager->CreateEntity("Karen", glm::vec3(0,-5,0.3));
    render->CreateComponent(karen, "Animated", "Karen");
    render->AddNode(karen);
    render->ScaleSceneNode(karen,0.5f);
    render->AddAnimatedSecuence(karen, "Victoria", {"seleccion2_karen", "seleccion3_karen", "seleccion4_karen",
    "seleccion5_karen", "seleccion6_karen", "seleccion7_karen", "seleccion8_karen", "seleccion9_karen", "seleccion10_karen", "seleccion11_karen",
    "seleccion12_karen", "seleccion13_karen", "seleccion14_karen", "seleccion15_karen", "seleccion16_karen"}, true);
    render->SetSecuence(karen->GetId(), "Victoria");
    render->GetRenderFacade()->SetAnimationFrameRate(karen->GetId(), (2.0f/2.0f));
	render->AddTexture(karen, "karen_texture.png", "Victoria");
}

void PodiumState::PlayKarenAplausos()
{

    karen = entityManager->CreateEntity("Karen", glm::vec3(-4.7,-6.5,-5));
    render->CreateComponent(karen, "Animated", "Karen");
    render->AddNode(karen);
    render->ScaleSceneNode(karen,0.5f);
    render->AddAnimatedSecuence(karen, "Aplausos", {"aplausos2_karen", "aplausos3_karen", "aplausos4_karen", "aplausos5_karen",
     "aplausos6_karen", "aplausos7_karen", "aplausos8_karen", "aplausos9_karen", "aplausos10_karen", "aplausos11_karen", "aplausos12_karen",
     "aplausos13_karen", "aplausos14_karen", "aplausos15_karen", "aplausos16_karen"}, true);
    render->SetSecuence(karen->GetId(), "Aplausos");
    render->GetRenderFacade()->SetAnimationFrameRate(karen->GetId(), (2.0f/2.0f));
	render->AddTexture(karen, "karen_texture.png", "Aplausos");
}

void PodiumState::PlayKarenLoose()
{

    karen = entityManager->CreateEntity("Karen", glm::vec3(2,-5,-3.2));
    render->CreateComponent(karen, "Animated", "Karen");
    render->AddNode(karen);
    render->ScaleSceneNode(karen,0.5f);
    render->AddAnimatedSecuence(karen, "Perder", {"karen_patada1", "karen_patada2", "karen_patada3", "karen_patada4", "karen_patada5",
    "karen_patada6", "karen_patada7", "karen_patada8", "karen_patada9", "karen_patada10", "karen_patada11", "karen_patada12",
    "karen_patada13", "karen_patada14", "karen_patada16"}, true);
    render->SetSecuence(karen->GetId(), "Perder");
    render->GetRenderFacade()->SetAnimationFrameRate(karen->GetId(), (2.0f/2.0f));
	render->AddTexture(karen, "karen_texture.png", "Perder");
}

void PodiumState::PlayJackVictory()
{

    jack = entityManager->CreateEntity("Jack", glm::vec3(0,-5,0.3));
    render->CreateComponent(jack, "Animated", "Jack");
    render->AddNode(jack);
    render->ScaleSceneNode(jack,0.5f);
    render->AddAnimatedSecuence(jack, "Salto", {"celebracion2_jack", "celebracion3_jack", "celebracion4_jack", "celebracion5_jack", "celebracion6_jack", "celebracion7_jack", "celebracion8_jack", 
     "celebracion9_jack", "celebracion10_jack", "celebracion11_jack", "celebracion12_jack", "celebracion13_jack", "celebracion14_jack", "celebracion15_jack", "celebracion16_jack"}, true);
    render->SetSecuence(jack->GetId(), "Salto");
    render->GetRenderFacade()->SetAnimationFrameRate(jack->GetId(), (2.0f/2.0f));
	render->AddTexture(jack, "jack_texture.png", "Salto");
}

void PodiumState::PlayJackAplausos()
{

    jack = entityManager->CreateEntity("Jack", glm::vec3(-2.8,-5,-1));
    render->CreateComponent(jack, "Animated", "Jack");
    render->AddNode(jack);
    render->ScaleSceneNode(jack,0.5f);
    render->AddAnimatedSecuence(jack, "Aplausos", {"aplausos2_jack", "aplausos3_jack", "aplausos4_jack", "aplausos5_jack",
     "aplausos6_jack", "aplausos7_jack", "aplausos8_jack", "aplausos9_jack", "aplausos10_jack", "aplausos11_jack", "aplausos12_jack",
     "aplausos13_jack", "aplausos14_jack", "aplausos15_jack", "aplausos16_jack"}, true);
    render->SetSecuence(jack->GetId(), "Aplausos");
    render->GetRenderFacade()->SetAnimationFrameRate(jack->GetId(), (2.0f/2.0f));
	render->AddTexture(jack, "jack_texture.png", "Aplausos");
}

void PodiumState::PlayJackLoose()
{

    jack = entityManager->CreateEntity("Jack", glm::vec3(2.2,-5,-2.5));
    render->CreateComponent(jack, "Animated", "Jack");
    render->AddNode(jack);
    render->ScaleSceneNode(jack,0.5f);
    render->AddAnimatedSecuence(jack, "Malcriado", {"perder2_jack", "perder3_jack", "perder4_jack", "perder5_jack", "perder6_jack", "perder7_jack",
    "perder8_jack", "perder9_jack", "perder10_jack", "perder11_jack", "perder12_jack", "perder13_jack", "perder14_jack", "perder15_jack", "perder16_jack"}, true);
    render->SetSecuence(jack->GetId(), "Malcriado");
    render->GetRenderFacade()->SetAnimationFrameRate(jack->GetId(), (2.0f/2.0f));
	render->AddTexture(jack, "jack_texture.png", "Malcriado");
}

void PodiumState::PlayLyannaVictory()
{
    lyanna = entityManager->CreateEntity("Lyanna", glm::vec3(0,-5,0));
    render->CreateComponent(lyanna, "Animated", "Lyanna");
    render->AddNode(lyanna);
    render->ScaleSceneNode(lyanna,0.5f);
    render->AddAnimatedSecuence(lyanna, "Breakdance", {"breakdance2", "breakdance3", "breakdance4", "breakdance5", "breakdance6",
     "breakdance7", "breakdance8", "breakdance9", "breakdance10", "breakdance11", "breakdance12", "breakdance15",
     "breakdance16", "breakdance17", "breakdance18"}, true);
    render->SetSecuence(lyanna->GetId(), "Breakdance");
    render->GetRenderFacade()->SetAnimationFrameRate(lyanna->GetId(), (2.0f/2.0f));
	render->AddTexture(lyanna, "lyanna_texture.png", "Breakdance");
}

void PodiumState::PlayLyannaAplausos()
{
    lyanna = entityManager->CreateEntity("Lyanna", glm::vec3(-2.8,-5,-1.9));
    render->CreateComponent(lyanna, "Animated", "Lyanna");
    render->AddNode(lyanna);
    render->ScaleSceneNode(lyanna,0.5f);
    render->AddAnimatedSecuence(lyanna, "Aplausos", {"aplausos1_lyanna", "aplausos2_lyanna", "aplausos3_lyanna", "aplausos4_lyanna", "aplausos5_lyanna",
     "aplausos6_lyanna", "aplausos7_lyanna", "aplausos8_lyanna", "aplausos9_lyanna", "aplausos10_lyanna", "aplausos11_lyanna", "aplausos12_lyanna",
      "aplausos13_lyanna", "aplausos14_lyanna", "aplausos15_lyanna", "aplausos16_lyanna"}, true);
    render->SetSecuence(lyanna->GetId(), "Aplausos");
    render->GetRenderFacade()->SetAnimationFrameRate(lyanna->GetId(), (2.0f/2.0f));
	render->AddTexture(lyanna, "lyanna_texture.png", "Aplausos");
}

void PodiumState::PlayLyannaLoose()
{
    lyanna = entityManager->CreateEntity("Lyanna", glm::vec3(2,-5,-2.5));
    render->CreateComponent(lyanna, "Animated", "Lyanna");
    render->AddNode(lyanna);
    render->ScaleSceneNode(lyanna,0.5f);
    render->AddAnimatedSecuence(lyanna, "Perder", {"patadita1_lyanna", "patadita2_lyanna", "patadita3_lyanna", "patadita4_lyanna", "patadita5_lyanna", "patadita6_lyanna", "patadita7_lyanna",
     "patadita8_lyanna", "patadita9_lyanna", "patadita10_lyanna", "patadita11_lyanna", "patadita12_lyanna", "patadita13_lyanna", "patadita14_lyanna", "patadita15_lyanna", "patadita16_lyanna"}, true);
    render->SetSecuence(lyanna->GetId(), "Perder");
    render->GetRenderFacade()->SetAnimationFrameRate(lyanna->GetId(), (2.0f/2.0f));
	render->AddTexture(lyanna, "lyanna_texture.png", "Perder");
}