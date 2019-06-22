#define GLM_ENABLE_EXPERIMENTAL
#include <GenesisEngine.hpp>
#include <TTransform.hpp>
#include <TNode.hpp>
#include <TMesh.hpp>
#include <TSprite.hpp>
#include <TText.hpp>
#include <TLight.hpp>
#include <TCamera.hpp>
#include <TSkybox.hpp>
#include <TAnimation.hpp>
#include <TBillBoard.hpp>
#include <TResourceMesh.hpp>
#include <TResourceShader.hpp>
#include <ResourceManager.hpp>
#include <PostProcessing.hpp>
#include <ParticlesEmitter.hpp>
#include <gtx/vector_angle.hpp>
#include <InputGenesis.hpp>
#include <TLodMesh.hpp>
#include <Clock.hpp>


void On_Event(GLFWwindow* l_window_ptr, int l_key_n, int l_scanCode_n, int l_action_n, int mods);
void On_MouseEvent(GLFWwindow* window, int button, int action, int mods);
void GetCursorPosition(GLFWwindow* window, double xPos, double yPos);

std::list<GenesisEngine*> registeredGenesis;
void registerGenesisInstance(GenesisEngine * genesis);
void GeneratePPBuffers(GenesisEngine * genesis, int l_screenWidth, int l_screenHeight);

/**
 * @brief Construct a new Genesis Engine:: Genesis Engine object
 * 
 */
GenesisEngine::GenesisEngine(int l_screenWidth_n, int l_screenHeight_n)
{
    m_screenWidth = l_screenWidth_n;
    m_screenHeight = l_screenHeight_n;

    registeredGenesis.push_back(this);

    // Initialise GLFW.
    if(!glfwInit())
    {
        ////std::cout << "GLFW initialization failed!" << std::endl;
        glfwTerminate();
    }
    // === GLFW PROPIERTIES === //

    // OpenGL MINOR supported version.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // OpenGL MAJOR supported version.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // No backwards compatibility.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allows forward compatiblity.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // Crates the GLFW window. 
    CreateWindow(l_screenWidth_n, l_screenHeight_n);
    // Get the Resource Manager. 
    m_resourceManager_ptr = ResourceManager::getInstance();

    m_clock_ptr = new Clock();

    m_activeCamera_ptr = nullptr;

    m_LODDistances = { 300.0f, 500.0f, 800.0f };
}

/**
 * @brief 
 * 
 */
void GenesisEngine::InitPostProccesing()
{
    m_postProcessingUnit_ptr = new PostProcessing(m_screenWidth, m_screenHeight, m_shadersMap);
}

/**
 * @brief 
 * 
 * @param l_effectId 
 */
void GenesisEngine::ActivatePPEffect(const std::string& l_effectId)
{
    m_postProcessingUnit_ptr->ActivateEffect(l_effectId);
}

/**
 * @brief Get the Effect I D object
 * 
 * @param l_effectName 
 * @return std::string 
 */
std::string GenesisEngine::GetEffectID()
{
    return m_postProcessingUnit_ptr->GetEffectID();
}

/**
 * @brief 
 * 
 * @param l_screenWidth 
 * @param l_screenHeight 
 */
void GeneratePPBuffers(GenesisEngine * genesis, int l_screenWidth, int l_screenHeight)
{
    genesis->m_screenWidth = l_screenWidth;
    genesis->m_screenHeight = l_screenHeight;
    genesis->m_postProcessingUnit_ptr->Generate(genesis->m_screenWidth, genesis->m_screenHeight);    
}

/**
 * @brief Destroy the Genesis Engine:: Genesis Engine object
 * 
 */
GenesisEngine::~GenesisEngine()
{
    ClearSceneTree();
    delete sceneRootNode;
    sceneRootNode = nullptr;
    delete m_skybox_ptr;
    m_skybox_ptr = nullptr;
    delete m_postProcessingUnit_ptr;
    m_postProcessingUnit_ptr = nullptr;
    delete m_clock_ptr;
    m_clock_ptr = nullptr;
    m_shadersMap.clear();
}

/**
 * @brief: Deletes the scene Tree.
 * 
 */
void GenesisEngine::ClearSceneTree()
{
    sceneRootNode->ClearList();

    for(auto& billboard : m_billBoards_v)
    {
        delete billboard;
        billboard = nullptr;
    }

    for(auto& sprite: m_sprites_v)
    {
        delete sprite;
        sprite = nullptr;
    }

    for(auto& text : m_text_v)
    {
        delete text;
        text = nullptr;
    }

    DeleteParticles();
    
    
    std::vector<TNode*> emptyMeshes;
    m_mesh_v.swap(emptyMeshes);

    std::vector<TNode*> emptyanimated;
    m_animatedMeshes_v.swap(emptyanimated);

    std::vector<TNode*> emptyLOD;
    m_LODMeshes.swap(emptyLOD);

    std::vector<TNode*> emptyCamera;
    m_camera_v.swap(emptyCamera);

    std::vector<TNode*> emptyLights;
    m_light_v.swap(emptyLights);

    std::vector<TSprite*> emptySprites;
    m_sprites_v.swap(emptySprites);

    std::vector<TBillBoard*> emptyBillboards;
    m_billBoards_v.swap(emptyBillboards);

    std::vector<TText*> emptyText;
    m_text_v.swap(emptyText);

    //m_mesh_v.clear();
    //m_animatedMeshes_v.clear();
    //m_LODMeshes.clear();
    //m_light_v.clear();
    //m_camera_v.clear();
    //m_text_v.clear();
    m_activeCamera_ptr = nullptr;
    //m_sprites_v.clear();
    //m_billBoards_v.clear();
}

/**
 * Creates GLFW window, initializes GLEW, sets viewport
 * and displays some user's graphics information
 * 
 * @return int
 * 1 -> Initialization failed
 * 0 -> OK
 */
int GenesisEngine::CreateWindow(int l_screenWidth_n, int l_screenHeight_n)
{

    window = glfwCreateWindow(l_screenWidth_n, l_screenHeight_n, "Inspire", NULL, NULL);

    m_spritesProjection_m4 = glm::ortho(0.0f, (float)l_screenWidth_n, (float)l_screenHeight_n, 0.0f, -1.0f, 1.0f);
    m_textProjection_m4 = glm::ortho(0.0f, (float)l_screenWidth_n, 0.0f, (float)l_screenHeight_n);

    if(!window)
    {
        //////std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    /**Set a callback function to update key states. */
    glfwSetKeyCallback(window, On_Event);
    glfwSetMouseButtonCallback(window, On_MouseEvent);
    glfwSetCursorPosCallback(window, GetCursorPosition);
    //Get Buffer information
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    /** To resize the window. */
    glfwSetFramebufferSizeCallback(window, Resize);
    //Current window where glew has to draw(if there's more than a window)
    glfwMakeContextCurrent(window);
    //Extension features
    glewExperimental = GL_TRUE;

    if(glewInit()!=GLEW_OK)
    {
        //std::cout << "GLEW initialisation failed!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_STENCIL_TEST);
    // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glViewport(0, 0, bufferWidth, bufferHeight);

    ////std::cout << "GRAPHICS CARD INFORMATION:" << std::endl;
    //MANUFACTURER OF THE GRAPHICS HARDWARE
    ////std::cout << "Manufacturer: " << (const char*)(glGetString(GL_VENDOR)) << std::endl;
    //MAKE AND MODEL OF THE GRAPHICS CARD
    ////std::cout << "Make and model: " << (const char*)(glGetString(GL_RENDERER)) << std::endl;
    //CURRENT OPENGL VERSION AND MODE SET
    ////std::cout << "OpenGL version and mode set: " << (const char*)(glGetString(GL_VERSION)) << std::endl;
    //WHICH GLSL VERSION IS SUPPORTED BY THE GRAPHICS CARD
    ////std::cout << "GLSL version supported: " << (const char*)(glGetString(GL_SHADING_LANGUAGE_VERSION)) << std::endl;
    return 0;
}

/**
 * @brief: Stops GLFW and close the window.
 * 
 */
void GenesisEngine::Close()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GenesisEngine::CloseWindow()
{
    glfwSetWindowShouldClose(window, true);
}

/**
 * @brief: Updates Graphic Engine:
 *         Node transforms, user inputs, current shader in use and swaps buffers
 * 
 */
void GenesisEngine::Update()
{   
    dt = (float)m_clock_ptr->GetElapsedTime();
    m_clock_ptr->Restart();
    
    m_postProcessingUnit_ptr->Update();

    glClearColor(1.0f, 0.8f, 0.2f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draws the scene.
    Draw(dt);

    m_postProcessingUnit_ptr->PostProcess(dt);

    DrawGUI();
    // Resets the selected Shader.
    glUseProgram(0);
    // Swap to the back buffer.
    glfwSwapBuffers(window);
    // Check events.
    glfwPollEvents();
    // Define the color we use to clear the window. 
    glfwGetWindowSize(window, &bufferWidth, &bufferHeight);
}

/**
 * @brief 
 * 
 */
void GenesisEngine::UpdateAnimations()
{
    TAnimation * animation = reinterpret_cast<TAnimation*>(m_animatedMeshes_v[0]->GetEntity());

    for(size_t i = 0; i < m_animatedMeshes_v.size(); i++)
    {
        animation = reinterpret_cast<TAnimation*>(m_animatedMeshes_v[i]->GetEntity());
        animation->Update();
    }
}

void GenesisEngine::RenderGUIOnly()
{
    dt = (float)m_clock_ptr->GetElapsedTime();
    m_clock_ptr->Restart();
    
    m_postProcessingUnit_ptr->Update();

    glClearColor(1.0f, 0.8f, 0.2f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_postProcessingUnit_ptr->PostProcess(dt);

    DrawGUI();

    glUseProgram(0);
    // Swap to the back buffer.
    glfwSwapBuffers(window);
    // Check events.
    glfwPollEvents();
    // Define the color we use to clear the window. 
    glfwGetWindowSize(window, &bufferWidth, &bufferHeight);
}

/**
 * @brief 
 * 
 */
void GenesisEngine::DrawGUI()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    RenderSprites();
    RenderText();
}



/**
 * Draw scene tree
 * 
 */
void GenesisEngine::Draw(float dt)
{
    DoCameras();
    DoLights();

    RenderBillBoards();

    SetDrawValues();
    sceneRootNode->Draw();
    //RenderLights();
    RenderSkybox();
    RenderParticles(dt);
}

/**
 * @brief 
 * 
 */
void GenesisEngine::SetDrawValues()
{
    auto& shader = *m_shadersMap["base"];
    shader.UseShader();

    uniformModel = shader.GetModelLocation();
    TEntity::SetModelLocation(uniformModel);

    shader.SetInt("lightCount", m_light_v.size());
    shader.SetVec3f("viewPos", m_activeCamera_ptr->GetCurrentPosition());
    //shader.SetMat4f("viewProjection", (TEntity::GetProjection()*TEntity::GetView()));
    TEntity::SetViewProjection();
}

/**
 * @brief: Set lights of scene calculating and setting their positions.
 *         We only calculate this for the active Camera, which it's always
 *         gonna be in the first position.
 * 
 */
void GenesisEngine::DoCameras()
{
    TEntity::SetView(CalculateMatrix(m_activeCamera_ptr, 0));
}

/**
 * @brief: Set lights of scene calculating and setting their positions
 * 
 */
void GenesisEngine::DoLights()
{
    auto& shader = *m_shadersMap["base"];
    shader.UseShader();

    glm::mat4 l_lightModelMatrix;
    TLight* l_light;

    /** Go through all the active lights and calculate its transformation matrix. */
    for(size_t i = 0; i < m_light_v.size(); i++)
    {
        if(m_light_v[i]->GetEntity()->GetActive())
        {
            l_lightModelMatrix = CalculateMatrix(m_light_v[i], 1);
            SetLightModelMatrix(i, l_lightModelMatrix);
            l_light = reinterpret_cast<TLight*>(m_light_v[i]->GetEntity());
            l_light->SetShaderValues(m_light_v[i]->GetCurrentPosition());
        }
    } 
}

/**
 * @brief: Calculate auxiliar matrix (for model view and lights position)
 * 
 * @param l_activeNode_ptr 
 * @param l_operationType_n 
 * @return glm::mat4 
 */
glm::mat4 GenesisEngine::CalculateMatrix(TNode * l_activeNode_ptr, int l_operationType_n)
{
    //Provisional stack to save the transformations    
    std::stack<TNode*> l_aux_stack;
    TNode* l_auxiliarNode_ptr;
    glm::mat4 l_aux_mat4 = glm::mat4(1.0);

    /** This is because after accumulate all the transformations,
     *  l_auxiliarNode_ptr is gonna be pointing to the root, so we don't
     *  change the value of l_activeNode_ptr.
     */
    l_auxiliarNode_ptr = l_activeNode_ptr;
    //Loop through the scene tree from camera to root node and save transformations in the stack
    while(l_auxiliarNode_ptr->getFather()->getFather())
    {
        l_aux_stack.push(l_auxiliarNode_ptr->getFather());
        l_auxiliarNode_ptr = l_auxiliarNode_ptr->getFather();
    }
    //We empty the stack and accumulate the transform operations in an auxiliar matrix
    while(!l_aux_stack.empty()){
        TTransform* l_transEnt = (TTransform*)l_aux_stack.top()->GetEntity();
        l_aux_mat4 = l_aux_mat4 * l_transEnt->GetMatrix();
        l_aux_stack.pop();
    }

    //In the case we're operating with a light we need to set the auxiliar matrix as its position
    if(l_operationType_n == 1)
    {
        /** l_aud_mat4 to glm::vec3 by doing -> l_aux_mat4 * glm::vec4(0,0,0,1) */
        TranslateSceneNode(l_activeNode_ptr, l_aux_mat4 * glm::vec4(0,0,0,1));
    }
    
    return l_aux_mat4;      
}

/**
 * @brief: Construct Translation and Rotation transformation matrices,
 *         given camera's position and target. 
 * 
 * @param c_cameraPosition_v3 
 * @param c_cameraTarget_v3 
 */
void GenesisEngine::UpdateCameraTarget(const glm::vec3& c_cameraPosition_v3, const glm::vec3& c_cameraTarget_v3)
{
    TTransform* l_cameraRotation_ptr = reinterpret_cast<TTransform*>(m_activeCamera_ptr->getFather()->getFather()->GetEntity());
    TTransform* l_cameraTranslation_ptr = reinterpret_cast<TTransform*>(m_activeCamera_ptr->getFather()->getFather()->getFather()->GetEntity());

    glm::mat4 l_translation_m4 = glm::mat4(1.0);
    glm::mat4 l_rotation_m4    = glm::mat4(1.0);

    glm::vec3 l_up_v3 = glm::vec3(0,1,0);
    glm::vec3 l_cameraFront_v3 = glm::normalize(c_cameraTarget_v3 - c_cameraPosition_v3);
    glm::vec3 l_cameraRight_v3 = glm::normalize(glm::cross(l_cameraFront_v3, l_up_v3));
    
    l_up_v3 = glm::cross(l_cameraRight_v3,l_cameraFront_v3); 

    l_rotation_m4[0][0] = l_cameraRight_v3.x;
    l_rotation_m4[1][0] = l_cameraRight_v3.y;
    l_rotation_m4[2][0] = l_cameraRight_v3.z;

    l_rotation_m4[0][1] = l_up_v3.x;
    l_rotation_m4[1][1] = l_up_v3.y;
    l_rotation_m4[2][1] = l_up_v3.z;

    l_rotation_m4[0][2] = -l_cameraFront_v3.x;
    l_rotation_m4[1][2] = -l_cameraFront_v3.y;
    l_rotation_m4[2][2] = -l_cameraFront_v3.z;

    l_translation_m4[3][0] = -glm::dot(l_cameraRight_v3,c_cameraPosition_v3);
    l_translation_m4[3][1] = -glm::dot(l_up_v3,c_cameraPosition_v3);
    l_translation_m4[3][2] =  glm::dot(l_cameraFront_v3,c_cameraPosition_v3);

    l_cameraRotation_ptr->SetMatrix(l_rotation_m4);
    l_cameraTranslation_ptr->SetMatrix(l_translation_m4);
    
    m_activeCamera_ptr->SetCurrentPosition(c_cameraPosition_v3);
}

/**
 * @brief 
 * 
 * @param l_angle 
 */
void GenesisEngine::SetPerspectiveAngle(double l_angle)
{
    TCamera * camera = reinterpret_cast<TCamera*>(m_activeCamera_ptr->GetEntity());
    camera->SetPerspectiveAngle(l_angle);
}


/**
 * Resizes GLFW window
 * 
 * @param window 
 * @param width 
 * @param height 
 */
void GenesisEngine::Resize(GLFWwindow* window, int width, int height)
{
    GeneratePPBuffers(registeredGenesis.front(), width, height);

    glViewport(0,0,width,height);
}

/**
 * Create Scene node (Frist node of the scene tree)
 * 
 */
void GenesisEngine::CreateRootNode()
{
    sceneRootNode = new TNode(NodeType::Root);
}

/**
 *  Add Mesh leaf node to the scene tree
 * 
 * @param id 
 * @param translation
 */
TNode* GenesisEngine::AddMesh(const std::string& l_meshName_str, TNode* c_father_ptr, int c_HasOutline)
{
    /** Creates the Transformation Nodes. */
    TNode* translation = AddTransformNodes(c_father_ptr);
    /** Creates the Mesh Node. */
    TNode* l_meshNode_ptr = new TNode(NodeType::Mesh);
    /** Sets its Entity TMesh. */
    TMesh* mesh = new TMesh((MeshType)c_HasOutline);
    /** Loads the Mesh file. */
    mesh->LoadMesh(l_meshName_str, "metal.jpg");
    
    if(mesh->GetTRMesh()->GetNumVertices() <= 0)
    {   
        delete translation->getFather()->getFather()->getFather();
        delete l_meshNode_ptr;
        return NULL;
    }

    /** Sets this Mesh file to the Entity. */
    l_meshNode_ptr->SetEntity(mesh);
    /** Sets the Mesh Node as its Translation Node. */
    translation->AddChild(l_meshNode_ptr);
    /** Save this Mesh Node.*/
    m_mesh_v.emplace_back(l_meshNode_ptr);
    /** Returns a pointer to this Mesh Node.*/
    return m_mesh_v.back();
}

/**
 * @brief 
 * 
 * @param l_frames 
 * @param c_father_ptr 
 * @return TNode* 
 */
TNode* GenesisEngine::AddAnimatedMesh(TNode* c_father_ptr)
{
    // Creates the Transformation Nodes.
    TNode* translation = AddTransformNodes(c_father_ptr);
    // Creates the Animated Mesh Node.
    TNode * l_animatedMeshNode = new TNode(NodeType::AnimatedMesh);

    TAnimation * animation = new TAnimation(m_activeCamera_ptr->GetPositionRef(), l_animatedMeshNode->GetPositionRef());

    l_animatedMeshNode->SetEntity(animation);

    translation->AddChild(l_animatedMeshNode);

    m_animatedMeshes_v.emplace_back(l_animatedMeshNode);

    return m_animatedMeshes_v.back();
}

/**
 * @brief 
 * 
 * @param l_meshesNames 
 * @param l_textureSource 
 * @return TNode* 
 */
TNode* GenesisEngine::AddLodMesh(TNode * c_father_ptr)
{
    /** Creates the Transformation Nodes. */
    TNode* translation = AddTransformNodes(c_father_ptr);

    TNode * l_lodNode  = new TNode(NodeType::LODMesh);

    TLodMesh * lodMesh = new TLodMesh
    (
        m_LODLevels, 
        m_LODDistances, 
        m_activeCamera_ptr->GetPositionRef(),
        l_lodNode->GetPositionRef()
    );

    /** Sets this Mesh file to the Entity. */
    l_lodNode->SetEntity(lodMesh);
    /** Sets the Mesh Node as its Translation Node. */
    translation->AddChild(l_lodNode);
    /** Save this Mesh Node.*/
    m_LODMeshes.emplace_back(l_lodNode);
    /** Returns a pointer to this Mesh Node.*/
    return m_LODMeshes.back();
}

/**
 * @brief 
 * 
 * @param node 
 * @param l_meshName 
 * @param l_level 
 */
void GenesisEngine::SetLod(TNode * node, const std::string& l_meshName)
{
    TLodMesh * lodMesh = reinterpret_cast<TLodMesh*>(node->GetEntity());
    lodMesh->SetLodMesh(l_meshName);
}

/**
 * @brief 
 * 
 * @param node 
 * @param l_secuenceName 
 * @param l_frames 
 * @param l_loop 
 */
void GenesisEngine::AddAnimatedSecuence(TNode* node, const std::string& l_secuenceName, const std::vector<std::string>& l_frames, bool l_loop)
{
    TAnimation * animation = reinterpret_cast<TAnimation*>(node->GetEntity());

    animation->AddSecuence(l_secuenceName, l_frames, l_loop);
}

/**
 * @brief 
 * 
 * @param node 
 * @param l_secuenceName 
 * @param l_framesMatrix 
 * @param l_loop 
 */
void GenesisEngine::AddAnimatedSecuence(TNode * node, const std::string& l_secuenceName, const std::vector<std::vector<std::string>>& l_framesMatrix, bool l_loop)
{
    TAnimation * animation = reinterpret_cast<TAnimation*>(node->GetEntity());

    animation->AddSecuence(m_LODLevels, m_LODDistances, l_secuenceName, l_framesMatrix, l_loop);
}

/**
 * Add Camera leaf node to the scene tree
 * 
 * @param id 
 * @param translation 
 */
TNode* GenesisEngine::AddCamera()
{
    /** Creates the Transformation Nodes. */
    TNode* translation = AddTransformNodes(nullptr);
    /** Creates the Mesh Node. */
    TNode* l_cameraNode_ptr = new TNode(NodeType::Camera);
    /** Sets its Entity TCamera. */
    TCamera* camera = new TCamera(0.1f, 120000.0f, 0.0f, glm::vec3(0, 0, 0));
    /** Sets this Camera to the Entity. */
    l_cameraNode_ptr->SetEntity(camera);
    /** Sets the Camera Node as its Translation Node. */
    translation->AddChild(l_cameraNode_ptr);
    /** Save this Mesh Node.*/
    m_camera_v.emplace_back(l_cameraNode_ptr);
    /** Returns a pointer to the this Camera Node.*/
    return m_camera_v.back();
}

/**
 * @brief 
 * 
 * @param l_shaderDirectory 
 * @return TResourceShader* 
 */
TResourceShader* GenesisEngine::AddShader(const std::string& l_shaderName, const std::string& l_shaderDirectory)
{
    TResourceShader * shader;

    if(m_resourceManager_ptr)
    {
        shader = (TResourceShader*)(m_resourceManager_ptr->GetShader(l_shaderDirectory));

        if(shader)
        {
            m_shadersMap[l_shaderName] = shader;
            return shader;
        }
        else
        {
            std::cout << "[ERROR]: -" + l_shaderDirectory + "- directory not found.\n";
        }
    }
    else
    {
        std::cout << "[ERROR]: Can't load shader. -Resource Manager- hasn't been initialized.\n";
    }

    return shader;
}

/**
 * @brief 
 * 
 */
void GenesisEngine::AddParticlesEmitter
(
    const std::string& l_meshName, 
    const std::string& l_textureSource, 
    const glm::vec3& l_position,
    uint32_t l_particlesAmount
)
{
    if(m_shadersMap["particles"])
    {
        m_particles.push_back
        (
            new ParticlesEmitter
            (
                m_shadersMap["particles"], 
                l_meshName, 
                l_textureSource, 
                l_position,
                l_particlesAmount
            )
        );
    }
}

/**
 * @brief 
 * 
 */
void GenesisEngine::DeleteParticles()
{
    for(uint32_t i = 0; i < m_particles.size(); i++)
    {
        delete m_particles[i];
        m_particles[i] = nullptr;
    }

    m_particles.clear();
}

/**
 * @brief 
 * 
 * @param dt 
 */
void GenesisEngine::RenderParticles(float dt)
{
    for(uint32_t i = 0; i < m_particles.size(); i++)
    {
        m_particles[i]->Update(dt, 2, glm::vec3(500.0f, 30.0f, 500.0f));
        m_particles[i]->Draw(dt);
    }
}



/**
 * @brief 
 * 
 * @param id 
 * @return TNode* 
 */
TNode* GenesisEngine::AddLight(const glm::vec3& l_color, const glm::vec3& l_attenuation)
{
    /** Creates the Transformation Nodes. */
    TNode* translation = AddTransformNodes(nullptr);
    /** Creates the Light Node. */
    TNode* light_node = new TNode(NodeType::Light);
    /** Sets its Entity TLight. */
    TLight* light = new TLight(l_color, l_attenuation);
    /** Set this camera as an Active Light. */
    light->SetActive(true);
    /** Sets this Light to the Entity. */
    light_node->SetEntity(light);
    /** Sets the Light Node as its Translation Node. */
    translation->AddChild(light_node);
    /** Call the shader to create the uniform variables for the light and set them for it */
    m_shadersMap["base"]->SetLightShaderValues(light, m_light_v.size());
    /** Save this Light Node.*/
    m_light_v.emplace_back(light_node);
    /** Returns a pointer to the this Light Node.*/
    return m_light_v.back();
}

/**
 * @brief 
 * 
 * @param l_modelMatrix_m4 
 * @param l_light_id 
 */
void GenesisEngine::SetLightModelMatrix(unsigned int l_light_id, const glm::mat4& l_modelMatrix_m4)
{
    TLight * light = (TLight*)m_light_v[l_light_id]->GetEntity();
    light->SetModelMatrix(l_modelMatrix_m4);
}

/**
 * @brief
 * 
 */
void GenesisEngine::RenderLights()
{
    auto& shader = *m_shadersMap["lightShader"];
    shader.UseShader();
    
    for(size_t i = 0; i < m_light_v.size(); i++)
    {
        TLight * light = (TLight*)m_light_v[i]->GetEntity();
        light->DrawLight(shader.GetModelLocation(), shader.GetViewLocation(), shader.GetProjectionLocation());
    }
}

/**
 * @brief: Add transformation nodes to the scene tree
 * 
 * @return TNode* 
 */
TNode* GenesisEngine::AddTransformNodes(TNode* c_father_ptr)
{
     /**
     * Transformations order:
     * 1 - Rotation
     * 2 - Scale
     * 3 - Translation
     */
    //Transform nodes previous to Mesh node
    TNode* rotationAroundParent = new TNode(NodeType::Transform);
    TNode* rotation = new TNode(NodeType::Transform);
    TNode* translation = new TNode(NodeType::Transform);
    TNode* scale = new TNode(NodeType::Transform);
    //Transform nodes's entities
    TTransform* rotationAroundParenT = new TTransform(TTransformType::RotationAroundParent);
    TTransform* rotationT = new TTransform(TTransformType::Rotation);
    TTransform* scaleT    = new TTransform(TTransformType::Scale);
    TTransform* translateT = new TTransform(TTransformType::Translation);
    //Set the entities to its transform nodes
    rotationAroundParent->SetEntity(rotationAroundParenT);
    rotation->SetEntity(rotationT);
    scale->SetEntity(scaleT);
    translation->SetEntity(translateT);
    //Create tree adding childs to nodes
    if(c_father_ptr)
        c_father_ptr->AddChild(rotationAroundParent);
    else
        sceneRootNode->AddChild(rotationAroundParent);
    
    rotationAroundParent->AddChild(translation);
    translation->AddChild(rotation);
    rotation->AddChild(scale);

    return scale;
}

/**
 * Translate a node
 * 
 * @param id 
 * @param translation 
 */
void GenesisEngine::TranslateSceneNode(TNode * l_node_ptr, const glm::vec3& l_translation_v3)
{
    if(l_node_ptr)
    {
        TTransform* transform = (TTransform*)l_node_ptr->getFather()->getFather()->getFather()->GetEntity();
        transform->Identity();
        //Use translate method from TTransform
        transform->Translate(l_translation_v3);
        /** Update the current Position of the Node. */
        l_node_ptr->SetCurrentPosition(l_translation_v3);
    }
    
}

/**
 * Scale a node
 * 
 * @param id 
 * @param scale 
 */
void GenesisEngine::ScaleSceneNode(TNode * l_node_ptr, const glm::vec3& l_scale_v3)
{   
    if(l_node_ptr)
    {
        //Get the father TWO times because Scale is the third previous node to Mesh
        //Downcast from TNode* to TTransform*
        TTransform* transform = (TTransform*)l_node_ptr->getFather()->GetEntity();

        transform->Identity();
        //Use scale method from TTransform
        transform->Scale(l_scale_v3);
        /** Update the current Scale of the Node. */
        l_node_ptr->SetCurrentScale(l_scale_v3);
    }
}

/**
 * Rotate a node
 * 
 * @param id 
 * @param rotation 
 */
void GenesisEngine::RotateSceneNode(TNode * l_node_ptr, const glm::vec3& l_rotation_v3)
{
    if(l_node_ptr)
    {
        TTransform* transform = reinterpret_cast<TTransform*>(l_node_ptr->getFather()->getFather()->GetEntity());
        transform->Identity();
        transform->Rotate(glm::vec3(0,1,0), glm::radians(l_rotation_v3.y));
        transform->Rotate(glm::vec3(1,0,0), glm::radians(l_rotation_v3.x));
        transform->Rotate(glm::vec3(0,0,1), glm::radians(l_rotation_v3.z));
        l_node_ptr->SetCurrentRotation(l_rotation_v3);
    }
}

/**
 * @brief 
 * 
 * @param l_cameraId_n 
 */
void GenesisEngine::SetActiveCamera(unsigned int l_cameraId_n)
{
    TNode * l_activeCamera_ptr = m_camera_v[l_cameraId_n];

    if(l_activeCamera_ptr && l_activeCamera_ptr != m_activeCamera_ptr)
    {
        if(m_activeCamera_ptr)
        {
            TCamera* l_activeCameraOldEnt = (TCamera*)m_activeCamera_ptr->GetEntity();
            l_activeCameraOldEnt->SetActive(false);
        }
        
        m_activeCamera_ptr = l_activeCamera_ptr;

        TCamera* l_activeCameraNewEnt = (TCamera*)m_activeCamera_ptr->GetEntity();
        l_activeCameraNewEnt->SetActive(true);
        l_activeCameraNewEnt->SetPerspectiveProjection(45.0f, 4.0f/3.0f);
    }
}

/**
 * @brief 
 * 
 * @param c_billBoardTexture_str 
 * @return TBillBoard* 
 */
TBillBoard* GenesisEngine::CreateBillBoard(const std::string& c_billBoardTexture_str, float l_width_f, float l_height_f)
{
    TBillBoard * l_newBillBoard = new TBillBoard(l_width_f, l_height_f);
    l_newBillBoard->SetTexture(c_billBoardTexture_str);

    m_billBoards_v.push_back(l_newBillBoard);

    return l_newBillBoard;
}

/**
 * @brief 
 * 
 * @param c_affectedBillBoard_ptr 
 * @param c_newPosition_v3 
 */
void GenesisEngine::TranslateBillBoard(TBillBoard* c_affectedBillBoard_ptr, const glm::vec3& c_newPosition_v3)
{
    c_affectedBillBoard_ptr->SetPosition(c_newPosition_v3);
}

/**
 * @brief 
 * 
 * @param c_affectedBillBoard_ptr 
 * @param c_billBoardTexture_str 
 */
void GenesisEngine::ChangeBillBoardTexture(TBillBoard* c_affectedBillBoard_ptr, std::string& c_billBoardTexture_str)
{
    c_affectedBillBoard_ptr->SetTexture(c_billBoardTexture_str);
}

/**
 * @brief 
 * 
 */
void GenesisEngine::RenderBillBoards()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto& shader = *m_shadersMap["billboards"];
    shader.UseShader();
    
    glm::mat4 l_viewMatrix_m4 = TEntity::GetView();
    glm::mat4 l_projectionMatrix_m4 = TEntity::GetProjection();
    glm::mat4 l_viewProjectionMatrix_m4 = l_projectionMatrix_m4 * l_viewMatrix_m4;
    glm::vec3 l_cameraPosition_v3 = m_activeCamera_ptr->GetCurrentPosition();

    shader.SetMat4f("VPMatrix", l_viewProjectionMatrix_m4);
    shader.SetVec3f("cameraPosition", l_cameraPosition_v3);

    for(size_t i = 0; i< m_billBoards_v.size(); i++)
    {
        m_billBoards_v[i]->BeginDraw();
    }
}

/**
 * @brief 
 * 
 */
void GenesisEngine::AddSkybox(const std::vector<std::string>& l_boxImagesPath)
{
    m_skybox_ptr = new TSkybox(l_boxImagesPath);
}

/**
 * @brief 
 * 
 */
void GenesisEngine::RenderSkybox()
{
    glDepthFunc(GL_LEQUAL);  

    TResourceShader& shader = *m_shadersMap["skybox"];
    shader.UseShader();
    
    glm::mat4 view = glm::mat4(glm::mat3(TEntity::GetView()));
    shader.SetMat4f("view", view);
    shader.SetMat4f("projection", TEntity::GetProjection());

    glBindVertexArray(m_skybox_ptr->GetVAO());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox_ptr->GetSkyboxID());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS); 
}

/**
 * @brief 
 * 
 * @param l_node_ptr 
 * @param l_texturePath_s 
 */
void GenesisEngine::SetNodeTexture(TNode* l_node_ptr, const std::string& l_texturePath_s)
{
    if(l_node_ptr)
    {
        if(l_node_ptr->GetType() == NodeType::Mesh)
        {
            TMesh * l_mesh = reinterpret_cast<TMesh*>(l_node_ptr->GetEntity());
            l_mesh->SetTexture(l_texturePath_s);
        }
        else
        {
            TLodMesh * l_mesh = reinterpret_cast<TLodMesh*>(l_node_ptr->GetEntity());
            l_mesh->SetLodTexture(l_texturePath_s);
        }
        
    }
}

/**
 * @brief Set the Node Texture object
 * 
 * @param l_node_ptr 
 * @param l_texturePath_s 
 * @param l_secuence 
 */
void GenesisEngine::SetNodeTexture(TNode* l_node_ptr, const std::string& l_texturePath_s, const std::string& l_secuence)
{
    if(l_node_ptr)
    {
        if(l_node_ptr->GetType() == NodeType::AnimatedMesh)
        {
            TAnimation * animated = reinterpret_cast<TAnimation*>(l_node_ptr->GetEntity());
            animated->SetSecuenceTexture(l_secuence, l_texturePath_s);
        }
    }
}

/**
 * @brief 
 * 
 * @param c_node 
 * @param c_newFrameRate 
 */
void GenesisEngine::SetAnimationFrameRate(TNode* c_node, float c_newFrameRate)
{
    TAnimation * animated = nullptr;
    animated = (TAnimation*)c_node->GetEntity();
    if(animated != nullptr)
        animated->SetFrameRate(c_newFrameRate);
}

/**
 * @brief: loads a texture via Resource Manager.
 * 
 * @param l_sourceName 
 */
void GenesisEngine::LoadTexture(const std::string& l_sourceName)
{
    if(m_resourceManager_ptr)
    {
        m_resourceManager_ptr->LoadTexture(l_sourceName);
    }
    else
    {
        std::cout << "[ERROR]: Can't load texture. -Resource Manager- doesn't been initialized.\n";
    }
}

/**
 * @brief 
 * 
 * @param l_sourceNameS 
 */
void GenesisEngine::LoadTexture(const std::vector<std::string>& l_sourceNameS)
{
    if(m_resourceManager_ptr)
    {
        for(uint32_t i = 0; i < l_sourceNameS.size(); i++)
        {
            m_resourceManager_ptr->LoadTexture(l_sourceNameS[i]);
        }
    }
    else
    {
        std::cout << "[ERROR]: Can't load texture(s). -Resource Manager- doesn't been initialized.\n";
    }
}

/**
 * @brief loads a font via Resource Manager.
 * 
 * @param l_fontPath 
 */
void GenesisEngine::LoadFont(const std::string& l_fontPath)
{
    if(m_resourceManager_ptr)
    {
        m_resourceManager_ptr->LoadFont(l_fontPath);
    }
    else
    {
        std::cout << "[ERROR]: Can't load font. -Resource Manager- hasn't been initialized.\n";
    }
}

/**
 * @brief loads a font via Resource Manager.
 * 
 * @param l_fontPath 
 */
void GenesisEngine::LoadFont(const std::vector<std::string>& l_fontsPaths)
{
    if(m_resourceManager_ptr)
    {
        for(uint32_t i = 0; i < l_fontsPaths.size(); i++)
        {
            m_resourceManager_ptr->LoadFont(l_fontsPaths[i]);
        }
    }
    else
    {
        std::cout << "[ERROR]: Can't load font(s). -Resource Manager- hasn't been initialized.\n";
    }
}

/**
 * @brief Set the Sprite Texture object
 * 
 * @param l_sprite_ptr 
 * @param l_texturePath_s 
 */
void GenesisEngine::SetSpriteTexture(TSprite* l_sprite_ptr, const std::string& l_texturePath_s)
{
    if(l_sprite_ptr)
    {
        l_sprite_ptr->SetTexture(l_texturePath_s);
    }
}

/**
 * @brief 
 * 
 * @param c_newTexture_string 
 * @param c_width 
 * @param c_height 
 * @param c_xPos 
 * @param c_yPos 
 * @return TSprite* 
 */
TSprite* GenesisEngine::CreateSprite(const std::string& c_newTexture_string, int c_width, int c_height, int c_xPos, int c_yPos)
{
    TSprite* l_newSprite = new TSprite(c_newTexture_string, c_width, c_height, c_xPos, c_yPos);
    m_sprites_v.emplace_back(l_newSprite);
    return l_newSprite;
}

/**
 * @brief 
 * 
 * @param c_sprite 
 * @param c_width 
 * @param c_height 
 */
void GenesisEngine::ScaleSprite(TSprite* c_sprite, int c_width, int c_height)
{
    c_sprite->ScaleSprite(c_width, c_height);
}

/**
 * @brief 
 * 
 * @param l_howMany 
 */
void GenesisEngine::EraseSprites(int l_howMany)
{
    for(int i = 0; i < l_howMany; i++)
    {
        int position = m_sprites_v.size()-1;
        delete m_sprites_v[position];
        m_sprites_v[position] = nullptr;
        m_sprites_v.erase(m_sprites_v.begin() + position);
    }
}

/**
 * @brief Set the Sprite Status object
 * 
 * @param c_sprite 
 * @param c_status 
 */
void GenesisEngine::SetSpriteStatus(TSprite* c_sprite, bool c_status)
{
    c_sprite->SetActive(c_status);
}

/**
 * @brief 
 * 
 */
void GenesisEngine::RenderSprites()
{
    auto& shader = *m_shadersMap["sprites"];
    shader.UseShader();

    shader.SetMat4f("ProjectionMat", m_spritesProjection_m4);

    for(auto const& sprite : m_sprites_v)
    {
        sprite->BeginDraw();
    }
}

/**
 * @brief 
 * 
 * @param c_xPos 
 * @param c_yPos 
 * @param c_Scale 
 * @param c_textToRender 
 * @param c_fontLocation 
 * @return TText* 
 */
TText* GenesisEngine::CreateText(float c_xPos, float c_yPos, float c_Scale, const std::string& c_textToRender, const std::string& c_fontLocation)
{
    TText* l_newText = new TText(c_xPos, c_yPos, c_Scale, c_textToRender, c_fontLocation);
    m_text_v.emplace_back(l_newText);
    return l_newText;
}

/**
 * @brief 
 * 
 */
void GenesisEngine::RenderText()
{ 
    auto& shader = *m_shadersMap["text"];

    shader.UseShader();
    shader.SetMat4f("projection", m_textProjection_m4);

    for(auto const& text : m_text_v)
    {
        text->BeginDraw();
    }

}

/**
 * @brief 
 * 
 * @param objectToRegister 
 */
void registerGenesisInstance(GenesisEngine* genesis)
{
    if(registeredGenesis.size() < 1)
    {
        for (GenesisEngine * obj : registeredGenesis)
        {
            if (obj == genesis) 
            {
                return; 
            }
        }
        registeredGenesis.push_back(genesis);
    }
}







