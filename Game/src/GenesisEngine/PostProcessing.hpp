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
#pragma once
#include <map>
#include <string>

class IEffect;
class TResourceShader;

class PostProcessing
{
public:
    PostProcessing(uint32_t l_screenWidth, uint32_t l_screenHeight, std::map<std::string, TResourceShader*>& l_shadersMap);
    ~PostProcessing();

    void InitEffects(std::map<std::string, TResourceShader*>& l_shadersMap);
    void Generate(uint32_t l_screenWidth, uint32_t l_screenHeight);
    void Update();
    void PostProcess(float dt);
    void ActivateEffect(const std::string& l_effectName);
    std::string GetEffectID();

private:
    uint32_t m_screenWidth_n = 0, m_screenHeight_n = 0;

    IEffect* m_activeEffect = nullptr;
    std::map<std::string, IEffect*> m_effects;
};