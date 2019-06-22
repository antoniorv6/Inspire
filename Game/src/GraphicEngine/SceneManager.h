#pragma once
#include <irrlicht.h>
#include <map>
#include <glm.hpp>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class SceneManager
{
    public:
        SceneManager();
        
        void UpdateCameraTarget(unsigned int c_nodename_s, const core::vector3df& NodePosition, const core::vector3df& newPos);
        
        const void initPointerValue(scene::ISceneManager *&c_sceneManager);
        void DrawBBOX(unsigned int c_name_s);
        bool AddNodeObject(unsigned int c_name_s, const core::vector3df& c_initialPosition_v3dirr);
        bool AddDebugObject(std::string c_name_s, unsigned int id, const core::vector3df& c_initialPosition_v3dirr);
        bool AddMeshObject(unsigned int c_name_s, const core::vector3df& c_initialPosition_v3dirr, const std::string& route);
        bool AddCamera(unsigned int c_name_s,  const core::vector3df& c_cameraPosition_vec3, const core::vector3df& c_cameraLookAt_vec3);
        void AddDebugSphere(unsigned int id, const core::vector3df& pos, float rad);
        void TranslateDebugSceneNode(unsigned int id, const core::vector3df& pos);

        void AddDebugCube(unsigned int c_name_s, const core::vector3df& c_initialPosition_v3dirr, const core::vector3df& c_initialRotation_v3dirr, const core::vector3df& c_initialScale_v3dirr);
        void MoveNode(unsigned int c_nodeName_s, const core::vector3df& c_newNodePosition_vec3);
        void RotateNode(unsigned int c_nodeName_s, const core::vector3df& c_newNodeRotation_vec3);
        void ScaleNode(unsigned int c_nodeName_s, float scaleFactor);
        void ScaleDebugNode(unsigned int c_nodeName_s, float scaleFactor);
        void RenderObjects();
        const bool AddTexture(unsigned int c_nodeName_s, video::ITexture*& c_texture_ptr);

        void CleanSceneTrees();
    private:
        scene::ISceneManager * m_sceneManager_p;
        //3D Scene node managers
        std::map<unsigned int, scene::ISceneNode*> m_SceneNodes_m;
        std::map<unsigned int, scene::ISceneNode*> m_DebugNodes_m;
        std::map<unsigned int, scene::ICameraSceneNode*> m_CameraNodes_m;
        
};