#pragma once

#include <string>
#include <vector>

#include "IComponent.h"

class LapGUIComponent: public IComponent{

    public:
        LapGUIComponent(const std::string& id, int ent_id) : IComponent(id, ent_id)
        {
            m_lapSpriteRoutes.push_back("./assets/0.jpg");
            m_lapSpriteRoutes.push_back("./assets/1.jpg");
            m_lapSpriteRoutes.push_back("./assets/2.jpg"); 
        };
        ~LapGUIComponent(){};
        std::string GetSpriteRoute(unsigned int lap) const
        { 
            lap--;
            if(lap<m_lapSpriteRoutes.size())
                return m_lapSpriteRoutes[lap]; 
            else
                return m_lapSpriteRoutes[m_lapSpriteRoutes.size()-1];
        };

    private:
        std::vector<std::string> m_lapSpriteRoutes;
    
};