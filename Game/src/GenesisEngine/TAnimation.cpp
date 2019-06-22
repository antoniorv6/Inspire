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
#include <TAnimation.hpp>
#include <Clock.hpp>
#include <TResourceShader.hpp>
#include <TSecuence.hpp>
#include <TLodSecuence.hpp>

/**
 * @brief Construct a new TAnimation::TAnimation object
 * 
 */
TAnimation::TAnimation(glm::vec3& l_viewPosition_ptr, glm::vec3& l_position_ptr)
{
    m_clock_ptr         = new Clock();
    m_frameRate_f       = 1.5f/2.0f;
    m_currentSecuence   = nullptr;
    m_position_ptr     = &l_position_ptr;
    m_viewPosition_ptr = &l_viewPosition_ptr;
}

/**
 * @brief Destroy the TAnimation::TAnimation object
 * 
 */
TAnimation::~TAnimation()
{
    std::map<std::string, ISecuence*> emptySequence;

    for(auto& secuence : m_secuences)
    {
        delete secuence.second;
        secuence.second = nullptr;
    }
    
    m_secuences.swap(emptySequence);
    delete m_clock_ptr;
    m_clock_ptr = nullptr;
}

/**
 * @brief: selects the corresponding animation frame.
 * 
 */
void TAnimation::Update()
{
    if(m_isPlaying)
    {
        // To reset the clock for the first time
        if(m_firstTime)
        {
            m_clock_ptr->Restart();
            m_firstTime = false;
        }

        m_elapsedTime_f = (float)m_clock_ptr->GetElapsedTime();
        m_totalTime_f   += m_elapsedTime_f;
        // Next frame
        if(m_totalTime_f > m_frameRate_f)
        {
            m_totalTime_f = 0;
            m_currentFrame_n++;
            m_clock_ptr->Restart();
        }
        // Ends a non-loopable animation
        if(m_currentFrame_n == m_currentSecuence->GetSize() - 2 && !m_currentSecuence->IsLoopable())
        {
            m_isPlaying = false;
        }
        // Resets a loopbale animations if it has finished.
        if(m_currentFrame_n >= m_currentSecuence->GetSize())
        {
            m_currentFrame_n = 0;   
        }
    }
}

/**
 * @brief: activates a non-loopable animation.
 * 
 */
void TAnimation::PlayNoLoopable()
{
    if(!m_isPlaying)
    {
        m_currentFrame_n = 0;
        m_isPlaying = true;
    }
}

/**
 * @brief: resets an animation.
 * 
 */
void TAnimation::Reset()
{
    m_currentFrame_n = 0;
}

/**
 * @brief 
 * 
 */
void TAnimation::BeginDraw()
{
    if(m_currentSecuence)
    {
        Update();
        m_currentSecuence->Draw(m_currentFrame_n);
    }
}

/**
 * @brief 
 * 
 */
void TAnimation::EndDraw()
{

}

/**
 * @brief 
 * 
 * @param l_secuenceName_s 
 * @param l_frames 
 * @param l_isLoopable 
 */
void TAnimation::AddSecuence(const std::string& l_secuenceName_s, const std::vector<std::string>& l_frames, bool l_isLoopable)
{
    ISecuence * secuence = reinterpret_cast<ISecuence*>(new TSecuence(l_frames, 0, l_isLoopable));
    m_secuences[l_secuenceName_s] = secuence;
}

/**
 * @brief 
 * 
 * @param l_levelsCount 
 * @param l_distances 
 * @param l_viewPositionconst 
 * @param l_secuenceName_s 
 * @param l_lodFrames 
 * @param l_isLoopable 
 */
void TAnimation::AddSecuence
(            
    uint32_t l_levelsCount, 
    const std::vector<float>& l_distances,
    const std::string& l_secuenceName_s, 
    const std::vector<std::vector<std::string>>& l_lodFrames, 
    bool l_isLoopable
)
{
    TLodSecuence * secuence = new TLodSecuence(l_lodFrames.size(), l_levelsCount, l_distances, *m_viewPosition_ptr, *m_position_ptr,  0, l_isLoopable);

    for(uint32_t i = 0; i < l_lodFrames.size(); i++)
    {
        for(uint32_t j = 0; j < l_lodFrames[i].size(); j++)
        {
            secuence->SetLodFrame(i, l_lodFrames[i][j]);
        }
    }
    
    ISecuence * auxsecuence = reinterpret_cast<ISecuence*>(secuence);

    m_secuences[l_secuenceName_s] =  secuence;
}

/**
 * @brief 
 * 
 * @param l_secuence 
 */
void TAnimation::SetSecuence(const std::string& l_secuence)
{
    m_currentSecuence = m_secuences[l_secuence];
}

/**
 * @brief 
 * 
 * @param l_secuence 
 * @param l_texturePath 
 */
void TAnimation::SetSecuenceTexture(const std::string& l_secuence, const std::string& l_texturePath)
{
    m_secuences[l_secuence]->SetTexture(l_texturePath);
}


/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool TAnimation::GetActive()
{
    //[WARNING] NO RETURN
    return false;
}

/**
 * @brief 
 * 
 * @param c_newFrameRate 
 */
void TAnimation::SetFrameRate(float c_newFrameRate)
{
    m_frameRate_f = c_newFrameRate;
}