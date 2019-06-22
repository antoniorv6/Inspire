/**
 * @file InputManager.hpp
 * @author Edward Owen (contact@edwardowen.es)
 * 
 * @brief: InputManager's declaration, whick is going to handle
 * all the Keyboard and Mouse event, using an InputFacade to
 * do this through Irrlicht Engine or OpenGL
 * 
 * @version 0.1
 * @date 2018-11-12
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#pragma once
#include "IInputFacade.hpp"
#include "InputComponent.hpp"
#include <vector>
#include <iostream>
#include <algorithm>

class RenderManager;
class GenericData;
class CameraManager;
class EntityManager;
class RenderManager;
class PowerUpManager;
class PhysicsManager;
class RenderUtilities;
class Entity;

class InputManager
{
public:
    InputManager(int l_engine);
    ~InputManager();

    void Init(int l_engine_n, GenericData *& c_genericData);
    void ClearList();
    void Update();
    void Update(std::vector<bool>&inputsClient);
    void SetGameKeyCodes();
    void SetGamepadButtonsCodes();
    void SetGamepadAxisCodes();
    void CreateInputComponent(Entity* l_entity_o);
    float KeyOrButton(unsigned int id_key, unsigned int id_button);
    float KeyOrAxis(unsigned int id_key, unsigned int id_axis, int sign);
    float KeyOrTrigger(unsigned int id_key, unsigned int id_trigger);

    int GetMousePositionX();
    int GetMousePosityonY();
    bool IsMouseClicked();
    void SetMouseClick(bool value);

    /**
     * @brief Get the Facade object
     * 
     * @return IInputFacade* 
     */
    IInputFacade* GetFacade() const
    {
        return m_inputFacade;
    };

private:
    IInputFacade* m_inputFacade = nullptr;
    
    KeyIdentifiers    keys;
    ButtonIdentifiers buttons;
    AxisIdentifiers   axis;
    
    //Vector of the avaiable Key Codes to use
    std::vector<unsigned int> m_gameKeyCodes_u32v;
    //Vector of the avaiable Buttons Codes to use
    std::vector<unsigned int> m_gamepadButtonCodes_u32v;
    //Vector of the avaiable Axis Codes to use
    std::vector<unsigned int> m_gamepadAxisCodes_u32v;

    //Vector of Input Components
    std::vector <InputComponent*> m_inputComponents_v;
    //Cont
    int m_cont_n = 0;

    //Engine
    int m_engine_n = 0;

    EntityManager   * m_entityManager_ptr   = nullptr;
    RenderManager   * m_renderManager_ptr   = nullptr; 
    PhysicsManager  * m_physicManager_ptr   = nullptr;
    PowerUpManager  * m_powerUpManager_ptr  = nullptr; 
    CameraManager   * m_cameraManager_ptr   = nullptr;
    RenderUtilities * m_renderUtilities_ptr = nullptr;
};

/**
 * @brief // TODO
 * 
 */
struct KeyCode2Function{
    unsigned int keyCode;
    void (*ptr2Function)();
};