/**
 * @file IInputFacade.hpp
 * @author Edward Owen (contact@edwardowen.es)
 * 
 * @brief: Facade's interface to handle the Keyboard and Mouse events
 * using different Engines (Irrlicht or OpenGL)
 * 
 * @version 0.1
 * @date 2018-11-12
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#pragma once
#include <irrlicht.h>
#include <IInputFacade.hpp>
#include <vector>

struct KeyIdentifiers
{
    u_int32_t W      = 0; 
    u_int32_t A      = 0; 
    u_int32_t S      = 0; 
    u_int32_t D      = 0; 
    u_int32_t I      = 0; 
    u_int32_t K      = 0; 
    u_int32_t J      = 0; 
    u_int32_t L      = 0;
    u_int32_t F8     = 0;
    u_int32_t ESCAPE = 0;
    u_int32_t SPACE  = 0;
    u_int32_t H      = 0;
};

struct ButtonIdentifiers
{
    u_int32_t A     = 0;  
    u_int32_t B     = 0;
    u_int32_t X     = 0;
    u_int32_t Y     = 0;
    u_int32_t L1    = 0;
    u_int32_t R1    = 0;
    u_int32_t Back  = 0;
    u_int32_t Start = 0;
    u_int32_t On    = 0;
    u_int32_t L3    = 0;
    u_int32_t R3    = 0;
};

struct AxisIdentifiers
{
    //Left Joystick Left and Right
    u_int32_t LJLR = 0;
    //Lett Joystick Up and Down
    u_int32_t LJUD = 0;
    //Right Joystick Up and Down
    u_int32_t RJLR = 0;
    //Right Joystick Up and Down
    u_int32_t RJUD = 0;
    //L2 trigger
    u_int32_t L2    = 0;
    //R2 trigger
    u_int32_t R2    = 0;
};

class IInputFacade
{
public:
    IInputFacade(){};
    virtual ~IInputFacade(){};

    virtual float IsKeyPressed(unsigned int l_keyCode_u32) const = 0;
    virtual float IsButtonPressed(unsigned int l_buttonCode_u32) const = 0;
    virtual float IsAxisPressed(unsigned int l_AxisCode_u32) const = 0;

    virtual std::vector<unsigned int> GetGameKeyCodes() const = 0;
    virtual std::vector<unsigned int> GetGamepadButtonCodes() const = 0;
    virtual std::vector<unsigned int> GetGamepadAxisCodes() const = 0;

    virtual void SetUpKeyIdentifiers(KeyIdentifiers& l_keyI_st) = 0;
    virtual void SetUpButtonIdentifiers(ButtonIdentifiers& l_buttonI_st) = 0;
    virtual void SetUpAxisIdentifiers(AxisIdentifiers& l_buttonI_st) = 0;

    virtual int GetMouseXPos() = 0;
    virtual int GetMouseYPos() = 0;
    virtual bool IsMousePressed() = 0;
    virtual void SetMouseClick(bool value) = 0;

protected:
    std::vector<unsigned int>   m_gameKeyCodes_u32v;
    std::vector<unsigned int>   m_gamepadButtonCodes_u32v;
    std::vector<unsigned int>   m_gamepadAxisCodes_u32v;
};