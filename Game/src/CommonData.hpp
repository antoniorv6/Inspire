#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <glm.hpp>
#include <memory>

enum TypePU
{
    shield,
    magnet,
    thief,
    cyberattack,
    freezing,
    reflector,
};

struct DroneData
{
    std::string m_characterName;
    std::string droneModel;
    std::string droneTexture;
    std::string droneName;
    int m_PUPreset;
};


class RenderUtilities
{
public:
    static RenderUtilities* getInstance()
    {
        static RenderUtilities instance;
        return &instance;
    }
    ~RenderUtilities(){};

    void SetNitroValue(float value) { m_nitroValue = value; };
    void SetAlphaValue(float value) { m_alphaValue = value; };

    float GetNitroValue()           { return m_nitroValue; };
    float GetAlphaValue()           { return m_alphaValue; };

    // ==== FREEZE EFFECT ==== // 
    void UpdateFreezeValues();
    void ResetFreezeValues();
    float GetOffsetDenom()          { return m_offsetDenom; };
    float GetMixPct()               { return m_mixPct; };
    bool IsFreezeDesactivated()     { return m_desactivateFreeze; };
    void DesactivateFreeze()        { m_desactivateFreeze = true; };

    // ==== MAGNET EFFECT ==== // 
    void UpdateMagnetValues();
    void ResetMagnetValues();
    float GetAmplitude()            { return m_amplitude; }
    float GetRefraction()           { return m_refraction; }
    float GetWidth()                { return m_width; }
    bool IsMagnetDesactivated()     { return m_desactivateMagnet; };
    void DesactivateMagnet()        { m_desactivateMagnet = true; };

    // ==== THIEF EFFECT ==== // 
    void UpdateThiefValues();
    void ResetThiefValues();
    glm::vec3 GetAverageRGB()       { return m_averageRGB; }
    bool IsThiefDesactivated()      { return m_desactivateThief; };
    void DesactivateThief()         { m_desactivateThief = true; };

    // ==== SHIELD EFFECT ==== // 
    void UpdateShieldValues();
    void ResetShieldValues();
    float GetRedPct()                { return m_redPct; };
    float GetBluePct()               { return m_bluePct; };
    float GetShieldAlpha()           { return m_shildAlpha; }
    bool IsShieldDesactivated()      { return m_desactivateShield; };
    void DesactivateShield()         { m_desactivateShield = true; };


    // ==== CHARGING EFFECT ==== // 
    void UpdateChargingValues();
    void ResetChargingValues();
    float GetExpand()                  { return m_expand; };
    bool IsChargingDesactivated()      { return m_desactivateCharging; };
    void DesactivateCharging()         { m_desactivateCharging = true; };

    // ==== MIRROR EFFECT ==== // 
    void UpdateMirrorValues();
    void ResetMirrorValues();
    float GetGreenPct()              { return m_greenPct; };
    float GetBluePct2()              { return m_bluePct2; };
    float GetMirrorAlpha()           { return m_mirrorAlpha; }
    bool IsMirrorDesactivated()      { return m_desactivateMirror; };
    void DesactivateMirror()         { m_desactivateMirror = true; };


    // ==== UTILITIES ==== //
    std::vector<std::vector<std::string>> GetLODNamesMatrix
    (
        
        const std::string& l_meshName, 
        uint32_t l_colCount,
        uint32_t l_rowCounter
    );

    std::vector<std::string> GetLODNamesVector
    (
        const std::string& l_meshName,
        uint32_t l_count
    );

    std::vector<std::string> SetUpNameVector
    (
        const std::string& l_firtsName, 
        const std::string& l_lastName,
        uint32_t l_framesNumber
    );

    std::vector<std::string> GetLODTexturesNames(const std::string& l_textureNames);

private:
    float m_nitroValue    = 0.0f;
    float m_alphaValue    = 1.0f;

    // ==== FREEZE EFFECT ==== //
    bool m_desactivateFreeze = false;
    float m_mixPct           = 0.0f;
    float m_offsetDenom      = 1.0f;

    // ==== MAGNET EFFECT ==== // 
    bool  m_desactivateMagnet = false;
    float m_amplitude  = 0.0f;
    float m_refraction = 0.08f; 
    float m_width      = 0.01f;

    // ==== THIEF EFFECT ==== // 
    bool  m_desactivateThief = false;
    glm::vec3 m_averageRGB = glm::vec3(1.0f);

    // ==== SHIELD EFFECT ==== // 
    bool  m_desactivateShield = false;
    float m_shildAlpha = 0.0f;
    float m_redPct     = 0.0f;
    float m_bluePct    = 0.0f;

    // ==== CHARGING EFFECT ==== // 
    bool  m_desactivateCharging = false;
    float m_expand = 1.0f;

    // ==== MIRROR EFFECT ==== // 
    bool m_desactivateMirror = false;
    float m_mirrorAlpha   = 0.0f;
    float m_greenPct      = 0.0f;
    float m_bluePct2      = 0.0f;
};



