#include "RenderManager.h"
#include "AIManager.hpp"
#include <GUIManager.hpp>
#include <StatesMachine.hpp>
#include "InputManager.hpp"
#include <memory>
#include <iostream>
#include <GenericData.hpp>
#include <GenesisRender.hpp>
#include <CommonData.hpp>

/**
 * @brief Construct a new Render Manager:: Render Manager object
 * 
 */
RenderManager::RenderManager()
{
    
}

/**
 * @brief Destroy the Render Manager:: Render Manager object
 * 
 */
RenderManager::~RenderManager()
{
    ClearList();
    delete render;
    render = nullptr;
}

/**
 * @brief 
 * 
 */
void RenderManager::ClearList()
{
    for(unsigned int i = 0; i < components.size(); i++)
     {
        delete components[i];
        components[i] = nullptr;
     }
    components.clear();
}

/**
 * @brief 
 * 
 */
void RenderManager::CloseWindow(){
    render->CloseWindow();
}

/**
 * @brief 
 * 
 */
void RenderManager::Reset()
{
    ClearList();
    render->CleanScene();
}

/**
 * @brief 
 * 
 * @param engine 
 * @param c_genericData 
 */
void RenderManager::Init(int engine, GenericData *&c_genericData)
{
    m_entityManager_ptr   = c_genericData->g_entityManager_ptr;
    m_inputManager_ptr    = c_genericData->g_inputManager_ptr;
    m_physicsManager_ptr  = c_genericData->g_physicsManager_ptr;
    m_guiManager_ptr      = c_genericData->g_guiManager_ptr;
    m_debugGuiManager_ptr = c_genericData->g_debugGuiManager_ptr;
    m_waypointManager_ptr = c_genericData->g_waypointManager_ptr;
    m_renderUtilities_ptr = RenderUtilities::getInstance();

    if(engine==1){
        render = new IrrlichtRender(680, 420, false, dynamic_cast<InputIrrlicht*>(m_inputManager_ptr->GetFacade()));
    }else{
        render = new GenesisRender(1280, 720);
    }
}

/**
 * @brief 
 * 
 * @param entity 
 * @param type 
 * @param file 
 */
void RenderManager::CreateComponent(Entity* entity, const std::string& type, const std::string& file)
{
    RenderComponent* render = new RenderComponent("Render", type, file, entity->GetId());
    components.push_back(render);
    m_entityManager_ptr->AddComponent(entity->GetId(), (IComponent*) render);
}

/**
 * @brief 
 * 
 * @param entity 
 */
void RenderManager::RenderScene(Entity* entity)
{
    IComponent* rendercomponent =  m_entityManager_ptr->GetComponent(entity->GetId(), "Render");
    //PositionComponent * positionComponent = ((PositionComponent*)m_entityManager_ptr->GetComponent(entity->GetId(), "Position"));

    if(rendercomponent != nullptr)
    {
        render->BeginScene();
        render->Render();
        // if(positionComponent != nullptr)
        //     m_guiManager_ptr->RenderGUI(positionComponent->GetLap());
        // else
        //     m_guiManager_ptr->RenderGUI(-1);
        // if(m_debugMode)
        // {
        //     this->SetDebugAIPath();
        //     this->DrawAIVectors();
        //     this->DrawPlayerVectors();
        //     this->DrawOBBVertex();
        // }
        // StatesMachine::getInstance()->GetActiveState()->DrawStateGUI();
        // m_debugGuiManager_ptr->RenderDebugWindows();
        render->EndScene();
    }
}

/**
 * @brief 
 * 
 */
void RenderManager::DrawGUI()
{
    //render->DrawGUI();
}

/**
 * @brief 
 * 
 * @param entity 
 * @param newPos 
 * @param distZ 
 */
void RenderManager::UpdateCameraTarget(Entity* entity, const glm::vec3& newPos, float distZ)
{
    glm::vec3 player_pos = entity->getPos();
    player_pos.z += distZ;
    render->UpdateCameraTarget(entity->GetId(),player_pos, newPos);
}

/**
 * @brief 
 * 
 * @param l_angle 
 */
void RenderManager::SetPerspectiveAngle(double l_angle)
{
    render->SetPerspectiveAngle(l_angle);
}

/**
 * @brief Get the Effect I D object
 * 
 * @param l_effectName 
 * @return std::string 
 */
std::string RenderManager::GetEffectID()
{
    return render->GetEffectID();
}

/**
 * @brief 
 * 
 * @param entity 
 */
void RenderManager::AddNode(Entity* entity, int c_hasDetail)
{
    IComponent* component =  m_entityManager_ptr->GetComponent(entity->GetId(), "Render");
    
    if(component != nullptr)
    {
        RenderComponent* rendercomponent = ((RenderComponent*) component);

        if(rendercomponent->getType() == "Cube")
        {
            render->AddNode(entity->GetId(),entity->getPos());
        }
        else if(rendercomponent->getType() == "Mesh")
        {
            render->AddMesh(entity->GetId(),entity->getPos(), rendercomponent->getMeshPath(), c_hasDetail);
        }
        else if(rendercomponent->getType() == "LOD")
        {
            render->AddLodMesh(entity->GetId(), rendercomponent->getMeshPath(), entity->getPos());
        }
        else if(rendercomponent->getType() == "Animated")
        {
            render->AddAnimatedMesh(entity->GetId(), entity->getPos());
        }
        else
        {
            render->AddNode(entity->GetId(),entity->getPos());
        }
        rendercomponent->setNode(entity->GetId());
    }

}

/**
 * @brief 
 * 
 * @param c_nodeName 
 * @param l_secuenceName 
 * @param l_frames 
 * @param l_loop 
 */
void RenderManager::AddAnimatedSecuence(Entity* entity, const std::string& l_secuenceName, const std::vector<std::string>& l_frames, bool l_loop)
{
    render->AddAnimatedSecuence(entity->GetId(), l_secuenceName, l_frames, l_loop);
}

/**
 * @brief 
 * 
 * @param c_nodeName 
 * @param l_secuenceName 
 * @param l_frames 
 * @param l_loop 
 */
void RenderManager::AddAnimatedSecuence(Entity* entity, const std::string& l_secuenceName, const std::vector<std::vector<std::string>>& l_frames, bool l_loop)
{
    render->AddAnimatedSecuence(entity->GetId(), l_secuenceName, l_frames, l_loop);
}

/**
 * @brief 
 * 
 * @param c_nodeName 
 * @param l_secuenceName 
 */
void RenderManager::SetSecuence(unsigned int c_nodeName, const std::string& l_secuenceName)
{
    render->SetSecuence(c_nodeName, l_secuenceName);
}

/**
 * @brief 
 * 
 * @param l_effectId 
 */
void RenderManager::ActivatePPEffect(const std::string& l_effectId)
{
    // if(l_effectId == "defaultPP")
    // {
    //     std::cout << "se activa el default\n";
    // }
    render->ActivatePPEffect(l_effectId);
}

void RenderManager::AddParticlesEmitter
(
    const std::string& l_meshName, 
    const std::string& l_textureSource, 
    const glm::vec3& l_position, 
    uint32_t l_particlesAmount
)
{
    render->AddParticlesEmitter
    (
        l_meshName, 
        l_textureSource, 
        l_position, 
        l_particlesAmount
    );
}

/**
 * @brief 
 * 
 * @param pos 
 * @param scale 
 */
void RenderManager::DebugBBox(const glm::vec3& pos, float scale)
{
    int i = m_entityManager_ptr->GetNumEntities() + 10;

    render->AddDebugNode("Octree", i, pos);
    render->ScaleDebugNode(i, scale);
}

/**
 * @brief 
 * 
 * @param id 
 * @param pos 
 * @param rad 
 */
void RenderManager::DebugBSphere(unsigned int id, const glm::vec3& pos, float rad)
{
    render->AddDebugSphere(id, pos, rad);
}

/**
 * @brief 
 * 
 * @param id 
 * @param pos 
 */
void RenderManager::TranslateBSphere(unsigned int id, const glm::vec3& pos)
{
    render->TranslateDebugSceneNode(id, pos);
}

/**
 * @brief 
 * 
 * @param entity 
 * @param c_texturePath_s 
 */
void RenderManager::AddTexture(Entity* entity, const std::string& c_texturePath_s)
{
    IComponent* component =  m_entityManager_ptr->GetComponent(entity->GetId(), "Render");

    if(component != nullptr)
    {
        RenderComponent* rendercomponent = ((RenderComponent*) component);
        render->AddTexture(rendercomponent->getNode(), c_texturePath_s);
    }
}

/**
 * @brief 
 * 
 * @param entity 
 * @param c_texturePath_s 
 * @param l_secuence 
 */
void RenderManager::AddTexture(Entity* entity, const std::string& c_texturePath_s, const std::string& l_secuence)
{
    IComponent* component =  m_entityManager_ptr->GetComponent(entity->GetId(), "Render");

    if(component != nullptr)
    {
        RenderComponent* rendercomponent = ((RenderComponent*) component);
        render->AddTexture(rendercomponent->getNode(), c_texturePath_s, l_secuence);
    }
}

/**
 * @brief 
 * 
 * @param entity 
 * @param position 
 */
void RenderManager::RotateSceneNode(Entity* entity, const glm::vec3& position)
{

    render->RotateSceneNode(entity->GetId(), position);
}

/**
 * @brief 
 * 
 * @param entity 
 * @param scaleFactor 
 */
void RenderManager::ScaleSceneNode(Entity* entity, float scaleFactor)
{
    render->ScaleSceneNode(entity->GetId(), scaleFactor);
}

/**
 * @brief 
 * 
 */
void RenderManager::SetDebugAIPath()
{
    
    const std::vector<WaypointComponent*> waypoints = m_waypointManager_ptr->GetWaypointsVector();
    for(size_t i = 0; i < waypoints.size()-1 ; i++)
    {
        render->DebugDraw2DLine(waypoints[i]->GetPosition(), waypoints[i+1]->GetPosition(), 2);

        render->DebugDraw2DLine(waypoints[i]->GetPosition() + glm::vec3(40,0,0), waypoints[i+1]->GetPosition() + glm::vec3(40,0,0), 1);
        render->DebugDraw2DLine(waypoints[i]->GetPosition() - glm::vec3(40,0,0), waypoints[i+1]->GetPosition() - glm::vec3(40,0,0), 1);

        render->DebugDraw2DLine(waypoints[i]->GetPosition() + glm::vec3(0,40,0), waypoints[i+1]->GetPosition() + glm::vec3(0,40,0), 1);
        render->DebugDraw2DLine(waypoints[i]->GetPosition() - glm::vec3(0,40,0), waypoints[i+1]->GetPosition() - glm::vec3(0,40,0), 1);
    }

    render->DebugDraw2DLine(waypoints[waypoints.size()-1]->GetPosition(), waypoints[0]->GetPosition(), 2); 
    render->DebugDraw2DLine(waypoints[waypoints.size()-1]->GetPosition() + glm::vec3(40,0,0), waypoints[0]->GetPosition() + glm::vec3(40,0,0), 1);
    render->DebugDraw2DLine(waypoints[waypoints.size()-1]->GetPosition() - glm::vec3(40,0,0), waypoints[0]->GetPosition() - glm::vec3(40,0,0), 1);

    render->DebugDraw2DLine(waypoints[waypoints.size()-1]->GetPosition() + glm::vec3(0,40,0), waypoints[0]->GetPosition() + glm::vec3(0,40,0), 1);
    render->DebugDraw2DLine(waypoints[waypoints.size()-1]->GetPosition() - glm::vec3(0,40,0), waypoints[0]->GetPosition() - glm::vec3(0,40,0), 1);
}

/**
 * @brief 
 * 
 */
void RenderManager::DrawAIVectors()
{  
 
 
}

/**
 * @brief 
 * 
 */
void RenderManager::DrawPlayerVectors()
{
    const std::vector<TransformComponent*> PhysicsComponents = m_physicsManager_ptr->GetPhysicsComponentList();

    for(size_t i = 0; i<PhysicsComponents.size(); i++)
    {

        render->DebugDraw2DLine( PhysicsComponents[i]->GetPhysicsData()->currentPosition, (PhysicsComponents[i]->GetPhysicsData()->v_acceleration * 500.0f + PhysicsComponents[i]->GetPhysicsData()->currentPosition ), 1);
        render->DebugDraw2DLine( PhysicsComponents[i]->GetPhysicsData()->currentPosition, (PhysicsComponents[i]->GetPhysicsData()->v_velocity * 50.0f + PhysicsComponents[i]->GetPhysicsData()->currentPosition ), 2);
    }
}

/**
 * @brief 
 * 
 */
void RenderManager::DrawOBBVertex()
{
    const std::vector<RigidBodyComponent*> RBCom = m_physicsManager_ptr->GetRBComponentList();

    for(size_t i = 0; i<RBCom.size(); i++)
    {
        if(RBCom[i]->getBounding() == "Rectangle")
        {
            for(unsigned int j=0; j<8; j++)
            {
                render->DebugDraw2DPoint(RBCom[i]->getRectangleVertex(j),1);
            }
            for(unsigned int j=0; j<6; j++)
            {
                render->DebugDraw2DLine(RBCom[i]->getRectanglePlanePoint(j), (RBCom[i]->getRectanglePlaneNormal(j)*100.0f) + RBCom[i]->getRectanglePlanePoint(j), 1);
            }
        }
    }
}

/**
 * @brief 
 * 
 * @param entity 
 */
void RenderManager::DrawBBOX(Entity* entity)
{
    render->DrawBBOX(entity->GetId());
}

/**
 * @brief 
 * 
 * @param c_name_s 
 * @param c_initialPosition_v3dirr 
 * @param c_initialRotation_v3dirr 
 * @param c_initialScale_v3dirr 
 */
void RenderManager::AddDebugCube(unsigned int c_name_s, const glm::vec3& c_initialPosition_v3dirr, const glm::vec3& c_initialRotation_v3dirr, const glm::vec3& c_initialScale_v3dirr)
{
    render->AddDebugCube(c_name_s, c_initialPosition_v3dirr, c_initialRotation_v3dirr, c_initialScale_v3dirr);
}

/**
 * @brief 
 * 
 */
void RenderManager::SetDebugMode()
{
    m_debugMode = !m_debugMode;
}

/**
 * @brief 
 * 
 * @param c_entity_ptr 
 * @param c_initialPosition_vec3 
 */
void RenderManager::AddCamera(Entity* c_entity_ptr, const glm::vec3& c_initialPosition_vec3)
{
    render->AddCamera(c_entity_ptr->GetId(), c_initialPosition_vec3, glm::vec3(0,0,0));
}

// void UpdateRotate(Event r){
//     RenderManager::getInstance()->GetRenderFacade()->TranslateSceneNode(node->Nodeid,node->Pos);
// }

// void UpdateTranslate(Event r){
//     RenderManager::getInstance()->GetRenderFacade()->RotateSceneNode(node->Nodeid, node->Rot);
// }


/**
 * @brief 
 * 
 * @return IRenderFacade* 
 */
IRenderFacade* RenderManager::GetRenderFacade()
{
    return render;
}

