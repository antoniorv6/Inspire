/**
 * @file GenesisRender.hpp
 * @author Antonio Ríos Vila  (antoniorv6@gmail.com)
 *         Edward Thomas Owen (contact@edwardowen.es)
 * @brief 
 * @version 0.1
 * @date 2019-03-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <IRenderFacade.h>
#include <map>

class GenesisEngine;
class TNode;
class TSprite;
class TBillBoard;
class TText;
class TAnimation;
class TResourceShader;

class GenesisRender : public IRenderFacade
{
public:
    GenesisRender(uint32_t l_screenWidth, uint32_t l_screenHeight);
    virtual ~GenesisRender();

    // === INITIALIZATION === //
    void InitGenesis(uint32_t l_screenWidth, uint32_t l_screenHeight);
    virtual void Render();
    virtual void DrawGUI();
    virtual void BeginScene(){};
    virtual void EndScene(){};
    virtual void CleanScene();

    // === WINDOW === //
    virtual bool IsWindowOpened();
    virtual void CloseWindow();
    virtual int GetScreenWidth();
    virtual int GetScreenHeight();

    // === MESHES - ANIMATIED MESHES === //
    virtual bool AddNode(unsigned int c_nodeName_s, const glm::vec3& c_initial_position_vec3){ return true; };
    virtual bool AddMesh(unsigned int c_nodeName_s, const glm::vec3& c_initial_position_vec3, const std::string& route, int c_detailed = 0);
    virtual bool AddAnimatedMesh(unsigned int c_nodeName_s, const glm::vec3& c_initial_position_vec3);
    virtual void AddLodMesh(unsigned int c_nodeName_s, const std::string& l_LODName, const glm::vec3& l_position);
    virtual void AddAnimatedSecuence(unsigned int c_nodeName, const std::string& l_secuenceName, const std::vector<std::string>& l_frames, bool l_loop);
    virtual void AddAnimatedSecuence(unsigned int c_nodeName, const std::string& l_secuenceName, const std::vector<std::vector<std::string>>& l_frames, bool l_loop);
    virtual void Reserver4Meshes(size_t l_memoryCount_st);
    virtual void SetSecuence(unsigned int c_nodeName, const std::string& l_secuenceName);
    virtual void SetAnimationFrameRate(unsigned int c_nodeName, float c_frameRate);
    
    // === TEXTURES - FONTS === //
    virtual bool AddTexture(unsigned int c_nodeName_s, const std::string& c_texturePath_s);
    virtual bool AddTexture(unsigned int c_nodeName_s, const std::string& c_texturePath_s, const std::string& l_secuence);
    void LoadTexture(const std::string& l_textureDir);
    void LoadTexture(const std::vector<std::string>& l_texturesDir);
    void AddFont(const std::string& l_fontDir);
    void AddFont(const std::vector<std::string>& l_fontsDir);

    // === SHADERS === //
    virtual void AddShader(const std::string& l_shaderId, const std::string& l_shaderDir_str);
    virtual void ActivatePPEffect(const std::string& l_effectId);
    virtual std::string GetEffectID();
    virtual void AddParticlesEmitter(const std::string& l_meshName, const std::string& l_textureSource, const glm::vec3& l_position, uint32_t l_particlesAmount);

    // === CAMERAS === //
    virtual bool AddCamera(unsigned int c_cameraName_s,  const glm::vec3& c_cameraPosition_vec3, const glm::vec3& c_cameraLookAt_vec3);
    virtual void UpdateCameraTarget(unsigned int c_nodeName_s, const glm::vec3& pos, const glm::vec3& newPos);
    virtual void SetPerspectiveAngle(double l_angle);
    
    // === LIGHTS === //
    virtual void AddLight(const glm::vec3& l_lightPosition_v3, const glm::vec3& l_color, const glm::vec3& l_attenuation);

    // === SKYBOX === //
    void AddSkybox(const std::vector<std::string>& l_facesDir);

    // === TRANSFORMATIONS=== //
    virtual void TranslateSceneNode(unsigned int c_nodeName_s, const glm::vec3& c_nodeNewPosition_vec3);
    virtual void RotateSceneNode(unsigned int c_nodeName_s, const glm::vec3& c_nodeNewRotation_vec3);
    virtual void ScaleSceneNode(unsigned int c_nodeName_s, float scaleFactor);

    // === GUI === //
    virtual void GUICreateSprite(unsigned int c_nodeName_i, int c_xPos, int c_yPos, const std::string& c_textureSource_s, int c_width, int c_height, bool HUDorGUI);
    virtual void GUIChangeSpriteTexture(unsigned int c_nodeName_i, const std::string& c_textureSource_s, bool HUDorGUI);
    virtual void GUIScaleSprite(unsigned int c_nodeName_i, int c_width, int c_height, bool HUDorGUI);
    virtual void GUIEraseSprite(int c_howMany_n);
    virtual void GUICreateText(int c_id, float c_xPos, float c_yPos, float c_Scale, const std::string& c_textToRender, const std::string& c_fontLocation);
    virtual void GUIDrawBox(int c_xPos, int c_yPos, int c_width, int c_height, int c_Red, int c_Blue, int c_Green){};
    virtual void GUIDrawSprite(const std::string& c_SpriteRoute, int c_xPos, int c_yPos){};
    virtual void GUIDraw2DPoint(int c_xPos, int c_YPos){};

    virtual void CreateBillboard(int c_id, const std::string& c_billBoardTexture_str, float l_width_f, float l_height_f, glm::vec3& c_initialPosition);
    virtual void TranslateBillboard(int c_id, glm::vec3& c_newPosition);

    virtual void SetSpriteActiveStatus(unsigned int c_id, bool c_status, bool HUDorGUI);

    // === NOT USE === //
    virtual void DrawBBOX(unsigned int c_nodeName_s){};
    virtual void DebugDraw2DLine(const glm::vec3& c_origin_v3, const glm::vec3& c_destiny_v3, unsigned int color){};
    virtual void DebugDraw2DPoint(const glm::vec3& c_point_v3, unsigned int color){};
    virtual void AddDebugNode(const std::string& c_nodeName_s, unsigned int id, const glm::vec3& c_initial_postiion_vec3){};
    virtual void ScaleDebugNode(unsigned int c_nodeName_s, float c_scaleFactor_f){};
    virtual void AddDebugCube(unsigned int c_name_s, const glm::vec3& c_initialPosition_v3dirr, const glm::vec3& c_initialRotation_v3dirr, const glm::vec3& c_initialScale_v3dirr);
    virtual void AddDebugSphere(unsigned int c_id_ui, const glm::vec3& c_pos_v3, float c_rad_f){};
    virtual void TranslateDebugSceneNode(unsigned int id, const glm::vec3& pos){};

private: 
    GenesisEngine* m_engine_ptr = nullptr;
                     
    int m_lightsCount_n = 0;

    std::map<unsigned int, TNode*>       m_meshes_m; 
    std::map<unsigned int, TNode*>       m_cameras_m; 
    std::map<unsigned int, TNode*>       m_lights_m; 
    std::map<unsigned int, TSprite*>     m_HUDElements_m;
    std::map<unsigned int, TSprite*>     m_GUIElements_m;
    std::map<unsigned int, TText*>       m_TextElements_m; 
    std::map<unsigned int, TBillBoard*>  m_Billboards_m;
    std::map<std::string, TResourceShader*> m_shaderList;

    glm::vec3 ChangeAxisOrientation(const glm::vec3& c_gameVector_v3);
    glm::vec3 ChangeRotationOrientation(const glm::vec3& c_gameVector_v3);
};