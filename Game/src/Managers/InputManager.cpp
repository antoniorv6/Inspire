/**
 * @file InputManager.cpp
 * @author Edward Owen (contact@edwardowen.es)
 * 
 * @brief: InputManager's definition, whick is going to handle
 * all the Keyboard and Mouse event, using an InputFacade to
 * do this through Irrlicht Engine or OpenGL
 * 
 * @version 0.1
 * @date 2018-11-12
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include "InputManager.hpp"
#include "InputIrrlicht.hpp"
#include <InputGenesis.hpp>
#include <StatesMachine.hpp>
#include <RenderManager.h>
#include <CameraManager.hpp>
#include <EntityManager.h>
#include <PowerUpManager.hpp>
#include <CommonData.hpp>

/**
 * InputManager::InputManager
 * 
 * @brief: creates the corresponding InputFacade (Irrlicht or OpenGL) to handle
 * Keyboard and Mouse events. Also init the Key Codes vector using the Key Codes
 * values given by the InputFacade
 * 
 * @param l_engine (Irrlicht = 1, OpenGL = 0)
 */
InputManager::InputManager(int l_engine) : m_engine_n(l_engine) 
{
    switch(m_engine_n)
    {
        case 0: m_inputFacade = new InputGenesis(); break;
        case 1: m_inputFacade = new InputIrrlicht(); break;
    };

    SetGameKeyCodes();
    SetGamepadButtonsCodes();
    SetGamepadAxisCodes();

    m_inputFacade->SetUpKeyIdentifiers(keys);
    m_inputFacade->SetUpButtonIdentifiers(buttons);
    m_inputFacade->SetUpAxisIdentifiers(axis);
}

void InputManager::Init(int l_engine_n, GenericData *& c_genericData)
{

    m_entityManager_ptr   = c_genericData->g_entityManager_ptr;
    m_physicManager_ptr   = c_genericData->g_physicsManager_ptr;
    m_powerUpManager_ptr  = c_genericData->g_powerUpManager_ptr;
    m_renderManager_ptr   = c_genericData->g_renderManager_ptr;
    m_cameraManager_ptr   = c_genericData->g_cameraManager_ptr;
    m_renderUtilities_ptr = RenderUtilities::getInstance();

    if(l_engine_n == 0)
    {
        InputGenesis * inputGenesis = reinterpret_cast<InputGenesis*>(m_inputFacade);
        inputGenesis->Init(c_genericData);
    }
}


/**
 * InputManager::~InputManager
 * 
 * @brief Destroy the Input Manager and InputFacade
 * 
 */
InputManager::~InputManager()
{
    delete m_inputFacade;
    m_inputFacade = nullptr;
    ClearList();
}

void InputManager::ClearList()
{
    for(unsigned int i = 0; i < m_inputComponents_v.size(); i++)
     {
        delete m_inputComponents_v[i];
        m_inputComponents_v[i] = nullptr;
     }

    m_gameKeyCodes_u32v.clear();
    m_gamepadAxisCodes_u32v.clear();
    m_gamepadButtonCodes_u32v.clear();
    m_inputComponents_v.clear();
}

/**
 * InputManager::CreateInputComponent
 * 
 * @brief: Creates an Input Component and inserts it into the
 * Input Components vector of the manager.
 * 
 * @param l_entity_o 
 */
void InputManager::CreateInputComponent(Entity* l_entity_o){
    InputComponent* l_inputComponent_o = new InputComponent("Input", l_entity_o->GetId());
    m_inputComponents_v.push_back(l_inputComponent_o);
    m_entityManager_ptr->AddComponent(l_entity_o->GetId(), (IComponent*)l_inputComponent_o);
}

/**
 * InputManager::Update
 * 
 * @brief: Checks Keyboard and Mouse events and pass its states to the physics
 * 
 */

void InputManager::Update()
{
    if(m_engine_n == 0)
        ((InputGenesis*)m_inputFacade)->UpdateGamepadValues();

    for(unsigned int i = 0; i < m_inputComponents_v.size(); i--)
    {
        if(m_inputComponents_v[i]!=NULL)
        {
            int l_entityId_n = m_inputComponents_v[i]->GetEntityId();

            m_physicManager_ptr->UpdateKeys
            (
                KeyOrButton(keys.W, buttons.A),       //alante
                KeyOrAxis(keys.A, axis.LJLR, -1),     //izquierda
                KeyOrButton(keys.S, buttons.B),       //atras
                KeyOrAxis(keys.D , axis.LJLR, 1),     //derecha
                KeyOrTrigger(keys.K, axis.L2),        //abajo
                KeyOrTrigger(keys.I, axis.R2),        //arriba
                KeyOrButton(keys.SPACE, buttons.X),   //nitro
                l_entityId_n
            );
            if(m_cont_n > 1)
            {
                if( KeyOrButton(keys.J, buttons.L1) ) //POWER UP 1
                    m_powerUpManager_ptr->CreatePUEvent(l_entityId_n, 0);
                
                if( KeyOrButton(keys.L, buttons.R1) ) //POWER UP 2
                    m_powerUpManager_ptr->CreatePUEvent(l_entityId_n, 1);
                

                if( KeyOrButton(keys.H, buttons.L3) ) //REAR CAMERA
                    m_cameraManager_ptr->SetActiveCamera(2);
                else
                    m_cameraManager_ptr->SetActiveCamera(1);
                
                m_cont_n = 0;
            }
        }
    }
    
    if( KeyOrButton(keys.F8, buttons.Back) )
    {
        m_renderManager_ptr->SetDebugMode();
    }
    
    if( KeyOrButton(keys.ESCAPE, buttons.Start) )
    {
        StatesMachine::getInstance()->GetActiveState()->Pause();
    }

    if(m_cont_n <= 1)
        m_cont_n++;
}

void InputManager::Update(std::vector<bool>&inputsClient)
{
    inputsClient[0] = m_inputFacade->IsKeyPressed(m_gameKeyCodes_u32v[0]);
    inputsClient[1] = m_inputFacade->IsKeyPressed(m_gameKeyCodes_u32v[1]);
    inputsClient[2] = m_inputFacade->IsKeyPressed(m_gameKeyCodes_u32v[2]);
    inputsClient[3] = m_inputFacade->IsKeyPressed(m_gameKeyCodes_u32v[3]);
    inputsClient[4] = m_inputFacade->IsKeyPressed(m_gameKeyCodes_u32v[4]);
    inputsClient[5] = m_inputFacade->IsKeyPressed(m_gameKeyCodes_u32v[5]);
    inputsClient[6] = m_inputFacade->IsKeyPressed(m_gameKeyCodes_u32v[7]);

    /*Power ups*/
    inputsClient[7] = m_inputFacade->IsKeyPressed(m_gameKeyCodes_u32v[9]);
    inputsClient[8] = m_inputFacade->IsKeyPressed(m_gameKeyCodes_u32v[10]);
}

/**
 * InputManager::SetGameKeyCodes
 * 
 * @brief: Initialize the Key Codes vector with the Keys and Buttons
 * of the corresponding Engine (Irrlicht)
 * 
 */
void InputManager::SetGameKeyCodes()
{
    m_gameKeyCodes_u32v.reserve(11);
    m_gameKeyCodes_u32v = m_inputFacade->GetGameKeyCodes();
}

/**
 * @brief 
 * 
 */
void InputManager::SetGamepadButtonsCodes()
{
    m_gamepadButtonCodes_u32v.reserve(8);
    m_gamepadButtonCodes_u32v = m_inputFacade->GetGamepadButtonCodes();
}

/**
 * @brief 
 * 
 */
void InputManager::SetGamepadAxisCodes()
{
    m_gamepadAxisCodes_u32v.reserve(6);
    m_gamepadAxisCodes_u32v = m_inputFacade->GetGamepadAxisCodes();
}

/**
 * @brief 
 * 
 * @param id_key 
 * @param id_button 
 * @return float 
 */
float InputManager::KeyOrButton(unsigned int id_key, unsigned int id_button)
{
    return std::max(m_inputFacade->IsKeyPressed(m_gameKeyCodes_u32v[id_key]),
                    m_inputFacade->IsButtonPressed(m_gamepadButtonCodes_u32v[id_button]));
}

/**
 * @brief 
 * 
 * @param id_key 
 * @param id_trigger 
 * @return float 
 */
float InputManager::KeyOrTrigger(unsigned int id_key, unsigned int id_trigger)
{
    return std::max(m_inputFacade->IsKeyPressed(m_gameKeyCodes_u32v[id_key]),
                    m_inputFacade->IsAxisPressed(m_gamepadAxisCodes_u32v[id_trigger]));
}

/**
 * @brief 
 * 
 * @param id_key 
 * @param id_axis 
 * @param sign 
 * @return float 
 */
float InputManager::KeyOrAxis(unsigned int id_key, unsigned int id_axis, int sign)
{
    if(sign == -1)
    {
        float axis = m_inputFacade->IsAxisPressed(m_gamepadAxisCodes_u32v[id_axis])* -1.0f;
        return std::max(m_inputFacade->IsKeyPressed(m_gameKeyCodes_u32v[id_key]), axis);
    }
    else
    {
        return std::max(m_inputFacade->IsKeyPressed(m_gameKeyCodes_u32v[id_key]), 
                        m_inputFacade->IsAxisPressed(m_gamepadAxisCodes_u32v[id_axis]));
    }
    
}

int InputManager::GetMousePositionX()
{
    return m_inputFacade->GetMouseXPos();
}

int InputManager::GetMousePosityonY()
{
    return m_inputFacade->GetMouseYPos();
}

bool InputManager::IsMouseClicked()
{
    return m_inputFacade->IsMousePressed();
}

void InputManager::SetMouseClick(bool value)
{
    m_inputFacade->SetMouseClick(value);
}