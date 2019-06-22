#pragma once

#include <string>
#include <vector>

#include <LapGUIComponent.hpp>
#include <GUIMiniMapMarker.hpp>
#include <EntityManager.h>
#include <Entity.h>

class RenderManager;
class GenericData;

class GUIManager{
    public:
        GUIManager();
        ~GUIManager();
        void Init(GenericData *& c_genericData);
        void ClearList(){};

    private:
        EntityManager* m_entityManager_ptr = nullptr;
        RenderManager* m_renderManager_ptr = nullptr;
};