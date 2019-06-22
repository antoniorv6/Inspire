/**
 * @file InputGenesis.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-03-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <IInputFacade.hpp>
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <list>
#include <algorithm>
#include <functional>

class GenesisEngine;
class GenericData;

class InputGenesis : public IInputFacade
{
    friend void On_Event(GLFWwindow* l_window_ptr, int l_key_n, int l_scanCode_n, int l_action_n, int mods);
    friend void On_MouseEvent(GLFWwindow* window, int button, int action, int mods);
    friend void GetCursorPosition(GLFWwindow* window, double xPos, double yPos);
public:
    InputGenesis();
    ~InputGenesis();

    void Init(GenericData* l_genericData_ptr);
    void OnEvent(GLFWwindow* l_window_ptr, int l_key_n, int l_scanCode_n, int l_action_n, int mods);
    void On_MouseEvent(GLFWwindow* window, int button, int action, int mods);
    void GetCursorPosition(GLFWwindow* window, double xPos, double yPos);
    
    void UpdateGamepadValues();
    void ShowJoystickValues(int present, int c_axes, const float* axes, int c_buttons, const unsigned char* buttons, const char* name);

    virtual float IsKeyPressed(unsigned int l_keyCode_u32) const;
    virtual float IsButtonPressed(unsigned int l_buttonCode_u32) const;
    virtual float IsAxisPressed(unsigned int l_AxisCode_u32) const;

    virtual std::vector<unsigned int> GetGameKeyCodes() const;
    virtual std::vector<unsigned int> GetGamepadButtonCodes() const;
    virtual std::vector<unsigned int> GetGamepadAxisCodes() const;

    virtual void SetUpKeyIdentifiers(KeyIdentifiers& l_keyI_st);
    virtual void SetUpButtonIdentifiers(ButtonIdentifiers& l_buttonI_st);
    virtual void SetUpAxisIdentifiers(AxisIdentifiers& l_buttonI_st);

    virtual int GetMouseXPos();
    virtual int GetMouseYPos();
    virtual bool IsMousePressed();
    virtual void SetMouseClick(bool value){m_mousePressed_b = value;};

    void InitGameKeyCodes();
    void InitGamepadButtonCodes();
    void InitGamepadAxisCodes();

private:
    bool    m_keyStatus_vb[1024];
    float   m_buttonsStatus_bv[11];
    float   m_axisStatus_bv[6];

    double m_mousePosition_x = 0;
    double m_mousePosition_y = 0;
    bool m_mousePressed_b    = false;
    bool m_mouseHold_b       = false;
};