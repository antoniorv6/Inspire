
#pragma once
#include <glm.hpp>
#include <string>
#include <vector>

class IRenderFacade 
{
    public:
        IRenderFacade(){};
        virtual ~IRenderFacade(){};
        virtual void Render() = 0;
        virtual void DrawGUI() = 0;
        virtual bool IsWindowOpened() = 0;
        virtual void CloseWindow() = 0;
        virtual void DrawBBOX(unsigned int c_nodeName_s) = 0;
        
        virtual void UpdateCameraTarget(unsigned int c_nodeName_s, const glm::vec3& pos, const glm::vec3& newPos) = 0;
        virtual void SetPerspectiveAngle(double l_angle) = 0;

        virtual bool AddNode(unsigned int c_nodeName_s, const glm::vec3& c_initial_position_vec3) = 0;
        virtual bool AddMesh(unsigned int c_nodeName_s, const glm::vec3& c_initial_position_vec3, const std::string& route, int c_detailed = 0) = 0;
        virtual bool AddAnimatedMesh(unsigned int c_nodeName_s, const glm::vec3& c_initial_position_vec3) = 0;
        virtual void AddAnimatedSecuence(unsigned int c_nodeName, const std::string& l_secuenceName, const std::vector<std::string>& l_frames, bool l_loop) = 0;
        virtual void AddAnimatedSecuence(unsigned int c_nodeName, const std::string& l_secuenceName, const std::vector<std::vector<std::string>>& l_frames, bool l_loop) = 0;
        virtual void SetAnimationFrameRate(unsigned int c_node, float c_newFrameRate) = 0;
        virtual void AddLodMesh(unsigned int c_nodeName_s, const std::string& l_LODName, const glm::vec3& l_position) = 0;
        virtual void SetSecuence(unsigned int c_nodeName, const std::string& l_secuenceName) = 0;
        virtual bool AddCamera(unsigned int c_cameraName_s,  const glm::vec3& c_cameraPosition_vec3, const glm::vec3& c_cameraLookAt_vec3) = 0;
        virtual void AddShader(const std::string& l_shaderId, const std::string& l_shaderDir_str) = 0;
        virtual void ActivatePPEffect(const std::string& l_effectId) = 0;
        virtual void AddParticlesEmitter(const std::string& l_meshName, const std::string& l_textureSource, const glm::vec3& l_position, uint32_t l_particlesAmount) = 0;
        virtual void AddLight(const glm::vec3& l_lightPosition_v3, const glm::vec3& l_color, const glm::vec3& l_attenuation) = 0;
        virtual void TranslateSceneNode(unsigned int c_nodeName_s, const glm::vec3& c_nodeNewPosition_vec3) = 0;
        virtual void RotateSceneNode(unsigned int c_nodeName_s, const glm::vec3& c_nodeNewRotation_vec3) = 0;
        virtual void ScaleSceneNode(unsigned int c_nodeName_s, float scaleFactor) = 0;
        virtual bool AddTexture(unsigned int c_nodeName_s, const std::string& c_texturePath_s) = 0;
        virtual bool AddTexture(unsigned int c_nodeName_s, const std::string& c_texturePath_s, const std::string& l_secuence) = 0;

        virtual std::string GetEffectID() = 0;

        virtual void BeginScene() = 0;
        virtual void EndScene() = 0;

        virtual void DebugDraw2DLine(const glm::vec3& c_origin_v3, const glm::vec3& c_destiny_v3, unsigned int color) = 0;
        virtual void DebugDraw2DPoint(const glm::vec3& c_point_v3, unsigned int color) = 0;
        virtual void AddDebugNode(const std::string& c_nodeName_s, unsigned int id, const glm::vec3& c_initial_postiion_vec3) = 0;
        virtual void ScaleDebugNode(unsigned int c_nodeName_s, float c_scaleFactor_f) = 0;
        virtual void AddDebugCube(unsigned int c_name_s, const glm::vec3& c_initialPosition_v3dirr, const glm::vec3& c_initialRotation_v3dirr, const glm::vec3& c_initialScale_v3dirr) = 0;
        virtual void AddDebugSphere(unsigned int c_id_ui, const glm::vec3& c_pos_v3, float c_rad_f) = 0;
        virtual void TranslateDebugSceneNode(unsigned int id, const glm::vec3& pos) = 0;

        virtual int GetScreenWidth() = 0;
        virtual int GetScreenHeight() = 0;

        virtual void GUIDrawBox(int c_xPos, int c_yPos, int c_width, int c_height, int c_Red, int c_Blue, int c_Green) = 0;
        virtual void GUIDrawSprite(const std::string& c_SpriteRoute, int c_xPos, int c_yPos) = 0;
        virtual void GUIDraw2DPoint(int c_xPos, int c_YPos) = 0;

        virtual void GUICreateSprite(unsigned int c_nodeName_i, int c_xPos, int c_yPos, const std::string& c_textureSource_s, int c_width, int c_height, bool HUDorGUI) = 0;
        virtual void GUIChangeSpriteTexture(unsigned int c_nodeName_i, const std::string& c_textureSource_s, bool HUDorGUI) = 0;
        virtual void GUIScaleSprite(unsigned int c_nodeName_i, int c_width, int c_height, bool HUDorGUI) = 0;
        virtual void GUIEraseSprite(int c_howMany_n) = 0;
        virtual void GUICreateText(int c_id, float c_xPos, float c_yPos, float c_Scale, const std::string& c_textToRender, const std::string& c_fontLocation)=0;
        virtual void SetSpriteActiveStatus(unsigned int c_id, bool c_status, bool HUDorGUI) = 0;

        virtual void CreateBillboard(int c_id, const std::string& c_billBoardTexture_str, float l_width_f, float l_height_f, glm::vec3& c_initialPosition)=0;
        virtual void TranslateBillboard(int c_id, glm::vec3& c_newPosition)=0;

        
        virtual void Reserver4Meshes(size_t l_memoryCount_st) = 0;

        virtual void CleanScene() = 0;
};