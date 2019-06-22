#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include <gtx/vector_angle.hpp>
#include <Clock.hpp>
#include <iostream>
#include <math.h>
#include <vector>

namespace PICASSO{

/*
    #####  #  #####  #####  #####  #####  #####
    #   #  #  #      #   #  #      #      #   #
    #####  #  #      #####  #####  #####  #   #
    #      #  #      #   #      #      #  #   #
    #      #  #####  #   #  #####  #####  #####

    Inspire's Physics Engine by Zenon Games
*/

//=============================496.845314612=======
//============VARIABLES===============
//====================================

    typedef glm::vec3 vec3f;

    struct Plane{
    glm::vec3 normal;
    glm::vec3 point;
    float d;
    };

    struct PhysicsVariables{
        //=======Keys=============
        //Forward and behind
        float KeyW=false; 
        float KeyS=false; 

        //Spin left and right
        float KeyA=false;
        float KeyD=false;
    
        //Up and down
        float KeyI=false;
        float KeyK=false;
        //========================
    
        //=====Axis Values========
        float AxisY= 0.0f;
        float AxisZ= 0.0f;
        float AxisSpin= 0.0f;

        float incAxis= 0.06f;
        float decAxis= 0.06f;
    
        //========================
    
        //======Rotation==========
        float   RotSPIN= 180.0f;
        float   RotFWRD= 0.0f;
        float   RotSIDE= 0.0f;
    
        float   incRotSPIN = 3;
    
        float MaxRotSPIN = 360;
        float MaxRotFWRD = 28; 
        float MaxRotSIDE = 25; 
        //========================
    
        //======Acceleration======
        float MaxAcc= 3.0f;
    
        float accUP= 0.0f;
        float accFWRD= 0.0f;
        //========================
    
        //=======Velocity==========
        float MaxVelocity= 70;
        const float MaxVelocityStandar = 70;
        float AirFriction = 0.99f; //Velocity is reduced 0.01%  
        //=========================

        //===========Nitro==========
        float MaxAccNITRO = 4.0f;
        const float MaxVelocityNITRO = 90;
        float decMaxVelocity = 0.5f;
        
        float chargeNITRO = 0.0f;
        float incNITRO = 7.0f;
        float decNITRO = 3.0f;
        float MaxChargeNITRO = 100;
        float thiefNITRO = 0.0f;
        bool  thief = false;
        //==========================

        //=======Vectors============
        vec3f v_velocity= glm::vec3(0,0,0);
        vec3f v_acceleration = glm::vec3(0,0,0);
        vec3f v_rotation= glm::vec3(0,0,0);
        vec3f v_col_velocity= glm::vec3(0,0,0);
        vec3f v_col_rotation= glm::vec3(0,0,0);
        //===========================
    
        //=======Conditionals=======
        bool move= false; //Moving Key is Pressed
        bool nitro= false; 
        //==========================

        //======ElectricShock=======
        bool electric_shock = false;
        Clock* electShock_clk = new Clock();
        const float electShock_time = 0.8f;
        //========================== 

        //========Freezing==========
        bool frozen = false;
        Clock* frozen_clk = new Clock();
        const float frozen_time = 2.0f;
        //==========================

        //======Current Position====
        vec3f currentPosition = glm::vec3(0, 0, 0);
        float verticalLimit = 680.0f;
        //==========================
        
        void (*pMethod)(PhysicsVariables*, float delta) = 0;
    };

//====================================
//============FUNCTIONS===============
//====================================

    //Movement
    void UpdateMovement(PhysicsVariables*, float delta, int);
    void UpdateCollisionMovement(PhysicsVariables*, float delta);
    void CalculateCollisionRotation(PhysicsVariables*);

    void GetVectorVelocity(PhysicsVariables*);
    void GetVectorAcceleration(PhysicsVariables* data);
    void GetVectorRotation(PhysicsVariables*, float delta);

    void GetAcc(float &, float &, float, PhysicsVariables*, float delta);
    void GetRot(float &, float, float &);
    void GetSpin(PhysicsVariables*, float delta);

    void LimitVelocity(PhysicsVariables*);

    //Special effects
    void UpdateElectricShock(PhysicsVariables* data);
    void UpdateFreezing(PhysicsVariables* data);
    void SetElectricShock(PhysicsVariables* data, bool shield);
    void SetFreezing(PhysicsVariables* data, bool shield);
    bool IsElectrified(PhysicsVariables* data);
    int IsFrozen(PhysicsVariables* data, bool disabled);

    //Nitro
    void ChargeNitro(PhysicsVariables* data);
    void DecreaseNitro(PhysicsVariables* data, float delta);
    void DecreaseMaxVelocity(PhysicsVariables* data, float delta);
    void ThiefActive(PhysicsVariables* data);

    //Collisions
    void SetCollisionData(PhysicsVariables*, float delta);
    bool CheckSpheresCollision(const vec3f* , float, const vec3f* , float );
    bool CheckSphereOBBCollision(const vec3f* , const vec3f* , float );
    void CalculateSpheresCollision(const vec3f*,vec3f& , const vec3f* , float , const vec3f* ,vec3f& , const vec3f* , float );
    void CalculateSphereOBBCollision(const glm::mat3*, const vec3f*,  const std::vector<Plane>*, const vec3f* , const vec3f*, PhysicsVariables*, float, float);
    vec3f ClosestPoint(const glm::mat3*, const vec3f*, const vec3f* , const vec3f*);
    vec3f FindPlane();
    bool PointOnPlane(const vec3f*, float, const vec3f*, float&);
    bool ClosestPlane(const vec3f*, float, const vec3f*, float);
    

    //Axis
    void GetAxisY(PhysicsVariables*,  float delta);
    void GetAxisZ(PhysicsVariables*,  float delta);
    void GetAxisSpin(PhysicsVariables*, float delta);

    //Inputs
    void UpdateW(float , PhysicsVariables*);
    void UpdateA(float , PhysicsVariables*);
    void UpdateS(float , PhysicsVariables*);
    void UpdateD(float , PhysicsVariables*);
    void UpdateK(float , PhysicsVariables*);
    void UpdateI(float , PhysicsVariables*);
    void UpdateJ(float , PhysicsVariables*);
    void UpdateL(float , PhysicsVariables*);
    void UpdateSpace(float , PhysicsVariables*);
    void Keys2False(PhysicsVariables*);

    //Maths
    float GetSinDegrees(float);
    float GetCosDegrees(float);
    float GetAngleBetween2D(const vec3f& vec1, const vec3f& vec2);
    float GetAngleBetween2D2(const vec3f& vec1, const vec3f& vec2);
    float GetOrientedAngleBewteen2D(const vec3f& vec1, const vec3f& vec2);
};