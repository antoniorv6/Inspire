
#pragma once
#include <irrlicht.h>
#include <memory>
#include <IRenderFacade.h>
#include <InputIrrlicht.hpp>
#include <SceneManager.h>

#include <IrrIMGUI.h>

using namespace irr;

class IrrlichtRender: public IRenderFacade
{
    public:
        IrrlichtRender(int width, int height, bool fullscreen, InputIrrlicht* inputIrrlicht);
        virtual ~IrrlichtRender();

        void Render();
        bool IsWindowOpened();
        void CloseWindow();
        void DrawBBOX(unsigned int c_nodeName_s);
        virtual void DrawGUI() {};
        
        void UpdateCameraTarget(unsigned int c_nodeName_s, const glm::vec3& pos, const glm::vec3& newPos);
        virtual void SetPerspectiveAngle(double l_angle){};
        virtual void AddParticlesEmitter(const std::string& l_meshName, const std::string& l_textureSource, const glm::vec3& l_position, uint32_t l_particlesAmount){};
        virtual void AddLodMesh(unsigned int c_nodeName_s, const std::string& l_LODName, const glm::vec3& l_position){};

        bool AddMesh(unsigned int c_nodeName_s, const glm::vec3& c_initial_position_vec3, const std::string& route, int c_detailed = 0);
        virtual bool AddAnimatedMesh(unsigned int c_nodeName_s, const glm::vec3& c_initial_position_vec3){return false;};
        virtual void AddAnimatedSecuence(unsigned int c_nodeName, const std::string& l_secuenceName, const std::vector<std::string>& l_frames, bool l_loop){};
        virtual void AddAnimatedSecuence(unsigned int c_nodeName, const std::string& l_secuenceName, const std::vector<std::vector<std::string>>& l_frames, bool l_loop){};
        virtual void SetSecuence(unsigned int c_nodeName, const std::string& l_secuenceName){};
        bool AddNode(unsigned int c_nodeName_s, const glm::vec3& c_initial_position_vec3);
        bool AddCamera(unsigned int c_cameraName_s, const glm::vec3& c_cameraPosition_vec3, const glm::vec3& c_cameraLookAt_vec3);
        virtual void AddShader(const std::string& l_shaderId, const std::string& l_shaderDir_str){};
        virtual void ActivatePPEffect(const std::string& l_effectId){};
        virtual std::string GetEffectID(){ return ""; };

        void SetAnimationFrameRate(unsigned int c_node, float c_newFrameRate){};
        virtual void AddLight(const glm::vec3& l_lightPosition_v3, const glm::vec3& l_color, const glm::vec3& l_attenuation){};
        
        void TranslateSceneNode(unsigned int c_nodeName_s, const glm::vec3& c_nodeNewPosition_vec3);
        void RotateSceneNode(unsigned int c_nodeName_s, const glm::vec3& c_nodeNewRotation_vec3);
        void ScaleSceneNode(unsigned int c_nodeName_s, float scaleFactor);
        
        core::vector3df glmVec2irrVec(const glm::vec3& c_glmVector_vec3);
        glm::vec3 irrVec2glmVec(const core::vector3df& c_irrVector_vec3);

        bool AddTexture(unsigned int c_nodeName_s, const std::string& c_texturePath_s);
        virtual bool AddTexture(unsigned int c_nodeName_s, const std::string& c_texturePath_s, const std::string& l_secuence){return false;};

        void BeginScene();
        void EndScene();

        void SetSpriteActiveStatus(unsigned int c_id, bool c_status, bool HUDorGUI){};
        
        //* DEBUG RELATED FUNCTIONS *//
        //----------------------------------------------------------------------------------------//
        void DebugDraw2DLine(const glm::vec3& c_origin_v3, const glm::vec3& c_destiny_v3, unsigned int color);
        void DebugDraw2DPoint(const glm::vec3& c_point_v3, unsigned int color);
        void AddDebugNode(const std::string& c_nodeName_s, unsigned int id, const glm::vec3& c_initial_postiion_vec3);
        void ScaleDebugNode(unsigned int c_nodeName_s, float c_scaleFactor_f);
        void AddDebugCube(unsigned int c_name_s, const glm::vec3& c_initialPosition_v3dirr, const glm::vec3& c_initialRotation_v3dirr, const glm::vec3& c_initialScale_v3dirr);
        void AddDebugSphere(unsigned int c_id_ui, const glm::vec3& c_pos_v3, float c_rad_f);
        void TranslateDebugSceneNode(unsigned int id, const glm::vec3& pos);
        //----------------------------------------------------------------------------------------//
        //* GRAPHICAL USER INTERFACE RELATED FUNCTIONS *//
        //------------------------------------------------------------------------------------------------------------------------------------------//
        void GUIDrawBox(int c_xPos, int c_yPos, int c_width, int c_height, int c_Red, int c_Blue, int c_Green);
        void GUIDrawSprite(const std::string& c_SpriteRoute, int c_xPos, int c_yPos);
        void GUIDraw2DPoint(int c_xPos, int c_YPos);
        virtual void GUIScaleSprite(unsigned int c_nodeName_i, int c_width, int c_height, bool HUDorGUI){};
        virtual void GUIEraseSprite(int c_howMany_n){};

        virtual void CreateBillboard(int c_id, const std::string& c_billBoardTexture_str, float l_width_f, float l_height_f, glm::vec3& c_initialPosition){};
        virtual void TranslateBillboard(int c_id, glm::vec3& c_newPosition){};

        int GetScreenWidth() { return m_irrVideoDriver_p->getScreenSize().Width; };
        int GetScreenHeight() { return m_irrVideoDriver_p->getScreenSize().Height; };
        //------------------------------------------------------------------------------------------------------------------------------------------//

        virtual void Reserver4Meshes(size_t l_memoryCount_st){};

        virtual void GUICreateSprite(unsigned int c_nodeName_i, int c_xPos, int c_yPos,const std::string& c_textureSource_s, int c_width, int c_height, bool HUDorGUI){};
        virtual void GUIChangeSpriteTexture(unsigned int c_nodeName_i, const std::string& c_textureSource_s, bool HUDorGUI){};
        virtual void GUICreateText(int c_id, float c_xPos, float c_yPos, float c_Scale, const std::string& c_textToRender, const std::string& c_fontLocation){};
        void CleanScene();

    private:
        IrrlichtDevice      * m_irrDevice_p;
        video::IVideoDriver * m_irrVideoDriver_p;
        std::unique_ptr<SceneManager> m_SceneManager_p;
        IrrIMGUI::IIMGUIHandle* debugGUI;
        unsigned int m_screenWidth;
        unsigned int m_screenHeight;
        int fps = 0;

};
