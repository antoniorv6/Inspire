#pragma once

#include <glm.hpp>
#include <vector>

class RenderManager;
class EntityManager;
class Entity;
class GenericData;
class TransformComponent;
class CameraManager
{
    public:
    
    /*=======CAMERA MOVEMENT========*/
    struct CameraData
    {
        glm::vec3 position = {0,0,0};

        float distance;
        float rotation;
        float height;

        float max_rotation;
        float inc_rotation;
        float min_distance;
        float inc_distance;
        float value_height;

        float max_velocity;

        /*INTERPOLATION VARIABLES ==>*/ 
        float prev_rotation, now_rotation,
              prev_distance, now_distance,
              prev_height,   now_height;
    };
    /*==============================*/
  
        CameraManager();
        ~CameraManager();
        void Init(GenericData *&c_genericData);
        void InitCameraData(int active_camera);
        void ClearList();
        void CopyCameraData(CameraData* origin, CameraData* target);
        void CopyCameraInterpolationData(CameraData* origin, CameraData* target);
        void SetTarget(Entity* c_entity_ptr);
        void SetPerspectiveAngle(float l_angle);
        void SetActiveCamera(int i);
        glm::vec3 GetActiveCameraPosition();
        bool CheckActiveCamera(CameraData* l_cam_ptr);

        //=======Calculate Datas========//
        void CalculateCamerasDatas(float RotSpin, glm::vec3 velocity, glm::vec3 player_pos, float delta);
        void CalculateStandardCameraData(float RotSpin, glm::vec3 velocity, glm::vec3 player_pos, float delta);
        void CalculateRearCameraData();
        void CalculateMenuCameraData();
        //=================================//

        //=========Update Positions========//
        void UpdateCameraPosition(float delta);
        void UpdateCameraPosition(float rotSPIN, const glm::vec3& v_velocity, float delta);
        //=================================//

        //=======Interpolate Cameras========//
        void InterpolateCameras(float& delta, const float loopTime);
        void InterpolateStandardCamera(float& delta, const float loopTime);
        void InterpolateRearCamera();
        //==================================//
    

    private:

        CameraData SCam;     //Standard Camera
        CameraData RCam;     //Rear Camera
        CameraData MenuCam;  //Selector Menu Camera

        CameraData* Cam = nullptr; //Active Camera

        //Target:
        Entity* m_target = nullptr;
        TransformComponent* m_target_component = nullptr;

        EntityManager* m_entityManager_ptr = nullptr;
        RenderManager* m_renderManager_ptr = nullptr;

};