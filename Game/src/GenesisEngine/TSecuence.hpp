/**
 * @file TSecuence.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-04-06
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <vector>
#include <ISecuence.hpp>

class TMesh;
class TResourceShader;

class TSecuence : public ISecuence
{
public:
    TSecuence(const std::vector<std::string>& l_frames, int l_startPoint_n, bool l_loop);
    virtual ~TSecuence();

    virtual void Draw(int l_frame);
    virtual void SetTexture(const std::string& l_texturePath);
    virtual size_t GetSize()    { return m_frames.size(); };

private:
    std::vector<TMesh*> m_frames;
};