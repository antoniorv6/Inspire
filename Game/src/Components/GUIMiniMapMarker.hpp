#pragma once
#include "IComponent.h"
#include <string>

class GUIMiniMapMarker : public IComponent{
    public:
         GUIMiniMapMarker(const std::string& id, int id_ent) : IComponent(id, id_ent){
            m_posX_i = 0;
            m_posY_i = 0;
         };
         ~GUIMiniMapMarker(){};
         int GetX() { return m_posX_i; };
         int GetY() { return m_posY_i; };
         void SetMarkerPosition(int c_minimapPositionX_i, int c_minimapPositionY_i, int c_minimapWidth_i, int c_minimapHeight_i, int c_mapWidth_i, int c_mapHeight_i, int c_EntPosX_i, int c_EntPosY_i)
         {
            m_posX_i = ((c_minimapWidth_i * c_EntPosX_i)/c_mapWidth_i) + c_minimapPositionX_i + c_minimapWidth_i/2 ;
            m_posY_i = ((c_minimapHeight_i * c_EntPosY_i)/c_mapHeight_i) + c_minimapPositionY_i + c_minimapHeight_i/2;
         }

    private:
        int m_posX_i = 0;
        int m_posY_i = 0;
};