#pragma once

#include "IrrlichtRender.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "DebugGUIManager.hpp"
#include "WaypointManager.hpp"
#include "InputIrrlicht.hpp"
#include "RenderComponent.h"
#include "Octree.h"
#include "Event.h"
#include <glm.hpp>
#include <string>
#include <vector>

class InputManager; 
class GUIManager;
class DebugGUIManager;
class GenericData;
class RenderUtilities;

class RenderManager{
    public:
        RenderManager();
        ~RenderManager();
        void Init(int engine, GenericData *& c_genericData);
        void RenderScene(Entity* entity);
        void UpdateCameraTarget(Entity* entity, const glm::vec3& newPos, float distZ);
        void CreateComponent(Entity* entity, const std::string& type, const std::string& file);
        void DebugBBox(const glm::vec3& pos, float scale);
        void ScaleSceneNode(Entity* entity, float scaleFactor);
        void RotateSceneNode(Entity* entity, const glm::vec3& position);
        void AddNode(Entity *entity, int c_hasDetail = 0);
        void AddAnimatedSecuence(Entity* entity, const std::string& l_secuenceName, const std::vector<std::string>& l_frames, bool l_loop);
        void AddAnimatedSecuence(Entity* entity, const std::string& l_secuenceName, const std::vector<std::vector<std::string>>& l_frames, bool l_loop);
        void AddParticlesEmitter(const std::string& l_meshName, const std::string& l_textureSource, const glm::vec3& l_position, uint32_t l_particlesAmount);
        void SetSecuence(unsigned int c_nodeName, const std::string& l_secuenceName);
        void ActivatePPEffect(const std::string& l_effectId);
        std::string GetEffectID();
        void SetPPShader(const std::string& l_shaderId);
        void SetPerspectiveAngle(double l_angle);
        
        void CloseWindow();
        void DrawGUI();
        void DrawBBOX(Entity* entity);
        void AddTexture(Entity* entity, const std::string& c_texturePath_s);
        void AddTexture(Entity* entity, const std::string& l_texturePath, const std::string& l_secuence);
        void SetDebugMode();
        void ClearList();
        void AddDebugCube(unsigned int c_name_s, const glm::vec3& c_initialPosition_v3dirr, const glm::vec3& c_initialRotation_v3dirr, const glm::vec3& c_initialScale_v3dirr);
        void AddCamera(Entity* c_entity_ptr, const glm::vec3& c_initialPosition_vec3);
        void DebugBSphere(unsigned int id, const glm::vec3& pos, float rad);
        void TranslateBSphere(unsigned int id, const glm::vec3& pos);

        void Reset();
        
        void SetDebugAIPath();
        void DrawAIVectors();
        void DrawPlayerVectors();
        void DrawOBBVertex();
        bool getPlay() { return render->IsWindowOpened(); };

        IRenderFacade* GetRenderFacade();

    private:
        IRenderFacade* render = nullptr;

        bool m_debugMode = false;
        std::vector<RenderComponent*> components;
        
        EntityManager   * m_entityManager_ptr     = nullptr;
        InputManager    * m_inputManager_ptr      = nullptr;
        PhysicsManager  * m_physicsManager_ptr    = nullptr;
        GUIManager      * m_guiManager_ptr        = nullptr;
        DebugGUIManager * m_debugGuiManager_ptr   = nullptr;
        WaypointManager * m_waypointManager_ptr   = nullptr;
        RenderUtilities * m_renderUtilities_ptr   = nullptr;
};