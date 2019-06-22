/**
 * @file TAnimation.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-04-03
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <TEntity.hpp>
#include <vector>
#include <string>

class Clock;
class TResourceShader;
class ISecuence;

class TAnimation : public TEntity
{
public:
    TAnimation(glm::vec3& l_viewPosition_ptr, glm::vec3& l_position_ptr);
    ~TAnimation();

    void Update();
    void PlayNoLoopable();
    void Reset();
    virtual void BeginDraw();
    virtual void EndDraw();
    virtual bool GetActive();

    void AddSecuence(const std::string& l_secuenceName_s, const std::vector<std::string>& l_frames, bool l_isLoopable);
    void AddSecuence
    (            
        uint32_t l_levelsCount, 
        const std::vector<float>& l_distances,
        const std::string& l_secuenceName_s, 
        const std::vector<std::vector<std::string>>& l_lodFrames, 
        bool l_isLoopable
    );

    void SetSecuence(const std::string& l_secuence);
    void SetSecuenceTexture(const std::string& l_secuence, const std::string& l_texturePath);
    void SetFrameRate(float c_newFrameRate);
    
private:
    ISecuence * m_currentSecuence  = nullptr;
    Clock     * m_clock_ptr        = nullptr;
    glm::vec3 * m_position_ptr     = nullptr;
    glm::vec3 * m_viewPosition_ptr = nullptr;
    std::map<std::string, ISecuence*> m_secuences;

    int m_frames_n           = 60;
    bool m_firstTime         = true;
    size_t m_currentFrame_n  = 0;
    float m_frameRate_f      = 1.5f/2.0f;
    float m_elapsedTime_f    = 0.0f;
    float m_totalTime_f      = 0.0f;

    bool m_isPlaying = true;
    bool m_loop      = true;
    bool m_render    = false;
};