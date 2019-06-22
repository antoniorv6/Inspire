#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <map>

/**
 * Forward declarations
 */
class TMesh;
class TResourceMesh;
class TResourceShader;
class TNode;
class TCamera;
class TTransform;
class TLight;
class ResourceManager;
class TBillBoard;
class TSkybox;
class TSprite;
class TText;
class PostProcessing;
class Clock;
class ParticlesEmitter;

enum ShaderTypes
{
    Base = 0,
    DebugLights = 1,
    BillBoards = 2,
    Outline = 3,
    Skybox = 4,
    Sprites = 5,
    DefaultPP = 6,
    Text    = 7
};

using mSkybox = TSkybox*;

class GenesisEngine{
    friend bool SortCameras(unsigned int l_cameraId_n);
    friend void GeneratePPBuffers(GenesisEngine * genesis, int l_screenWidth, int l_screenHeight);
    friend class ParticlesEmitter;
    public:
        GenesisEngine(int l_screenWidth_n, int l_screenHeight_n);
        ~GenesisEngine();
        void Update();
        void Draw(float dt);
        void DrawGUI();

        /**
         *  WINDOW
         */
        int CreateWindow(int l_screenWidth_n, int l_screenHeight_n);
        bool IsWindowOpen() { return !glfwWindowShouldClose(window); }
        static void Resize(GLFWwindow* window, int width, int height);
        int GetScreenWidth()  {  return bufferWidth;  };
        int GetScreenHeight() {  return bufferHeight; };
        void CloseWindow();

        /**
         * SCENE
         */
        void CreateRootNode();
        void SetDrawValues();
        TNode* AddTransformNodes(TNode* c_father_ptr);

        /**
         * CAMERA
         */
        TNode* AddCamera();
        void DoCameras();
        void SetActiveCamera(unsigned int l_cameraId_n);
        void UpdateCameraTarget(const glm::vec3& c_cameraPosition_v3, const glm::vec3& c_cameraTarget_v3);
        void SetPerspectiveAngle(double l_angle);
        glm::mat4 CalculateMatrix(TNode * l_activeNode_ptr, int l_operationType_n);

        /**
         * LIGHT
         */
        TNode* AddLight(const glm::vec3& l_color, const glm::vec3& l_attenuation);
        void DoLights();
        void RenderLights();
        void SetLightModelMatrix(unsigned int l_lightId_n, const glm::mat4& l_modelMatrix);

        /**
         * MESH
         */
        TNode* AddMesh(const std::string& l_meshName_str, TNode* c_father_ptr = nullptr, int c_HasOutline = 0);
        TNode* AddAnimatedMesh(TNode* c_father_ptr = nullptr);
        TNode* AddLodMesh(TNode * c_father_ptr = nullptr);
        void Reserver4Meshes(size_t l_memoryCount_st){ m_mesh_v.reserve(l_memoryCount_st); };
        void AddAnimatedSecuence(TNode* node, const std::string& l_secuenceName, const std::vector<std::string>& l_frames, bool l_loop);
        void AddAnimatedSecuence(TNode * node, const std::string& l_secuenceName, const std::vector<std::vector<std::string>>& l_framesMatrix, bool l_loop);
        void SetLod(TNode * node, const std::string& l_meshName);
        void UpdateAnimations();
        void SetAnimationFrameRate(TNode* c_node, float c_newFrameRate);

        /**
         * BILLBOARDS
         */
        TBillBoard* CreateBillBoard(const std::string& c_billBoardTexture_str, float l_width_f, float l_height_f);
        void TranslateBillBoard(TBillBoard* c_affectedBillBoard_ptr, const glm::vec3& c_newPosition_v3);
        void ChangeBillBoardTexture(TBillBoard* c_affectedBillBoard_ptr, std::string& c_billBoardTexture_str);
        void RenderBillBoards(); 

        /**
         * SKYBOX
         */
        void AddSkybox(const std::vector<std::string>& l_boxImagesPath);
        void RenderSkybox();

        /**
         *  2D SPRITES 
         */
        TSprite* CreateSprite(const std::string& c_newTexture_string, int c_width, int c_height, int c_xPos, int c_yPos);
        void ScaleSprite(TSprite* c_sprite, int c_width, int c_height);
        void RenderSprites();
        void EraseSprites(int l_howmany);
        void SetSpriteStatus(TSprite* c_sprite, bool c_status);

        /**
         * TRANSFORM METHODS
         */
        void TranslateSceneNode(TNode * l_node_ptr, const glm::vec3& l_translation_v3);
        void RotateSceneNode(TNode * l_node_ptr, const glm::vec3& l_rotation_v3);
        void ScaleSceneNode(TNode * l_node_ptr, const glm::vec3& l_scale_v3);

        /** 
         * TEXTURE HANDLING
        */
        void SetNodeTexture(TNode* l_node_ptr, const std::string& l_texturePath_s);
        void SetNodeTexture(TNode* l_node_ptr, const std::string& l_texturePath_s, const std::string& l_secuence);
        void SetSpriteTexture(TSprite* l_sprite_ptr, const std::string& l_texturePath_s);
        void LoadTexture(const std::string& l_sourceName_s);
        void LoadTexture(const std::vector<std::string>& l_sourceNameS);

        /**
         * TEXT
         */
        TText* CreateText(float c_xPos, float c_yPos, float c_Scale, const std::string& c_textToRender, const std::string& c_fontLocation);
        void RenderText();
        void LoadFont(const std::string& l_fontPath_s);
        void LoadFont(const std::vector<std::string>& l_fontsPaths_s);

        /**
         *  SHADERS
         */
        TResourceShader* AddShader(const std::string& l_shaderName, const std::string& l_shaderDirectory_s);

        /**
         *  POST-PROCCESING
         */
        void InitPostProccesing();
        void ActivatePPEffect(const std::string& l_effectId);
        std::string GetEffectID();

        /**
         *  PARTICLES
         */
        void AddParticlesEmitter(const std::string& l_meshName, const std::string& l_textureSource, const glm::vec3& l_position, uint32_t l_particlesAmount);
        void UpdateParticles(float dt);
        void RenderParticles(float dt);
        void DeleteParticles();

        /**
         * CLEAR
         */
        void Close();
        void ClearSceneTree();

        /**
         *  SPECIAL FUNCTION FOR GUI
         */
        void RenderGUIOnly();

    private:
        int m_screenWidth = 0, m_screenHeight = 0;
        int bufferWidth = 0, bufferHeight = 0;
        float dt = 0.0f;

        GLFWwindow      * window                       = nullptr;
        Clock           * m_clock_ptr                  = nullptr;
        ResourceManager * m_resourceManager_ptr        = nullptr;
        TNode           * sceneRootNode                = nullptr;
        TNode           * m_activeCamera_ptr           = nullptr;
        TSkybox         * m_skybox_ptr                 = nullptr;
        TResourceShader * m_postProccessingShader_ptr  = nullptr;
        PostProcessing  * m_postProcessingUnit_ptr     = nullptr;

        GLint  success;
        GLchar infoLog[512];
        GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
        
        std::vector<TResourceShader*> shaderList;
        std::vector<TNode*>           m_mesh_v;
        std::vector<TNode*>           m_animatedMeshes_v;
        std::vector<TNode*>           m_LODMeshes;
        std::vector<TNode*>           m_camera_v;
        std::vector<TNode*>           m_light_v;
        std::vector<TBillBoard*>      m_billBoards_v;
        std::vector<TSprite*>         m_sprites_v;
        std::vector<TText*>           m_text_v;
        std::vector<ParticlesEmitter*> m_particles;

        std::map<std::string, TResourceShader*> m_shadersMap;

        glm::mat4 m_spritesProjection_m4 = glm::mat4(1.0);
        glm::mat4 m_textProjection_m4    = glm::mat4(1.0);

        uint32_t m_LODLevels = 3;
        std::vector<float> m_LODDistances;
};
