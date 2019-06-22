/**
 * @file ISecuence.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-05-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <string>

class ISecuence
{
    friend class TAnimation;
    public:
        ISecuence(){};
        virtual ~ISecuence(){};

        virtual void Draw(int l_frame) = 0;
        virtual void SetTexture(const std::string& l_texturePath) = 0;
        virtual size_t GetSize() = 0;

        bool IsLoopable()   { return m_loop; };

    protected:
        int m_framesNumber_n  = 60;
        bool m_loop      = true;
};