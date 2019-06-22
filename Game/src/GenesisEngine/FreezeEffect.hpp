/**
 * @file FreezeEffect.hpp
 * @author Edward Owen (contact@edwardowen.es)
 * @brief 
 * @version 0.1
 * @date 2019-04-25
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <IEffect.hpp>

class RenderUtilities;

class FreezeEffect : public IEffect
{
public:
    FreezeEffect(uint32_t l_screenWidth, uint32_t l_screenHeight, TResourceShader * l_shader_ptr, const std::string& l_ID);
    virtual ~FreezeEffect();

    virtual void Init(uint32_t l_screenWidth, uint32_t l_screenHeight);
    virtual void SetShadingValues(){};
    virtual void Draw(float dt);

private:
    uint32_t m_screenWidth = 0, m_screenHeight = 0;
    RenderUtilities * m_renderUtilities_ptr = nullptr;

};