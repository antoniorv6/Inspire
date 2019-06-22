#include <PICASSO.hpp>
#include <iostream>
#include <math.h>

/*
    #####  #  #####  #####  #####  #####  #####
    #   #  #  #      #   #  #      #      #   #
    #####  #  #      #####  #####  #####  #   #
    #      #  #      #   #      #      #  #   #
    #      #  #####  #   #  #####  #####  #####

    Inspire's Physics Engine by Zenon Games
*/

//======================== MOVEMENT ===========================

void PICASSO::UpdateMovement(PICASSO::PhysicsVariables* data, float delta, int id){
    
    ThiefActive(data);

    //IF WE ARE NOT ELECTRIFIED OR FROZEN
    if( !IsElectrified(data) && IsFrozen(data, false) == 0)
    {
        if(id == 0)
        {
            //================MOVEMENT FORWARD===================
            data->pMethod = GetAxisY;
            //std::cout << "NITRO: " << data->chargeNITRO << std::endl;
            if(data->nitro && data->thiefNITRO >= 0)
            {
                data->KeyW = 1.0f;
                GetAcc(data->accFWRD, data->AxisY, data->MaxAccNITRO, data, delta);
                DecreaseNitro(data, delta);
                data->KeyW = 0.0f;
                //std::cout << "N BAJANDO: " << data->chargeNITRO << std::endl;
            }
            else
            {
                DecreaseMaxVelocity(data, delta);
                GetAcc(data->accFWRD, data->AxisY, data->MaxAcc, data, delta);
            }
            GetRot(data->RotFWRD, -data->AxisY, data->MaxRotFWRD);
            //===================================================

            //================MOVEMENT UP/DOWN===================
            data->pMethod = GetAxisZ;
            GetAcc(data->accUP, data->AxisZ, data->MaxAcc, data, delta);
            //===================================================

            //================MOVEMENT SPIN======================
            data->pMethod = GetAxisSpin;
            GetRot(data->RotSIDE, data->AxisSpin, data->MaxRotSIDE);
            GetSpin(data, delta);
            //===================================================
            //std::cout << data->v_acceleration.x << "   " << data->v_acceleration.y << "   " << data->v_acceleration.z << std::endl;
            //std::cout << data->v_velocity.x << "   " << data->v_velocity.y << "   " << data->v_velocity.z << std::endl;
            GetVectorAcceleration(data);
            GetVectorVelocity(data);

        }
        
    }

    GetVectorRotation(data, delta);
}

void PICASSO::UpdateCollisionMovement(PICASSO::PhysicsVariables* data, float delta){

    data->v_velocity += data->v_col_velocity;
    CalculateCollisionRotation(data);
}

void PICASSO::CalculateCollisionRotation(PICASSO::PhysicsVariables* data){
    
    float rotF, rotS;

    rotF = data->v_col_velocity.y*GetCosDegrees(data->RotSPIN)  +  data->v_col_velocity.x*(-1 * GetSinDegrees(data->RotSPIN));
    rotS = data->v_col_velocity.y*GetSinDegrees(data->RotSPIN)  +  data->v_col_velocity.x* GetCosDegrees(data->RotSPIN);

    data->v_col_rotation.x =  rotF * -2;
    data->v_col_rotation.y =  rotS * 2; 

}

void PICASSO::GetVectorVelocity(PICASSO::PhysicsVariables* data){

    data->v_velocity += data->v_acceleration;
    data->v_velocity.z *= 0.95f; // vertical velocity lowwwww
    LimitVelocity(data);

}

void PICASSO::GetVectorAcceleration(PICASSO::PhysicsVariables* data){

    data->v_acceleration.x = -data->accFWRD*GetSinDegrees(data->RotSPIN);
    data->v_acceleration.y = data->accFWRD*GetCosDegrees(data->RotSPIN);
    data->v_acceleration.z = data->accUP;

}

void PICASSO::GetVectorRotation(PICASSO::PhysicsVariables* data, float delta){


    data->v_rotation.x = data->RotFWRD; 
    data->v_rotation.y = data->RotSIDE;
    data->v_rotation.z = data->RotSPIN;

    /*ADD COLLISION ROTATION*/
    data->v_col_rotation -= data->v_col_rotation / (20.0f / delta);
    data->v_rotation += data->v_col_rotation;
}

void PICASSO::GetAcc(float &acc, float &axis, float maxacc, PhysicsVariables* data, float delta){
    //Obtenemos el get axis x para calcular la acc
    data->pMethod(data, delta);
    acc = axis * maxacc;
}

void PICASSO::LimitVelocity(PhysicsVariables* data){

        /*Apply AirFriction*/
        if(glm::length(data->v_velocity) !=0)
            data->v_velocity = glm::normalize(data->v_velocity) * (glm::length(data->v_velocity) * data->AirFriction) ;

        /*Limit velocity MAX*/
        data->v_velocity = glm::length(data->v_velocity) < data->MaxVelocity ? data->v_velocity : glm::normalize(data->v_velocity) * data->MaxVelocity;
    
        for(unsigned int i=0; i<3; i++){
            if(glm::abs(data->v_velocity[i])<0.001)
                data->v_velocity[i]=0;
        }
}

void PICASSO::GetRot(float &rot, float axis, float &maxrot){
    rot = axis * maxrot;
}

void PICASSO::GetSpin(PICASSO::PhysicsVariables* data, float delta){
    GetAxisSpin(data, delta);
    if(data->AxisSpin!=0){ 
        //data->v_velocity = glm::length(data->v_velocity) < data->MaxVelocity ? data->v_velocity : glm::normalize(data->v_velocity) * data->MaxVelocity;  
        float increment = data->incRotSPIN * data->AxisSpin;
        if(glm::abs(increment) > data->incRotSPIN)
        { 
            increment = copysign(data->incRotSPIN, increment);
        } 
        data->RotSPIN -= increment;

        while(glm::abs(data->RotSPIN)>data->MaxRotSPIN){
            data->MaxRotSPIN = copysign(data->MaxRotSPIN, data->RotSPIN);
            data->RotSPIN -= data->MaxRotSPIN;
        }
    }
}

//======================== COLLISIONS ===========================

    bool PICASSO::CheckSpheresCollision(const vec3f* pos1, float rad1, const vec3f* pos2, float rad2){
        double deltaXSquared = (*pos1).x - (*pos2).x; // calc. delta X
        deltaXSquared *= deltaXSquared; // square delta X
        double deltaYSquared = (*pos1).y - (*pos2).y; // calc. delta Y
        deltaYSquared *= deltaYSquared; // square delta Y
        double deltaZSquared = (*pos1).z - (*pos2).z; // calc. delta Y
        deltaZSquared *= deltaZSquared; // square delta Y

        // Calculate the sum of the radii, then square it
        double sumRad = rad1 + rad2; 
        sumRad *= sumRad;

        if(deltaXSquared + deltaYSquared + deltaZSquared <= sumRad){
            return true;
        }
        return false;
    }

    bool PICASSO::CheckSphereOBBCollision(const vec3f* sph_pos, const vec3f* cl_point, float rad){
        return (glm::distance(*sph_pos,*cl_point) <= rad);
    }

    void PICASSO::CalculateSpheresCollision(const vec3f* vel1, vec3f& velcol1,  const vec3f* pos1,  float mass1, const vec3f* vel2, vec3f& velcol2,  const vec3f* pos2,  float mass2) {
        // First, find the normalized vector n from the center of 
        // circle1 to the center of circle2
        vec3f n = *pos1 - *pos2;
        n = glm::normalize(n);
        // Find the length of the component of each of the movement
        // vectors along n. 
        // a1 = v1 . n
        // a2 = v2 . n
        float a1 = glm::dot(*vel1, n);
        float a2 = glm::dot(*vel2, n);

        // Using the optimized version, 
        // optimizedP =  2(a1 - a2)
        //              -----------
        //                m1 + m2
        float optimizedP = (2.0 * (a1 - a2)) / (mass1 + mass2);

        // Calculate v1', the new movement vector of circle1
        // v1' = v1 - optimizedP * m2 * n
        velcol1 = *vel1 - optimizedP * mass2 * n;

        // Calculate v1', the new movement vector of circle1
        // v2' = v2 + optimizedP * m1 * n
        velcol2 = *vel2 + optimizedP * mass1 * n;
    
    }

    void PICASSO::CalculateSphereOBBCollision(const glm::mat3* rot, const vec3f* size, const std::vector<Plane>* faces, const vec3f* obb_pos, const vec3f* sph_pos,
                                              PhysicsVariables* sph_data, float sph_mass, float obb_mass)
    {
        vec3f l_closestPoint_v3 = ClosestPoint(rot, size, obb_pos, sph_pos);
        int l_colPlane_n = -1;
        int l_numPlanes_n = 0;
        float l_shortest_f = std::numeric_limits<float>::max();

        for(unsigned int j=0; j<6; j++){
            if(PointOnPlane(&(*faces)[j].normal, (*faces)[j].d, &l_closestPoint_v3, l_shortest_f)){
                 
                 l_colPlane_n = j;
                 l_numPlanes_n++;                                
            }
        }
        if(l_numPlanes_n == 0){
             for(unsigned int j=0; j<6; j++){
                 if(ClosestPlane(&(*faces)[j].normal, (*faces)[j].d, &l_closestPoint_v3, l_shortest_f)){
                     l_colPlane_n = j;
                     l_numPlanes_n++;
                 }
             }
        }
         
        if(l_numPlanes_n == 1){
            sph_data->v_col_velocity = glm::reflect(sph_data->v_velocity, (*faces)[l_colPlane_n].normal);
        }
        else{
            glm::vec3 vel = {0,0,0};  
            PICASSO::CalculateSpheresCollision(&sph_data->v_velocity, sph_data->v_col_velocity, sph_pos, sph_mass, &vel, vel, obb_pos, obb_mass);
        }
    }

    glm::vec3 PICASSO::ClosestPoint(const glm::mat3* rot, const vec3f* size, const vec3f* obb_pos, const vec3f* sph_pos) { 
        vec3f result = *obb_pos;
        vec3f dir = *sph_pos - *obb_pos;

        for (int i = 0; i < 3; ++i) {
            vec3f axis = (*rot)[i];

            float distance = glm::dot(dir, axis);

            if (distance > (*size)[i]) {
                distance = (*size)[i];
            }
            if (distance < -(*size)[i]) {
                distance = -(*size)[i];
            }

            result = result + (axis * distance);
        }

        return result;
    }

    bool PICASSO::PointOnPlane(const vec3f* normal, float d, const vec3f* closestpoint, float &shortest){
        float dist = (glm::dot(*normal, *closestpoint) - d);
        
        if(glm::abs(dist) <= shortest)
            shortest = glm::abs(dist);
    
        return (glm::abs(dist) == 0);
    }

    bool PICASSO::ClosestPlane(const vec3f* normal, float d, const vec3f* closestpoint, float shortest){
        float dist = (glm::dot(*normal, *closestpoint) - d);        
        return (glm::abs(dist) == shortest);
    }

    void PICASSO::SetCollisionData(PICASSO::PhysicsVariables* data, float delta){
        data->AxisY = 0.0f;
        data->AxisZ = 0.0f;
        data->AxisSpin = 0.0f;

        data->RotSIDE = 0;
        data->RotFWRD = 0;
        data->v_acceleration = {0,0,0};
        data->v_velocity = {0,0,0};
        data->v_rotation = {0,0,data->RotSPIN};
        data->v_col_velocity /= 2.0f;
        UpdateCollisionMovement(data,delta);
    }

//========================== SPECIAL EFFECTS ===============================

    void PICASSO::SetElectricShock(PICASSO::PhysicsVariables* data, bool shield)
    {
        if(!shield) //if we havent shield, we are electrified
        {                
            data->electric_shock = true;
            data->electShock_clk->Init();
        }
    }

    void PICASSO::UpdateElectricShock(PICASSO::PhysicsVariables* data){
        data->v_velocity = glm::normalize(data->v_velocity) * (glm::length(data->v_velocity) * 0.95f) ;
    }

    bool PICASSO::IsElectrified(PICASSO::PhysicsVariables* data)
    {
        if(!data->electric_shock || data->electShock_clk->GetElapsedTime() > data->electShock_time)
        {
            data->electric_shock = false;

            return false;
        }
        
        else
        {
            UpdateElectricShock(data);
            return true;
        }
    }
    

    void PICASSO::SetFreezing(PICASSO::PhysicsVariables* data, bool shield)
    {
        if(!shield) //if we havent shield, the drone is frozen
        {    
            data->frozen = true;
            data->frozen_clk->Init();
        }
    }

    void PICASSO::UpdateFreezing(PICASSO::PhysicsVariables* data){

        data->v_acceleration = {0,0,-1.5};
        data->v_velocity += data->v_acceleration;
        data->v_velocity = glm::normalize(data->v_velocity) * (glm::length(data->v_velocity) * 0.95f);
        //std::cout << data->frozen_clk->GetElapsedTime() << std::endl;
        
    }

    int PICASSO::IsFrozen(PICASSO::PhysicsVariables* data, bool disabled)
    {
        if(data->frozen)
        {
            if(data->frozen_clk->GetElapsedTime() > data->frozen_time)
            {
                if(disabled)
                    data->frozen = false;

                return -1;
            }
            else
            {
                UpdateFreezing(data);
                return 1;
            }
        }
        else
        {
            return 0;
        }
        
    }

//========================== NITRO ===============================

    void PICASSO::ChargeNitro(PICASSO::PhysicsVariables* data)
    {
        if(!data->thief)
        {
            data->chargeNITRO += data->incNITRO;
            if(data->chargeNITRO > data->MaxChargeNITRO)
                data->chargeNITRO = data->MaxChargeNITRO;
            ////std::cout << "NITRO: " << data->chargeNITRO << std::endl;
        }
        
    }

    void PICASSO::DecreaseNitro(PICASSO::PhysicsVariables* data, float delta)
    {
        if(!data->thief)
        {
            data->chargeNITRO -= (data->decNITRO * delta);

            if(glm::length(data->v_velocity + data->v_acceleration) <= data->MaxVelocityNITRO && glm::length(data->v_velocity + data->v_acceleration) > data->MaxVelocity)
                data->MaxVelocity = glm::length(data->v_velocity + data->v_acceleration);
            else 
                data->MaxVelocity = data->MaxVelocityNITRO;

            if(data->chargeNITRO <= 0){
                data->chargeNITRO = 0;
                data->nitro = false;
            }
        }
    }

    void PICASSO::DecreaseMaxVelocity(PICASSO::PhysicsVariables* data, float delta)
    {
        if(data->MaxVelocity > data->MaxVelocityStandar)
            data->MaxVelocity -= (data->decMaxVelocity * delta);
    }

    void PICASSO::ThiefActive(PICASSO::PhysicsVariables* data)
    {
        if(data->thief)
        {
            
            if(glm::abs(data->thiefNITRO) > 0)
            {
                data->thiefNITRO -= copysign(1.0f, data->thiefNITRO);
            }
            else
            {
                data->thief = false;
            }
            
            data->chargeNITRO += copysign(1.0f, data->thiefNITRO);
            
            if(data->chargeNITRO > data->MaxChargeNITRO)
                data->chargeNITRO = data->MaxChargeNITRO;   

            if(data->chargeNITRO <= 0)
            {
                data->chargeNITRO = 0;
                data->nitro = false;
            }

            if(glm::abs(data->thiefNITRO) > 0 && glm::abs(data->thiefNITRO) < 1)
            {
                data->thiefNITRO = 0;
            }
        }
    }


//========================== AXIS ===============================


void PICASSO::GetAxisY(PICASSO::PhysicsVariables* data, float delta){
    
    //Avanzamos
    if(data->KeyW==1.0f && data->AxisY<1){
        data->AxisY+=data->incAxis * delta;
        data->move=true;


    //Retrodecemos
    }else if(data->KeyS==1.0f && data->AxisY>-1){
        data->AxisY-=data->incAxis * delta;
        data->move=true;
    }
    else if(data->KeyS==0.0f && data->KeyW==0.0f && data->AxisY!=0){
       
        if(glm::abs(data->AxisY) < glm::abs(data->decAxis * delta)){
            data->AxisY=0;
        }else{
            data->decAxis = copysign(data->decAxis,data->AxisY);
            data->AxisY-=data->decAxis * delta;
        }
        
        data->move=false;
    }
}

void PICASSO::GetAxisZ(PICASSO::PhysicsVariables* data, float delta){
    
    //Subimos
    if(data->KeyI > 0 && data->AxisZ<1){
        data->AxisZ+=data->incAxis * delta * 3 * data->KeyI;
        data->move=true;

    //Bajamos
    }else if(data->KeyK > 0 && data->AxisZ>-1){
        data->AxisZ-=data->incAxis * delta * 3 *  data->KeyK;
        data->move=true;
    }
    else if(data->KeyK==0.0f && data->KeyI==0.0f && data->AxisZ!=0){
       
        if(glm::abs(data->AxisZ) < glm::abs(data->decAxis * delta * 3)){
            data->AxisZ=0;
        }else{
            data->decAxis = copysign(data->decAxis,data->AxisZ);
            data->AxisZ-=data->decAxis * delta;
        }
        
        data->move=false;
    }
}

void PICASSO::GetAxisSpin(PICASSO::PhysicsVariables* data, float delta){
    
    //Giramos Derecha
    if(data->KeyD > 0 && data->AxisSpin>-1){
        data->AxisSpin-=data->incAxis * delta * 6 * data->KeyD;
        data->move=true;

    //Giramos IXquierda
    }else if(data->KeyA > 0 && data->AxisSpin<1){
        data->AxisSpin+=data->incAxis * delta * 6 * data->KeyA;
        data->move=true;
    }
    else if(data->KeyA==0.0f && data->KeyD==0.0f && data->AxisSpin!=0){

        if(glm::abs(data->AxisSpin) < glm::abs(data->decAxis * delta * 6)){
            data->AxisSpin=0;
        }else{
            data->decAxis = copysign(data->decAxis,data->AxisSpin);
            data->AxisSpin-=data->decAxis * delta * 6;
        }
        
        data->move=false;
    }

}

//=========================INPUTS================================

void PICASSO::UpdateW(float value, PICASSO::PhysicsVariables* data){
    data->KeyW = value;
}
void PICASSO::UpdateA(float value, PICASSO::PhysicsVariables* data){
    data->KeyA = value;
}
void PICASSO::UpdateS(float value, PICASSO::PhysicsVariables* data){
    data->KeyS = value;
}
void PICASSO::UpdateD(float value, PICASSO::PhysicsVariables* data){
    data->KeyD = value;
}
void PICASSO::UpdateK(float value, PICASSO::PhysicsVariables* data){
    data->KeyK = value;
}
void PICASSO::UpdateI(float value, PICASSO::PhysicsVariables* data){
    data->KeyI = value;
}
void PICASSO::UpdateSpace(float value, PICASSO::PhysicsVariables* data){
    if(data->chargeNITRO > 0)
        data->nitro = value;
}

void PICASSO::Keys2False(PICASSO::PhysicsVariables* data)
{
    data->KeyW = 0.0f;
    data->KeyA = 0.0f;
    data->KeyS = 0.0f;
    data->KeyD = 0.0f;
    data->KeyI = 0.0f;
    data->KeyK = 0.0f;
    data->nitro = false;
}

//========================== MATH ===============================

float PICASSO::GetSinDegrees(float degrees){

    float sinus = sin(degrees*M_PI/180);
    return sinus;

}

float PICASSO::GetCosDegrees(float degrees){
    float cosinus = cos(degrees*M_PI/180);
    return cosinus;
}

/**
 * PICASSO::GetAngleBetween2D
 *
 * @brief
 *
 * @param vec1
 * @param vec2
 * @return float
 */
float PICASSO::GetAngleBetween2D(const vec3f& vec1, const vec3f& vec2)
{
    return glm::angle(glm::normalize(glm::vec3(vec1.x, vec1.y, vec1.z)), glm::normalize(glm::vec3(vec2.x, vec2.y, vec2.z))) * 180.0f / M_PI;
}
 
/**
 * PICASSO::GetAngleBetween2D
 *
 * @brief
 *
 * @param vec1
 * @param vec2
 * @return float
 */
float PICASSO::GetAngleBetween2D2(const vec3f& vec1, const vec3f& vec2)
{
    return glm::angle(glm::normalize(glm::vec3(vec1.x, vec1.y, 0.0f)), glm::normalize(glm::vec3(vec2.x, vec2.y, 0.0f))) * 180.0f / M_PI;
}

/**
 * @brief 
 * 
 * @param vec1 
 * @param vec2 
 * @return float 
 */
float PICASSO::GetOrientedAngleBewteen2D(const vec3f& vec1, const vec3f& vec2)
{
    return glm::orientedAngle(glm::normalize(glm::vec2(vec1.x, vec1.y)), glm::normalize(glm::vec2(vec2.x, vec2.y))) * 180.0f / M_PI;
}
