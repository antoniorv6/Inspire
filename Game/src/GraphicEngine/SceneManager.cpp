#include <SceneManager.h>
#include <iostream>
#include <math.h>

/**
 * @brief Construct a new Scene Manager:: Scene Manager object
 * 
 */
SceneManager::SceneManager()
{

}

/**
 * @brief 
 *  INITPOINTERVALUE
 *      It copies the memory address of the scene manager pointer that we have created in the 
 *      irrlicht device (See IrrlichtRender.cpp constructor method)
 * 
 * @param c_sceneManager -> Scene Manager object passed by reference
 */
const void SceneManager::initPointerValue(scene::ISceneManager *&c_sceneManager)
{
    m_sceneManager_p = c_sceneManager;
}

/**
 * @brief 
 *  ADDNODEOBJECT
 *  It stores a new node object in our scene tree to be rendered
 *  NOTE: This function should grow in order to get the node mesh and the node texture to operate completely with node
 *  objects
 * 
 * @param c_name_s -> Name identifier of the node that we are going to store
 * @param c_initialPosition_v3dirr -> 3D Vector that indicates the initial position of the object in our scene
 * @return true -> If everything has gone alright
 * @return false -> If there has been any error during the proccess
 */
bool SceneManager::AddNodeObject(unsigned int c_name_s, const core::vector3df& c_initialPosition_v3dirr)
{
    m_SceneNodes_m[c_name_s] = m_sceneManager_p->addCubeSceneNode();
    if(m_SceneNodes_m[c_name_s])
    {
        //if(c_name_s == ("Octree")){
           m_SceneNodes_m[c_name_s]->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(m_SceneNodes_m[c_name_s]->isDebugDataVisible()^scene::EDS_HALF_TRANSPARENCY));
           m_SceneNodes_m[c_name_s]->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(m_SceneNodes_m[c_name_s]->isDebugDataVisible()^scene::EDS_BBOX));
        //}
        m_SceneNodes_m[c_name_s]->setPosition(c_initialPosition_v3dirr);
        return true;
    }
    return false;
}

/**
 * @brief 
 * 
 * @param c_name_s 
 * @param id 
 * @param c_initialPosition_v3dirr 
 * @return true 
 * @return false 
 */
bool SceneManager::AddDebugObject(std::string c_name_s, unsigned int id, const core::vector3df& c_initialPosition_v3dirr)
{
    // m_DebugNodes_m[id] = m_sceneManager_p->addCubeSceneNode();
    // if(m_DebugNodes_m[id])
    // {
    //     if(c_name_s == ("Octree")){
    //         m_DebugNodes_m[id]->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(m_DebugNodes_m[id]->isDebugDataVisible()^scene::EDS_HALF_TRANSPARENCY));
    //         m_DebugNodes_m[id]->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(m_DebugNodes_m[id]->isDebugDataVisible()^scene::EDS_BBOX));
    //     }
    //     m_DebugNodes_m[id]->setPosition(c_initialPosition_v3dirr);
    //     return true;
    // }
    // return false;
    
    //[WARNING] NO RETURN
    return false;
}

/**
 * @brief 
 * 
 * @param c_name_s 
 * @param c_initialPosition_v3dirr 
 * @param c_initialRotation_v3dirr 
 * @param c_initialScale_v3dirr 
 */
void SceneManager::AddDebugCube(unsigned int c_name_s, const core::vector3df& c_initialPosition_v3dirr, const core::vector3df& c_initialRotation_v3dirr, const core::vector3df& c_initialScale_v3dirr)
{
    // m_DebugNodes_m[c_name_s] = m_sceneManager_p->addCubeSceneNode(1.0f,0,-1, c_initialPosition_v3dirr, c_initialRotation_v3dirr, c_initialScale_v3dirr);

    // if(m_DebugNodes_m[c_name_s]){

    //     m_DebugNodes_m[c_name_s]->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(m_DebugNodes_m[c_name_s]->isDebugDataVisible()^scene::EDS_HALF_TRANSPARENCY));
    //     m_DebugNodes_m[c_name_s]->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(m_DebugNodes_m[c_name_s]->isDebugDataVisible()^scene::EDS_BBOX));
    // }
}

/**
 * @brief 
 * 
 * @param c_nodeName_s 
 * @param scaleFactor 
 */
void SceneManager::ScaleDebugNode(unsigned int c_nodeName_s, float scaleFactor)
{
    // core::vector3df scaleVector = core::vector3df(scaleFactor,scaleFactor,scaleFactor);
    // m_DebugNodes_m[c_nodeName_s]->setScale(scaleVector);
}

/**
 * @brief 
 * 
 * @param id 
 * @param pos 
 * @param rad 
 */
void SceneManager::AddDebugSphere(unsigned int id, const core::vector3df& pos, float rad)
{
    // m_DebugNodes_m[id] = m_sceneManager_p->addSphereSceneNode(rad, 16, 0, id, pos);
    // m_DebugNodes_m[id]->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(m_DebugNodes_m[id]->isDebugDataVisible()^scene::EDS_HALF_TRANSPARENCY));
}

/**
 * @brief 
 * 
 * @param id 
 * @param pos 
 */
void SceneManager::TranslateDebugSceneNode(unsigned int id, const core::vector3df& pos)
{
    // m_DebugNodes_m[id]->setPosition(pos);
}

/**
 * @brief 
 * 
 * @param c_name_s 
 */
void SceneManager::DrawBBOX(unsigned int c_name_s)
{
    // m_SceneNodes_m[c_name_s]->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(m_SceneNodes_m[c_name_s]->isDebugDataVisible()^scene::EDS_BBOX));
    // m_SceneNodes_m[c_name_s]->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(m_SceneNodes_m[c_name_s]->isDebugDataVisible()^scene::EDS_HALF_TRANSPARENCY));
}

/**
 * @brief 
 * 
 * @param c_name_s 
 * @param c_initialPosition_v3dirr 
 * @param route 
 * @return true 
 * @return false 
 */
bool SceneManager::AddMeshObject(unsigned int c_name_s, const core::vector3df& c_initialPosition_v3dirr, const std::string& route)
{
    std::string l_loadingRoute = "assets/" + route + ".obj";
    io::IReadFile* file = io::createReadFile(l_loadingRoute.c_str());

    IAnimatedMesh* mesh = m_sceneManager_p->getMesh(file);
    if (!mesh)
    {
        std::cout << "CANNOT READ MESH" << std::endl;
    }

    m_SceneNodes_m[c_name_s] = m_sceneManager_p->addAnimatedMeshSceneNode(mesh);
    if(m_SceneNodes_m[c_name_s])
    {
        m_SceneNodes_m[c_name_s]->setPosition(c_initialPosition_v3dirr);
        std::cout << "NODO METIDO" << std::endl;
    }else{
        //std::cout << "NO SE HA PODIDO METER EL NODO" << std::endl;
    }

    delete file;
    file = nullptr;
    return true;
}

/**
 * @brief 
 * 
 * @param c_nodename_s 
 * @param NodePosition 
 * @param newPos 
 */
void SceneManager::UpdateCameraTarget(unsigned int c_nodename_s, const core::vector3df& NodePosition,  const core::vector3df& newPos)
{    
    m_CameraNodes_m[c_nodename_s]->setTarget(NodePosition);
    m_CameraNodes_m[c_nodename_s]->setPosition(newPos);
}

/**
 * @brief 
 *  ADDCAMERA
 *  It stores a new camera in our scene tree. I recommend that there has to be only one camera and we change it's position
 *  for now.
 * @param c_name_s -> Identifier of our camera in the scene tree 
 * @param c_cameraPosition_vec3 -> 3D vector that indicates the initial position of the camera
 * @param c_cameraLookAt_vec3 -> 3D vector that indicates where the camera is looking at (the focus point of it)
 * @return true -> If everything has gone alright
 * @return false -> If there has been any error during proccess
 */
bool SceneManager::AddCamera(unsigned int c_name_s, const core::vector3df& c_cameraPosition_vec3, const core::vector3df& c_cameraLookAt_vec3)
{
     m_CameraNodes_m[c_name_s] = m_sceneManager_p->addCameraSceneNode(0, c_cameraPosition_vec3, c_cameraLookAt_vec3);
     m_CameraNodes_m[c_name_s]->setUpVector(core::vector3df(0,0,1));
     m_CameraNodes_m[c_name_s]->setFarValue(8400.0f);
     if(m_CameraNodes_m[c_name_s])
        return true;
        
     return false;
}

/**
 * @brief 
 *  MOVENODE
 *  Changes a certain node position
 * @param c_nodeName_s -> Identifier of the node that we want to retrieve
 * @param c_newNodePosition_vec3 -> 3D Vector that indicates the new node position (Later, physics engine would send this parameter)
 */
void SceneManager::MoveNode(unsigned int c_nodeName_s, const core::vector3df& c_newNodePosition_vec3)
{
    m_SceneNodes_m[c_nodeName_s]->setPosition(c_newNodePosition_vec3);
}

/**
 * @brief 
 * 
 * @param c_nodeName_s 
 * @param c_newNodeRotation_vec3 
 */
void SceneManager::RotateNode(unsigned int c_nodeName_s, const core::vector3df& c_newNodeRotation_vec3)
{
    m_SceneNodes_m[c_nodeName_s]->setRotation(c_newNodeRotation_vec3);
}

/**
 * @brief 
 * 
 * @param c_nodeName_s 
 * @param scaleFactor 
 */
void SceneManager::ScaleNode(unsigned int c_nodeName_s, float scaleFactor)
{
    core::vector3df scaleVector = core::vector3df(scaleFactor,scaleFactor,scaleFactor);
    
    if(m_SceneNodes_m[c_nodeName_s] == nullptr){
        //std::cout << "SOY NULL" << std::endl;
    }
    m_SceneNodes_m[c_nodeName_s]->setScale(scaleVector);
}

/**
 * @brief 
 *  RENDEROBJECTS
 *  It "draws" all the objects in the tree scene node
 *  Note: It doesn't draw phisically the objects yet, it readies the buffer to do it later. 
 */
void SceneManager::RenderObjects()
{
    m_sceneManager_p->drawAll();
}

/**
 * @brief 
 * 
 * @param c_nodeName_s 
 * @param c_texture_ptr 
 * @return true 
 * @return false 
 */
const bool SceneManager::AddTexture(unsigned int c_nodeName_s, video::ITexture*& c_texture_ptr)
{
    m_SceneNodes_m[c_nodeName_s]->setMaterialTexture(0, c_texture_ptr);
    m_SceneNodes_m[c_nodeName_s]->setMaterialFlag(video::EMF_LIGHTING, false);
    
    return true;
}

/**
 * @brief 
 * 
 */
void SceneManager::CleanSceneTrees()
{
    for(auto object : m_SceneNodes_m)
    {
        object.second->remove();
    }

    for(auto object : m_DebugNodes_m)
    {
        object.second->remove();
    }

    for(auto object : m_CameraNodes_m)
    {
        object.second->remove();
    }

    m_SceneNodes_m.clear();
    m_DebugNodes_m.clear();
    m_CameraNodes_m.clear();

    std::cout<<"[SUCCESS] - Irrlicht scene nodes erased correctly"<<std::endl;
}

