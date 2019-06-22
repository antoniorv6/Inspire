/**
 * @file GenesisRender.cpp
 * @author Antonio Ríos Vila  (antoniorv6@gmail.com)
 *         Edward Thomas Owen (contact@edwardowen.es)
 * @brief 
 * @version 0.1
 * @date 2019-03-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <GenesisRender.hpp>
#include <GenesisEngine.hpp>
#include <TNode.hpp>
#include <TSprite.hpp>
#include <TMesh.hpp>
#include <TAnimation.hpp>
#include <TText.hpp>
#include <TBillBoard.hpp>
#include <iostream>
#include <CommonData.hpp>

/**
 * @brief Construct a new Genesis Render:: Genesis Render object
 * 
 */
GenesisRender::GenesisRender(uint32_t l_screenWidth, uint32_t l_screenHeight)
{

    InitGenesis(l_screenWidth, l_screenHeight);

    Reserver4Meshes(200);

    AddSkybox
    ({
        "assets/Textures/sun_right.jpg",
        "assets/Textures/sun_left.jpg",
        "assets/Textures/sun_up.jpg",
        "assets/Textures/sun_down.jpg",
        "assets/Textures/sun_front.jpg",
        "assets/Textures/sun_back.jpg"
    });

    AddShader("base", "assets/shaders/shader");
    AddShader("lightShader", "assets/shaders/lightShader");
    AddShader("billboards", "assets/shaders/billboard");
    AddShader("outline", "assets/shaders/outline");
    AddShader("skybox", "assets/shaders/skybox");
    AddShader("sprites", "assets/shaders/sprites");
    AddShader("defaultPP", "assets/shaders/defaultPP");
    AddShader("nitro", "assets/shaders/nitro");
    AddShader("text", "assets/shaders/text");
    AddShader("glitch", "assets/shaders/glitch");
    AddShader("freeze", "assets/shaders/freeze");
    AddShader("magnet", "assets/shaders/magnet");
    AddShader("mirror", "assets/shaders/mirror");
    AddShader("thief", "assets/shaders/thief");
    AddShader("shield", "assets/shaders/shield");
    AddShader("particles", "assets/shaders/particles");
    AddShader("charging", "assets/shaders/charging");

    LoadTexture
    ({
        "1.png",
        "2.png",
        "3.png",
        "first.png",
        "second.png",
        "third.png",
        "fourth.png",
        "textura_ia.png",
        "mid_nitro.png",
        "semi_full_bar.png",
        "full_bar.png",
        "energy_bar.png",
        "energy_bar_charged.png",
        "shield_selector.png",
        "magnet_selector.png",
        "thief_selector.png",
        "cyberattack_selector.png",
        "mirror_selector.png",
        "freezer_selector.png",
        "cyberattack0_disabled.png",
        "cyberattack1_ready.png",
        "cyberattack2_being_used.png",
        "freezer0_disabled.png",
        "freezer1_ready.png",
        "freezer2_one.png",
        "freezer3_two.png",
        "freezer4_three.png",
        "magnet0_disabled.png",
        "magnet1_ready.png",
        "magnet2_effective.png",
        "mirror0_disabled.png",
        "mirror1_ready.png",
        "mirror2_being_used.png",
        "shield0_disabled.png",
        "shield1_ready.png",
        "shield2_one.png",
        "shield3_two.png",
        "shield4_three.png",
        "thief0_disabled.png",
        "thief1_ready.png",
        "thief2_effective.png",
        
    });

    AddFont("assets/fonts/BigNoodleTooOblique.ttf");

    m_engine_ptr->InitPostProccesing();
    m_engine_ptr->ActivatePPEffect("defaultPP");
}

/**
 * @brief Destroy the Genesis Render:: Genesis Render object
 * 
 */
GenesisRender::~GenesisRender()
{
    delete m_engine_ptr;
    m_engine_ptr = nullptr;
}

/**
 * @brief Construct a new Genesis Render:: Init Genesis object
 * 
 */
void GenesisRender::InitGenesis(uint32_t l_screenWidth, uint32_t l_screenHeight)
{
    m_engine_ptr = new GenesisEngine(l_screenWidth, l_screenHeight);
    m_engine_ptr->CreateRootNode();
}

/**
 * @brief deletes the scene's tree and clears all the vectors.
 * 
 */
void GenesisRender::CleanScene()
{
    m_engine_ptr->ClearSceneTree();

    m_meshes_m.clear();
    m_cameras_m.clear();
    m_lights_m.clear();
    m_HUDElements_m.clear();
    m_GUIElements_m.clear();
    m_TextElements_m.clear();
    m_Billboards_m.clear();
    //m_shaderList.clear();
}


/**
 * @brief checks if window still opened.
 * 
 */
bool GenesisRender::IsWindowOpened()
{
    return m_engine_ptr->IsWindowOpen();
}

/**
 * @brief closes glfw window.
 * 
 */
void GenesisRender::CloseWindow()
{
    m_engine_ptr->CloseWindow();
}

/**
 * @brief updates Genesis Engine's main loop.
 * 
 */
void GenesisRender::Render()
{
    m_engine_ptr->Update();
}

/**
 * @brief 
 * 
 */
void GenesisRender::DrawGUI()
{
    m_engine_ptr->RenderGUIOnly();
}

/**
 * @brief updates cameras target and position.
 * 
 * @param c_nodeName_s 
 * @param pos 
 * @param newPos 
 */
void GenesisRender::UpdateCameraTarget(unsigned int c_nodeName_s, const glm::vec3& l_cameraTarget_v3, const glm::vec3& l_cameraPosition_v3)
{
    m_engine_ptr->UpdateCameraTarget(ChangeAxisOrientation(l_cameraPosition_v3), ChangeAxisOrientation(l_cameraTarget_v3));
}

/**
 * @brief 
 * 
 * @param l_angle 
 */
void GenesisRender::SetPerspectiveAngle(double l_angle)
{
    m_engine_ptr->SetPerspectiveAngle(l_angle);
}

/**
 * @brief translates any scene node.
 * 
 * @param c_nodeName_s 
 * @param c_nodeNewPosition_vec3 
 */
void GenesisRender::TranslateSceneNode(unsigned int c_nodeName_s, const glm::vec3& c_nodeNewPosition_vec3)
{
    m_engine_ptr->TranslateSceneNode(m_meshes_m[c_nodeName_s], ChangeAxisOrientation(c_nodeNewPosition_vec3));
}

/**
 * @brief rotates any scene node.
 * 
 * @param c_nodeName_s 
 * @param c_nodeNewRotation_vec3 
 */
void GenesisRender::RotateSceneNode(unsigned int c_nodeName_s, const glm::vec3& c_nodeNewRotation_vec3)
{
    m_engine_ptr->RotateSceneNode(m_meshes_m[c_nodeName_s], ChangeRotationOrientation(c_nodeNewRotation_vec3));
}

/**
 * @brief scales any scene node.
 * 
 * @param c_nodeName_s 
 * @param scaleFactor 
 */
void GenesisRender::ScaleSceneNode(unsigned int c_nodeName_s, float scaleFactor)
{
    m_engine_ptr->ScaleSceneNode(m_meshes_m[c_nodeName_s], glm::vec3(scaleFactor, scaleFactor, scaleFactor));
}

/**
 * @brief creates a new scene node with a mesh associated.
 * 
 */
bool GenesisRender::AddMesh(unsigned int c_nodeName_s, const glm::vec3& c_initial_position_vec3, const std::string& route, int c_detailed)
{
    TNode* node = m_engine_ptr->AddMesh(route, nullptr, c_detailed);

    if(node)
    {
        m_engine_ptr->TranslateSceneNode(node, ChangeAxisOrientation(c_initial_position_vec3));
        m_meshes_m[c_nodeName_s] = node;
    }

    return node != NULL;
}

/**
 * @brief 
 * 
 * @param l_position 
 */
void GenesisRender::AddLodMesh(unsigned int c_nodeName_s, const std::string& l_LODName, const glm::vec3& l_position)
{
    TNode* node = m_engine_ptr->AddLodMesh();
    std::vector<std::string> l_namesVector = RenderUtilities::getInstance()->GetLODNamesVector(l_LODName, 3);

    if(node)
    {
        m_engine_ptr->TranslateSceneNode(node, ChangeAxisOrientation(l_position));
        m_meshes_m[c_nodeName_s] = node;
    }

    for(uint32_t i = 0; i < l_namesVector.size(); i++)
    {
        m_engine_ptr->SetLod(node, l_namesVector[i]);
    }
}

/**
 * @brief creates a new scene node with an animated mesh associated.
 * 
 * @param c_nodeName_s 
 * @param c_initial_position_vec3 
 * @param l_frames 
 * @return true 
 * @return false 
 */
bool GenesisRender::AddAnimatedMesh(unsigned int c_nodeName_s, const glm::vec3& c_initial_position_vec3)
{
    TNode * node = m_engine_ptr->AddAnimatedMesh();

    if(node)
    {
        m_engine_ptr->TranslateSceneNode(node, ChangeAxisOrientation(c_initial_position_vec3));
        m_meshes_m[c_nodeName_s] = node;
    }

    return false;
}

/**
 * @brief sets a new animated secuence to an existing Animated Mesh.
 * 
 * @param c_nodeName 
 * @param l_secuenceName 
 * @param l_frames 
 * @param l_loop 
 */
void GenesisRender::AddAnimatedSecuence(unsigned int c_nodeName, const std::string& l_secuenceName, const std::vector<std::string>& l_frames, bool l_loop)
{
    m_engine_ptr->AddAnimatedSecuence(m_meshes_m[c_nodeName], l_secuenceName, l_frames, l_loop);
}

/**
 * @brief sets a new animated secuence to an existing Animated Mesh.
 * 
 * @param c_nodeName 
 * @param l_secuenceName 
 * @param l_frames 
 * @param l_loop 
 */
void GenesisRender::AddAnimatedSecuence(unsigned int c_nodeName, const std::string& l_secuenceName, const std::vector<std::vector<std::string>>& l_frames, bool l_loop)
{
    m_engine_ptr->AddAnimatedSecuence(m_meshes_m[c_nodeName], l_secuenceName, l_frames, l_loop);
}

/**
 * @brief changes the animated secuence of an animated mesh.
 * 
 * @param c_nodeName 
 * @param l_secuenceName 
 */
void GenesisRender::SetSecuence(unsigned int c_nodeName, const std::string& l_secuenceName)
{
    TAnimation * animation = reinterpret_cast<TAnimation*>(m_meshes_m[c_nodeName]->GetEntity());
    animation->SetSecuence(l_secuenceName);
}


/**
 * @brief creates a new Shader on Genesis Engine, given its existing files.
 *        also saves the returned shader in a map for usage purposes.
 * 
 * @param l_shaderDir_str 
 */
void GenesisRender::AddShader(const std::string& l_shaderId, const std::string& l_shaderDir_str)
{
    m_shaderList[l_shaderId] = m_engine_ptr->AddShader(l_shaderId, l_shaderDir_str);
}

/**
 * @brief 
 * 
 * @param l_shaderName 
 * @param l_meshName 
 * @param l_textureSource 
 * @param l_position 
 * @param l_particlesAmount 
 */
void GenesisRender::AddParticlesEmitter
(
    const std::string& l_meshName, 
    const std::string& l_textureSource, 
    const glm::vec3& l_position, 
    uint32_t l_particlesAmount
){
    m_engine_ptr->AddParticlesEmitter
    (
        l_meshName, 
        l_textureSource, 
        ChangeAxisOrientation(l_position), 
        l_particlesAmount
    );
}

/**
 * @brief 
 * 
 * @param l_effectId 
 */
void GenesisRender::ActivatePPEffect(const std::string& l_effectId)
{
    m_engine_ptr->ActivatePPEffect(l_effectId);
}

/**
 * @brief Get the Effect I D object
 * 
 * @param l_effectName 
 * @return std::string 
 */
std::string GenesisRender::GetEffectID()
{
    return m_engine_ptr->GetEffectID();
}

/**
 * @brief creates a Skybox, given all the (6) faces images (directories) of the cubemap.
 * 
 * @param l_facesDir 
 */
void GenesisRender::AddSkybox(const std::vector<std::string>& l_facesDir)
{
    m_engine_ptr->AddSkybox(l_facesDir);
}


/**
 * @brief creates a new scene node with a light associated.
 * 
 * @param l_lightPosition_v3 
 */
void GenesisRender::AddLight(const glm::vec3& l_lightPosition_v3, const glm::vec3& l_color, const glm::vec3& l_attenuation)
{
    TNode * light = m_engine_ptr->AddLight(l_color, l_attenuation);

    if(light)
    {   
        m_lightsCount_n++;
        m_engine_ptr->TranslateSceneNode(light, ChangeAxisOrientation(l_lightPosition_v3));
        m_engine_ptr->ScaleSceneNode(light, glm::vec3(25.0, 25.0, 25.0));
        m_lights_m[m_lightsCount_n] = light;
    }
}

/**
 * @brief creates a new scene node with a camera associated.
 * 
 * @param c_cameraName_s 
 * @param c_cameraPosition_vec3 
 * @param c_cameraLookAt_vec3 
 */
bool GenesisRender::AddCamera(unsigned int c_cameraName_s,  const glm::vec3& c_cameraPosition_vec3, const glm::vec3& c_cameraLookAt_vec3)
{
    TNode * camera = m_engine_ptr->AddCamera();

    if(camera)
    {
        m_engine_ptr->SetActiveCamera(0);
        m_engine_ptr->UpdateCameraTarget(ChangeAxisOrientation(c_cameraPosition_vec3), 
                                         ChangeAxisOrientation(c_cameraLookAt_vec3));
        m_cameras_m[c_cameraName_s] = camera;
    }

    return camera != NULL;
}

/**
 * @brief sets any texture to an existing scene node.
 * 
 * @param c_nodeName_s 
 * @param c_texturePath_s 
 * @return true 
 * @return false 
 */
bool GenesisRender::AddTexture(unsigned int c_nodeName_s, const std::string& c_texturePath_s)
{
    if(m_meshes_m[c_nodeName_s])
    {
        m_engine_ptr->SetNodeTexture(m_meshes_m[c_nodeName_s], c_texturePath_s);
        return true;
    }

    return false;
}

/**
 * @brief sets any texture to every frame of a given animated secuence.
 * 
 * @param c_nodeName_s 
 * @param c_texturePath_s 
 * @param l_secuence 
 * @return true 
 * @return false 
 */
bool GenesisRender::AddTexture(unsigned int c_nodeName_s, const std::string& c_texturePath_s, const std::string& l_secuence)
{
    if(m_meshes_m[c_nodeName_s])
    {
        m_engine_ptr->SetNodeTexture(m_meshes_m[c_nodeName_s], c_texturePath_s, l_secuence);
        return true;
    }

    return false;
}

void GenesisRender::SetAnimationFrameRate(unsigned int c_node, float c_newFrameRate)
{
    if(m_meshes_m[c_node])
    {
        m_engine_ptr->SetAnimationFrameRate(m_meshes_m[c_node], c_newFrameRate);
    }
}

/**
 * @brief charges a new texture on Genesis Engine for a future usage.
 * 
 * @param l_textureDir 
 */
void GenesisRender::LoadTexture(const std::string& l_textureDir)
{
    m_engine_ptr->LoadTexture(l_textureDir);
}

/**
 * @brief charges new textures on Genesis Engine for a future usage.
 * 
 * @param l_texturesDir 
 */
void GenesisRender::LoadTexture(const std::vector<std::string>& l_texturesDir)
{
    m_engine_ptr->LoadTexture(l_texturesDir);
}

/**
 * @brief charges a new font on Genesis Engine for a future usage.
 * 
 * @param l_fontDir 
 */
void GenesisRender::AddFont(const std::string& l_fontDir)
{
    m_engine_ptr->LoadFont(l_fontDir);
}

/**
 * @brief charges new fonts on Genesis Engine for a future usage.
 * 
 * @param l_fontsDir 
 */
void GenesisRender::AddFont(const std::vector<std::string>& l_fontsDir)
{
    m_engine_ptr->LoadFont(l_fontsDir);
}

/**
 * @brief reserves memory for all the meshes.
 * 
 * @param l_memoryCount_st 
 */
void  GenesisRender::Reserver4Meshes(size_t l_memoryCount_st)
{
    m_engine_ptr->Reserver4Meshes(l_memoryCount_st);
}

/**
 * @brief Irrlicht to Genesis Engine's axis orientation (on rotations).
 * 
 * @param c_gameVector_v3 
 * @return glm::vec3 
 */
glm::vec3 GenesisRender::ChangeAxisOrientation(const glm::vec3& c_gameVector_v3)
{
    return glm::vec3(-c_gameVector_v3.x, c_gameVector_v3.z, -c_gameVector_v3.y);
}

/**
 * @brief Irrlicht to Genesis Engine's axis orientation.
 * 
 * @param c_gameVector_v3 
 * @return glm::vec3 
 */
glm::vec3 GenesisRender::ChangeRotationOrientation(const glm::vec3& c_gameVector_v3)
{
    return glm::vec3(c_gameVector_v3.x, -c_gameVector_v3.z, c_gameVector_v3.y);
}

/**
 * @brief 
 * 
 * @param c_nodeName_i 
 * @param c_xPos 
 * @param c_yPos 
 * @param c_textureSource_s 
 * @param c_width 
 * @param c_height 
 * @param HUDorGUI 
 */
void GenesisRender::GUICreateSprite(unsigned int c_nodeName_i, int c_xPos, int c_yPos, const std::string& c_textureSource_s, int c_width, int c_height, bool HUDorGUI)
{
    TSprite* l_billboard = m_engine_ptr->CreateSprite(c_textureSource_s, c_width, c_height, c_xPos, c_yPos);
    if(HUDorGUI)
        m_HUDElements_m[c_nodeName_i] = l_billboard;
    else
        m_GUIElements_m[c_nodeName_i] = l_billboard;
    
}

/**
 * @brief 
 * 
 * @param c_nodeName_i 
 * @param c_width 
 * @param c_height 
 * @param HUDorGUI 
 */
void GenesisRender::GUIScaleSprite(unsigned int c_nodeName_i, int c_width, int c_height, bool HUDorGUI)
{
    if(HUDorGUI)
        m_engine_ptr->ScaleSprite(m_HUDElements_m[c_nodeName_i], c_width, c_height);
    else
         m_engine_ptr->ScaleSprite(m_GUIElements_m[c_nodeName_i], c_width, c_height);
    
}

/**
 * @brief 
 * 
 * @param c_nodeName_i 
 * @param c_textureSource_s 
 * @param HUDorGUI 
 */
void GenesisRender::GUIChangeSpriteTexture(unsigned int c_nodeName_i, const std::string& c_textureSource_s, bool HUDorGUI)
{
    if(HUDorGUI)
        m_engine_ptr->SetSpriteTexture(m_HUDElements_m[c_nodeName_i], c_textureSource_s);
    else
        m_engine_ptr->SetSpriteTexture(m_GUIElements_m[c_nodeName_i], c_textureSource_s);

}

/**
 * @brief 
 * 
 * @param c_howMany_n 
 */
void GenesisRender::GUIEraseSprite(int c_howMany_n)
{   
    m_engine_ptr->EraseSprites(c_howMany_n);
    //m_GUIElements_m.clear();
}

/**
 * @brief 
 * 
 * @return int 
 */
int GenesisRender::GetScreenWidth()
{
    return m_engine_ptr->GetScreenWidth();
}

/**
 * @brief 
 * 
 * @return int 
 */
int GenesisRender::GetScreenHeight()
{
    return m_engine_ptr->GetScreenHeight();
}

/**
 * @brief 
 * 
 * @param c_id 
 * @param c_xPos 
 * @param c_yPos 
 * @param c_Scale 
 * @param c_textToRender 
 * @param c_fontLocation 
 */
void GenesisRender::GUICreateText(int c_id, float c_xPos, float c_yPos, float c_Scale, const std::string& c_textToRender, const std::string& c_fontLocation)
{
    TText* l_newText = m_engine_ptr->CreateText(c_xPos, c_yPos, c_Scale, c_textToRender, c_fontLocation);
    m_TextElements_m[c_id] = l_newText;
}

/**
 * @brief 
 * 
 * @param c_name_s 
 * @param c_initialPosition_v3dirr 
 * @param c_initialRotation_v3dirr 
 * @param c_initialScale_v3dirr 
 */
void GenesisRender::AddDebugCube(unsigned int c_name_s, const glm::vec3& c_initialPosition_v3dirr, const glm::vec3& c_initialRotation_v3dirr, const glm::vec3& c_initialScale_v3dirr)
{
    TNode * node = m_engine_ptr->AddMesh("debug_cube");
    m_engine_ptr->TranslateSceneNode(node, c_initialPosition_v3dirr);
    m_engine_ptr->RotateSceneNode(node, c_initialRotation_v3dirr);
    m_engine_ptr->ScaleSceneNode(node, c_initialScale_v3dirr);
    m_engine_ptr->SetNodeTexture(node, "plaino.jpg");
}

/**
 * @brief 
 * 
 * @param c_id 
 * @param c_billBoardTexture_str 
 * @param l_width_f 
 * @param l_height_f 
 * @param c_initialPosition 
 */
void GenesisRender::CreateBillboard(int c_id, const std::string& c_billBoardTexture_str, float l_width_f, float l_height_f, glm::vec3& c_initialPosition)
{
    TBillBoard* l_newBillboard = (TBillBoard*)m_engine_ptr->CreateBillBoard(c_billBoardTexture_str, l_width_f, l_height_f);
    m_engine_ptr->TranslateBillBoard(l_newBillboard,ChangeAxisOrientation(c_initialPosition));
    m_Billboards_m[c_id] = l_newBillboard;
}

/**
 * @brief 
 * 
 * @param c_id 
 * @param c_status 
 * @param HUDorGUI 
 */
void GenesisRender::SetSpriteActiveStatus(unsigned int c_id, bool c_status, bool HUDorGUI)
{
    if(HUDorGUI)
        m_engine_ptr->SetSpriteStatus(m_HUDElements_m[c_id], c_status);
    else
        m_engine_ptr->SetSpriteStatus(m_GUIElements_m[c_id], c_status);
}

/**
 * @brief 
 * 
 * @param c_id 
 * @param c_newPosition 
 */
void GenesisRender::TranslateBillboard(int c_id, glm::vec3& c_newPosition)
{
    m_engine_ptr->TranslateBillBoard(m_Billboards_m[c_id], ChangeAxisOrientation(c_newPosition));
}