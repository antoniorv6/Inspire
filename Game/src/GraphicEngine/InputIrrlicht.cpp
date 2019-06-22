/**
 * @file InputIrrlicht.cpp
 * @author Edward Owen (contact@edwardowen.es)
 * 
 * @brief: Irrlicht definition of the IInputFacade interface
 * 
 * @version 0.1
 * @date 2018-11-12
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include <InputIrrlicht.hpp>

/**
 * @brief Construct a new Input Irrlicht:: Input Irrlicht object
 * 
 */
InputIrrlicht::InputIrrlicht()
{
    //Initialize the current status of the Keyboard to FALSE
    for(int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++)
    {
        m_keysStatus_bv[i] = false;
    }

    //Initialize the values of the Keyboard and Mouse keys/buttons codes
    InitGameKeyCodes();
    InitGamepadAxisCodes();
    InitGamepadButtonCodes();
}

/**
 * @brief Destroy the Input Irrlicht:: Input Irrlicht object
 * 
 */
InputIrrlicht::~InputIrrlicht()
{
    m_gameKeyCodes_u32v.clear();
    m_gamepadAxisCodes_u32v.clear();
    m_gamepadButtonCodes_u32v.clear();
}

/**
 * InputIrrlicht::OnEvent
 * 
 * @brief: Neccesary implementation of the Irrlicht's OnEvent function
 * to handle the different types of events and update the status of the keys
 * 
 * @param event 
 * @return true  (pressed)
 * @return false (not pressed)
 */
bool InputIrrlicht::OnEvent(const irr::SEvent& event)
{
    // Remember whether each key is down or up
    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
        m_keysStatus_bv[event.KeyInput.Key] = (float)event.KeyInput.PressedDown;
    
     
    if(event.EventType == irr::EET_JOYSTICK_INPUT_EVENT && event.JoystickEvent.Joystick == 1)
    {
        
        for(unsigned int i = 0; i < 8; i++)
        {
            m_buttonsStatus_bv[i] = (float)event.JoystickEvent.IsButtonPressed(i);
            //std::cout<<"Button " << i << ": " << m_buttonsStatus_bv[i] << std::endl;

            if( i < 6)
            {
                m_axisStatus_bv[i] = event.JoystickEvent.Axis[i]/32767.0f;
                //std::cout<<"Axis " << i << ": " << m_axisStatus_bv[i] << std::endl;
            }
        }

       //std::cout << "\n" << std::endl;
       //std::cout<<event.JoystickEvent.Axis[irr::SEvent::SJoystickEvent::AXIS_Y]/32767.f<<std::endl;
    }
    
    IrrIMGUI::CIMGUIEventReceiver::OnEvent(event);
    
    return false;
}

/**
 * InputIrrlicht::IsKeyPressed
 * 
 * @brief: returns the current status of a Key, checking it
 * on the Key Codes vector
 * 
 * @param l_keyCode_u32 (Irrlicht Key Code of the Key)
 * @return true         (Pressed)
 * @return false        (Not Pressed)
 */
float InputIrrlicht::IsKeyPressed(unsigned int l_keyCode_u32) const
{
    return m_keysStatus_bv[l_keyCode_u32];
}

/**
 * @brief 
 * 
 * @param l_buttonCode_u32 
 * @return float 
 */
float InputIrrlicht::IsButtonPressed(unsigned int l_buttonCode_u32) const
{
    return m_buttonsStatus_bv[l_buttonCode_u32];
}

/**
 * @brief 
 * 
 * @param l_axisCode_u32 
 * @return float 
 */
float InputIrrlicht::IsAxisPressed(unsigned int l_axisCode_u32) const
{
    return m_axisStatus_bv[l_axisCode_u32];
}

/**
 * InputIrrlicht::GetGameKeyCodes
 * 
 * @brief 
 * 
 * @return std::vector<unsigned int> 
 */
std::vector<unsigned int> InputIrrlicht::GetGameKeyCodes() const 
{
    return m_gameKeyCodes_u32v;
}

std::vector<unsigned int> InputIrrlicht::GetGamepadButtonCodes() const
{
    return m_gamepadButtonCodes_u32v;
}

std::vector<unsigned int> InputIrrlicht::GetGamepadAxisCodes() const
{
    return m_gamepadAxisCodes_u32v;
}

/**
 * InputIrrlicht::InitGameKeyCodes
 * 
 * @brief: Initialize the Game Key Codes with the Irrlicht's keys values.
 * As you can see, with only put in that vector the values that we want, not the value
 * of the whole keyboard table.
 * 
 */
void InputIrrlicht::InitGameKeyCodes()
{
    m_gameKeyCodes_u32v.reserve(12);
    m_gameKeyCodes_u32v.emplace_back(irr::KEY_KEY_W);
    m_gameKeyCodes_u32v.emplace_back(irr::KEY_KEY_A);
    m_gameKeyCodes_u32v.emplace_back(irr::KEY_KEY_S);
    m_gameKeyCodes_u32v.emplace_back(irr::KEY_KEY_D);
    m_gameKeyCodes_u32v.emplace_back(irr::KEY_KEY_K);
    m_gameKeyCodes_u32v.emplace_back(irr::KEY_KEY_I);
    m_gameKeyCodes_u32v.emplace_back(irr::KEY_F8);
    m_gameKeyCodes_u32v.emplace_back(irr::KEY_SPACE);
    m_gameKeyCodes_u32v.emplace_back(irr::KEY_ESCAPE);
    m_gameKeyCodes_u32v.emplace_back(irr::KEY_KEY_J);
    m_gameKeyCodes_u32v.emplace_back(irr::KEY_KEY_L);
    m_gameKeyCodes_u32v.emplace_back(irr::KEY_KEY_H);
}

/**
 * @brief 
 * 
 * @param l_keyI_st 
 */
void InputIrrlicht::SetUpKeyIdentifiers(KeyIdentifiers& l_keyI_st)
{
    l_keyI_st.W      = 0;
    l_keyI_st.A      = 1;
    l_keyI_st.S      = 2;
    l_keyI_st.D      = 3;
    l_keyI_st.K      = 4;
    l_keyI_st.I      = 5;
    l_keyI_st.F8     = 6;
    l_keyI_st.SPACE  = 7;
    l_keyI_st.ESCAPE = 8;
    l_keyI_st.J      = 9;
    l_keyI_st.L      = 10;
    l_keyI_st.H      = 11;
}

/**
 * @brief 
 * 
 */
void InputIrrlicht::InitGamepadButtonCodes()
{
    m_gamepadButtonCodes_u32v.reserve(9);
    m_gamepadButtonCodes_u32v.emplace_back(0); // A
    m_gamepadButtonCodes_u32v.emplace_back(1); // B
    m_gamepadButtonCodes_u32v.emplace_back(2); // X
    m_gamepadButtonCodes_u32v.emplace_back(3); // Y
    m_gamepadButtonCodes_u32v.emplace_back(4); // L1
    m_gamepadButtonCodes_u32v.emplace_back(5); // R1
    m_gamepadButtonCodes_u32v.emplace_back(6); // Back
    m_gamepadButtonCodes_u32v.emplace_back(7); // Start
    m_gamepadButtonCodes_u32v.emplace_back(8); // On/Off
    m_gamepadButtonCodes_u32v.emplace_back(9); // L3
    m_gamepadButtonCodes_u32v.emplace_back(10);// R3

}

/**
 * @brief 
 * 
 * @param l_buttonI_st 
 */
void InputIrrlicht::SetUpButtonIdentifiers(ButtonIdentifiers& l_buttonI_st)
{
    l_buttonI_st.A     = 0;
    l_buttonI_st.B     = 1;
    l_buttonI_st.X     = 2;
    l_buttonI_st.Y     = 3;
    l_buttonI_st.L1    = 4;
    l_buttonI_st.R1    = 5;
    l_buttonI_st.Back  = 6; 
    l_buttonI_st.Start = 7;
    l_buttonI_st.On    = 8;
    l_buttonI_st.L3    = 9;
    l_buttonI_st.R3    = 10;
}

/**
 * @brief 
 * 
 */
void InputIrrlicht::InitGamepadAxisCodes()
{
    m_gamepadAxisCodes_u32v.reserve(6);
    m_gamepadAxisCodes_u32v.emplace_back(0); // Joystick Left (Left ... Right) (-1 ... 1)
    m_gamepadAxisCodes_u32v.emplace_back(1); // Joystick Left (Up ... Down) (-1 ... 1)
    m_gamepadAxisCodes_u32v.emplace_back(2); // L2 (-1 not pressed) (1 pressed)
    m_gamepadAxisCodes_u32v.emplace_back(3); // Joystick Right (Left ... Right) (-1 ... 1)
    m_gamepadAxisCodes_u32v.emplace_back(4); // Joystick Right (Up ... Down) (-1 ... 1)
    m_gamepadAxisCodes_u32v.emplace_back(5); // R2 (-1 not pressed) (1 pressed)
}

/**
 * @brief 
 * 
 * @param l_buttonI_st 
 */
void InputIrrlicht::SetUpAxisIdentifiers(AxisIdentifiers& l_axisI_st)
{
    l_axisI_st.LJLR = 0;
    l_axisI_st.LJUD = 1;
    l_axisI_st.L2   = 2;
    l_axisI_st.RJLR = 3;
    l_axisI_st.RJUD = 4;
    l_axisI_st.R2   = 5;
}