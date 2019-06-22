/**
 * @file DefaultEffect.hpp
 * @author Edward Owen (contact@edwardowen.es)
 * @brief 
 * @version 0.1
 * @date 2019-04-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <IEffect.hpp>

class DefaultEffect : public IEffect
{
public:
    DefaultEffect(uint32_t l_screenWidth, uint32_t l_screenHeight, TResourceShader * l_shader_ptr, const std::string& l_ID);
    virtual ~DefaultEffect();

    virtual void Init(uint32_t l_screenWidth, uint32_t l_screenHeight);
    virtual void SetShadingValues(){};
    virtual void Draw(float dt);
};