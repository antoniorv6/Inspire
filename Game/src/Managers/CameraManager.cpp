#include <CameraManager.hpp>
#include <Entity.h>
#include <EntityManager.h>
#include <RenderManager.h>
#include <GenericData.hpp>

CameraManager::CameraManager()
{

}

void CameraManager::Init(GenericData *&c_genericData)
{
    m_entityManager_ptr = c_genericData->g_entityManager_ptr;
    m_renderManager_ptr = c_genericData->g_renderManager_ptr;

    SetActiveCamera(1);
}

void CameraManager::InitCameraData(int active_camera)
{
    SetActiveCamera(active_camera);

    if(CheckActiveCamera(&SCam) || CheckActiveCamera(&RCam))
    {
        SCam.max_rotation = 20.0f;
        //SCam.inc_rotation;
        SCam.min_distance = 5.5f;
        SCam.inc_distance = 6.0f;

        SCam.max_velocity = 70.0f;

        SCam.distance = (SCam.min_distance + SCam.inc_distance);
        SCam.rotation = 180;
        SCam.height = 2.0f;
    }
    else if(CheckActiveCamera(&MenuCam))
    {
        MenuCam.now_distance = 10.0f;
        MenuCam.now_rotation = 0.0f;
        MenuCam.now_height = 0.0f;
    }
}

CameraManager::~CameraManager()
{
    Cam = nullptr;
    ClearList();
}

void CameraManager::ClearList()
{
    m_target = nullptr;
    m_target_component = nullptr;
}

void CameraManager::SetTarget(Entity* c_entity_ptr)
{
    m_target_component = nullptr;
    m_target = c_entity_ptr;
    m_renderManager_ptr->AddCamera(c_entity_ptr, glm::vec3{0,0,0});
}

/**
 * @brief 
 * 
 * @param l_angle 
 */
void CameraManager::SetPerspectiveAngle(float l_angle)
{
    m_renderManager_ptr->SetPerspectiveAngle(l_angle);
}

glm::vec3 CameraManager::GetActiveCameraPosition()
{
    return Cam->position;
}

void CameraManager::SetActiveCamera(int i)
{
    switch(i)
    {
        case 1:
            Cam = &SCam; //Standard Camera
            break;
        case 2:
            Cam = &RCam; //Rear Camera
            break;
        case 3:
            Cam = &MenuCam; //Menu Camera
            break;
    }
}

bool CameraManager::CheckActiveCamera(CameraData* l_cam_ptr)
{
    return (Cam == l_cam_ptr);
}

void CameraManager::CopyCameraData(CameraManager::CameraData* origin, CameraManager::CameraData* target)
{
    target->distance = origin->distance;
    target->rotation = origin->rotation;
    target->height   = origin->height;

    target->max_rotation = origin->max_rotation;
    target->inc_rotation = origin->inc_rotation;
    target->min_distance = origin->min_distance;
    target->inc_distance = origin->inc_distance;
    target->value_height = origin->value_height;
}

void CameraManager::CopyCameraInterpolationData(CameraManager::CameraData* origin, CameraManager::CameraData* target)
{
    target->prev_rotation   = origin->prev_rotation;
    target->now_rotation    = origin->now_rotation;
    target->prev_distance   = origin->prev_distance;
    target->now_distance    = origin->now_distance;
    target->prev_height     = origin->prev_height;
    target->now_height      = origin->now_height;
}

//==========================================================================
//==========================================================================
//                  CALCULATE CAMERAS DATA
//==========================================================================
//==========================================================================

//===========================MAIN FUNCTION===========================

void CameraManager::CalculateCamerasDatas(float RotSpin, glm::vec3 velocity, glm::vec3 player_pos, float delta)
{
    CalculateStandardCameraData(RotSpin, velocity, player_pos, delta);
    CalculateRearCameraData();
}
//===========================STANDARD CAMERA===========================

void CameraManager::CalculateStandardCameraData(float RotSpin, glm::vec3 velocity, glm::vec3 player_pos, float delta)
{
    
    SCam.value_height = 1.5f;

    //New Camera Data is now the old Camera Data and we have to calculate the New Camera Data
    SCam.prev_distance  = SCam.distance;
    SCam.now_distance   = SCam.distance;
    SCam.prev_height    = SCam.height;
    SCam.now_height     = SCam.height;
    SCam.prev_rotation  = SCam.rotation;
    SCam.now_rotation   = SCam.rotation;

    /*========CAMERA SMOOTH SPIN MOVEMENT========*/
    if(RotSpin != SCam.rotation){
        float dif = RotSpin - SCam.rotation;
        float absdif = glm::abs(dif);
        
        if(absdif >100){
            if(glm::abs(RotSpin) > 300)
                RotSpin-= copysign(360,SCam.rotation);
            else if(glm::abs(SCam.rotation) > 300)
                SCam.rotation-= copysign(360,SCam.rotation);
        
        dif = RotSpin - SCam.rotation;
        absdif = glm::abs(dif);
        }
        
        SCam.inc_rotation = absdif/SCam.max_rotation;
        SCam.rotation += copysign(SCam.inc_rotation, dif);
        
        if(absdif < 0.1)
            SCam.rotation = RotSpin;
    }
    /*==========================================*/

    /*===========CAMERA HEIGHT==============*/
    SCam.value_height += velocity.z * (SCam.distance/SCam.min_distance) * 0.6f / (SCam.max_velocity/2); //value
    float diffZ = SCam.value_height-SCam.height;
    
    if(glm::abs(diffZ) > 0.1f)
        SCam.height += copysign(0.1,diffZ); //if there's collision
    else
        SCam.height = SCam.value_height; //if no there's collision
    /*==========================================*/

    /*========CAMERA DISTANCE TO PLAYER==========*/
    velocity.z=0;

    SCam.distance = SCam.min_distance;
    SCam.distance += glm::length(velocity) * SCam.inc_distance / SCam.max_velocity;

    if(glm::abs(SCam.distance - SCam.prev_distance) > 0.25)
    {
        SCam.distance = SCam.prev_distance + copysign(0.25, SCam.distance - SCam.prev_distance); // if there's collision
    }
    /*==========================================*/
}

//===========================REAR CAMERA===========================

void CameraManager::CalculateRearCameraData()
{
    CopyCameraData(&SCam, &RCam);
}

//===========================MENU CAMERA===========================

void CameraManager::CalculateMenuCameraData()
{
    glm::vec3 player_pos = m_target->getPos();
    
    MenuCam.position = {player_pos.x - MenuCam.now_distance*sin(MenuCam.now_rotation*M_PI/180.0f),
                        player_pos.y + MenuCam.now_distance*cos(MenuCam.now_rotation*M_PI/180.0f),
                        player_pos.z + MenuCam.now_distance/4};

    m_renderManager_ptr->UpdateCameraTarget(m_target, Cam->position, Cam->now_height);
}

//==========================================================================
//==========================================================================
//                  UPDATE CAMERAS POSITIONS
//==========================================================================
//==========================================================================

//===========================SINGLE PLAYER FUNCTION===========================

void CameraManager::UpdateCameraPosition(float delta)
{   
    if(!CheckActiveCamera(&MenuCam))
    {
        if(m_target_component == nullptr)
            m_target_component = (TransformComponent*) m_entityManager_ptr->GetComponent(m_target->GetId(), "Physics");

        PICASSO::PhysicsVariables* data = m_target_component->GetPhysicsData();

        delta /= (1.0f/30.0f);

        CalculateCamerasDatas(data->RotSPIN, data->v_velocity, m_target->getPos(), delta);
    }
}

//===========================ONLINE FUNCTION===========================

void CameraManager::UpdateCameraPosition(float rotSPIN, const glm::vec3& v_velocity, float delta)
{
    if(!CheckActiveCamera(&MenuCam))
    {
        delta /= (1.0f/30.0f);

        CalculateCamerasDatas(rotSPIN, v_velocity, m_target->getPos(), delta);
    }
}

//==========================================================================
//==========================================================================
//                    INTERPOLATE CAMERAS 
//==========================================================================
//==========================================================================

//===========================MAIN FUNCTION===========================
void CameraManager::InterpolateCameras(float &delta, const float loopTime)
{
    InterpolateStandardCamera(delta, loopTime);
    InterpolateRearCamera();

    m_renderManager_ptr->UpdateCameraTarget(m_target, Cam->position, Cam->now_height);
}

//===========================STANDARD CAMERA===========================
void CameraManager::InterpolateStandardCamera(float &delta, const float loopTime)
{
    SCam.now_distance = SCam.prev_distance + (delta * (SCam.distance - SCam.prev_distance)) / loopTime;
    SCam.now_height = SCam.prev_height + (delta * (SCam.height - SCam.prev_height)) / loopTime;
        
        float dif = SCam.rotation - SCam.prev_rotation;

        if(glm::abs(dif) >100)
        {
            if(glm::abs(SCam.rotation) > 300)
                SCam.rotation-= copysign(360,SCam.prev_rotation);
            else if(glm::abs(SCam.prev_rotation) > 300)
                SCam.prev_rotation-= copysign(360,SCam.prev_rotation);
                
            dif = SCam.rotation - SCam.prev_rotation;
        }
        

    SCam.now_rotation = SCam.prev_rotation + (delta * (dif)) / loopTime;
    glm::vec3 player_pos = m_target->getPos();

    SCam.position = {player_pos.x + SCam.now_distance*sin(SCam.now_rotation*M_PI/180.0f),
                     player_pos.y - SCam.now_distance*cos(SCam.now_rotation*M_PI/180.0f),
                     player_pos.z + SCam.now_distance/3};
}

//===========================REAR CAMERA===========================
void CameraManager::InterpolateRearCamera()
{
    CopyCameraInterpolationData(&SCam, &RCam);
    RCam.now_distance = RCam.min_distance + RCam.inc_distance/2;
    glm::vec3 player_pos = m_target->getPos();

    RCam.position = {player_pos.x - RCam.now_distance*sin(RCam.now_rotation*M_PI/180.0f),
                     player_pos.y + RCam.now_distance*cos(RCam.now_rotation*M_PI/180.0f),
                     player_pos.z + RCam.now_distance/3};
}



