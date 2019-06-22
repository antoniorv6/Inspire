/**
 * @file InputIrrlicht.hpp
 * @author Edward Owen (contact@edwardowen.es)
 * 
 * @brief: Irrlicht declaration of the IInputFacade interface
 * 
 * @version 0.1
 * @date 2018-11-12
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#pragma once
#include <IInputFacade.hpp>
#include <irrlicht.h>
#include <IrrIMGUI.h>
#include <iostream>
#include <vector>

class InputIrrlicht : public IInputFacade, public IrrIMGUI::CIMGUIEventReceiver
{
public:
    InputIrrlicht();
    ~InputIrrlicht();

    virtual bool OnEvent(const irr::SEvent& event);

    virtual float IsKeyPressed(unsigned int l_keyCode_u32) const;
    virtual float IsButtonPressed(unsigned int l_buttonCode_u32) const;
    virtual float IsAxisPressed(unsigned int l_axisCode_u32) const;

    virtual std::vector<unsigned int> GetGameKeyCodes() const;
    virtual std::vector<unsigned int> GetGamepadButtonCodes() const;
    virtual std::vector<unsigned int> GetGamepadAxisCodes() const;

    virtual void SetUpKeyIdentifiers(KeyIdentifiers& l_keyI_st);
    virtual void SetUpButtonIdentifiers(ButtonIdentifiers& l_buttonI_st);
    virtual void SetUpAxisIdentifiers(AxisIdentifiers& l_buttonI_st);

    virtual int GetMouseXPos(){ return 1; };
    virtual int GetMouseYPos(){ return 1; };
    virtual bool IsMousePressed(){  return false; }
    virtual void SetMouseClick(bool value){ };

    void InitGameKeyCodes();
    void InitGamepadButtonCodes();
    void InitGamepadAxisCodes();

private:
    float m_keysStatus_bv[irr::KEY_KEY_CODES_COUNT];
    float m_buttonsStatus_bv[11];
    float m_axisStatus_bv[6];

};