/**
 * @file PostProcessing.cpp
 * @author Edward Owen (contact@edwardowen.es)
 * @brief 
 * @version 0.1
 * @date 2019-04-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <PostProcessing.hpp>
#include <IEffect.hpp>
#include <TResourceShader.hpp>
#include <DefaultEffect.hpp>
#include <NitroEffect.hpp>
#include <GlitchEffect.hpp>
#include <FreezeEffect.hpp>
#include <MagnetEffect.hpp>
#include <MirrorEffect.hpp>
#include <ShieldEffect.hpp>
#include <ThiefEffect.hpp>
#include <ChargingEffect.hpp>

/**
 * @brief Construct a new Post Processing:: Post Processing object
 * 
 */
PostProcessing::PostProcessing(uint32_t l_screenWidth, uint32_t l_screenHeight, std::map<std::string, TResourceShader*>& l_shadersMap)
{
    m_screenWidth_n  = l_screenWidth;
    m_screenHeight_n = l_screenHeight;

    InitEffects(l_shadersMap);
}

/**
 * @brief 
 * 
 */
void PostProcessing::InitEffects(std::map<std::string, TResourceShader*>& l_shadersMap)
{
    IEffect * effect; 

    DefaultEffect * defaultFX  = new DefaultEffect(m_screenWidth_n, m_screenHeight_n, l_shadersMap["defaultPP"], "defaultPP");
    effect                     = reinterpret_cast<IEffect*>(defaultFX);
    m_effects["defaultPP"]     = effect; 

    NitroEffect * nitroFX      = new NitroEffect(m_screenWidth_n, m_screenHeight_n, l_shadersMap["nitro"], "nitro");
    effect                     = reinterpret_cast<IEffect*>(nitroFX);
    m_effects["nitro"]         = effect; 

    GlitchEffect * glitchFX    = new GlitchEffect(m_screenWidth_n, m_screenHeight_n, l_shadersMap["glitch"], "glitch");
    effect                     = reinterpret_cast<IEffect*>(glitchFX);
    m_effects["glitch"]        = effect; 

    FreezeEffect * freezeFX    = new FreezeEffect(m_screenWidth_n, m_screenHeight_n, l_shadersMap["freeze"], "freeze");
    effect                     = reinterpret_cast<IEffect*>(freezeFX);
    m_effects["freeze"]        = effect; 

    MagnetEffect * magnetFX    = new MagnetEffect(m_screenWidth_n, m_screenHeight_n, l_shadersMap["magnet"], "magnet");
    effect                     = reinterpret_cast<IEffect*>(magnetFX);
    m_effects["magnet"]        = effect; 

    MirrorEffect * mirrorFX    = new MirrorEffect(m_screenWidth_n, m_screenHeight_n, l_shadersMap["mirror"], "mirror");
    effect                     = reinterpret_cast<IEffect*>(mirrorFX);
    m_effects["mirror"]        = effect; 

    ShieldEffect * shieldFX    = new ShieldEffect(m_screenWidth_n, m_screenHeight_n, l_shadersMap["shield"], "shield");
    effect                     = reinterpret_cast<IEffect*>(shieldFX);
    m_effects["shield"]        = effect; 

    ChargingEffect * chargingFX   = new ChargingEffect(m_screenWidth_n, m_screenHeight_n, l_shadersMap["charging"], "charging");
    effect                        = reinterpret_cast<IEffect*>(chargingFX);
    m_effects["charging"]         = effect; 

    ThiefEffect * thiefFX      = new ThiefEffect(m_screenWidth_n, m_screenHeight_n, l_shadersMap["thief"], "thief");
    effect                     = reinterpret_cast<IEffect*>(thiefFX);
    m_effects["thief"]         = effect; 
}

/**
 * @brief Destroy the Post Processing:: Post Processing object
 * 
 */
PostProcessing::~PostProcessing()
{
    for(auto& effect : m_effects)
    {
        delete effect.second;
        effect.second = nullptr;
    }
    
    m_effects.clear();
}

/**
 * @brief 
 * 
 * @param l_screenWidth 
 * @param l_screenHeight 
 */
void PostProcessing::Generate(uint32_t l_screenWidth, uint32_t l_screenHeight)
{
    m_screenWidth_n  = l_screenWidth;
    m_screenHeight_n = l_screenHeight;

    for(auto& effect : m_effects)
    {
        effect.second->DeleteBuffers();
        effect.second->Init(m_screenWidth_n, m_screenHeight_n);
    }
}

/**
 * @brief: Update object
 * 
 */
void PostProcessing::Update()
{
    if(m_activeEffect->IsFinished())
    {
        m_activeEffect = m_effects["defaultPP"];
    }

    m_activeEffect->Bind();
    glEnable(GL_DEPTH_TEST);
}

/**
 * @brief 
 * 
 */
void PostProcessing::PostProcess(float dt)
{
    m_activeEffect->Draw(dt);
}

/**
 * @brief 
 * 
 * @param l_effectName 
 */
void PostProcessing::ActivateEffect(const std::string& l_effectName)
{   
    m_activeEffect = m_effects[l_effectName];
    m_activeEffect->ResetFinished();
}

/**
 * @brief 
 * 
 * @param l_effectName 
 */
std::string PostProcessing::GetEffectID()
{
    return m_activeEffect->GetID();
}


