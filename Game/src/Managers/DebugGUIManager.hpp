#pragma once
#include <IrrIMGUI.h>
#include <vector>
#include <string>

class GenericData;
class Entity;
class EntityManager;
class DebugConsoleWindowComponent;

class DebugGUIManager
{
    public:
        DebugGUIManager();
        ~DebugGUIManager();
        void Init(GenericData *&c_genericData);
        
        const void initPointerValue(IrrIMGUI::IIMGUIHandle *&c_GUIDevice);
        void CreateComponent(Entity* entity, int x, int y, int w, int h);
        void AddTextToInterface(Entity* entity, const std::string& c_text);
        void ClearComponentWindows();
        void RenderDebugWindows();
        void ClearList();

    private:
        IrrIMGUI::IIMGUIHandle* m_GUIDevice_ptr = nullptr;
        std::vector<DebugConsoleWindowComponent*> m_DConsoleComponents_vec;

        EntityManager* m_entityManager_ptr = nullptr;
};