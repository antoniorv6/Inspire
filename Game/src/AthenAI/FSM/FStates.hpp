
/**
 * @brief 
 * 
 */
#pragma once
#include <AIDecisionComponent.hpp>
#include <Utils.hpp>
#include <SteerLib.hpp>
#include <SensorsLib.hpp>

void Avoid(AIDecisionComponent* l_DComponent);
void Seek(AIDecisionComponent* l_DComponent);
void GoToCharge(AIDecisionComponent * l_DComponent);
void LOD(AIDecisionComponent* l_DComponent);

/**
 * @brief 
 * 
 * @param l_DComponent 
 */
void Avoid(AIDecisionComponent* l_DComponent)
{
    //std::cout << l_DComponent->GetEntityId() << " Avoid " << std::endl;

    SteerResult result = l_DComponent->m_steerModule_ptr->SteerForSeek(l_DComponent->m_obstacleSensor_ptr->CalculateAvoidanceTarget());

    *l_DComponent->m_currentVelocity_v3 = result.m_resultant_v3; 

    l_DComponent->CalculateSpinRotation(result.m_desiredVelocity_v3, *l_DComponent->m_currentVelocity_v3);

    l_DComponent->m_obstacleSensor_ptr->UpdatePerpendicularPlane();

    if(l_DComponent->m_obstacleSensor_ptr->HowMany(l_DComponent->m_octree_ptr->GetCloseObjectsV3(l_DComponent->m_rigidBody_ptr), 1000.0f, 50.0f) > 0)
    {
        l_DComponent->m_sfsm_ptr->PushState(&Avoid);
        return;
    }

    l_DComponent->m_sfsm_ptr->PopState();
}

/**
 * @brief 
 * 
 * @param l_DComponent 
 */
void Seek(AIDecisionComponent* l_DComponent)
{
    //std::cout << l_DComponent->GetEntityId() << " Seek " << std::endl;

    SteerResult result = l_DComponent->m_steerModule_ptr->SteerToFollowPath(l_DComponent->m_path_ptr);

    *l_DComponent->m_currentVelocity_v3 = result.m_resultant_v3;

    l_DComponent->CalculateFuzzyVelocity(result.m_resultant_v3, l_DComponent->m_steerModule_ptr->GetLastTarget());

    l_DComponent->CalculateSpinRotation(result.m_desiredVelocity_v3, *l_DComponent->m_currentVelocity_v3);

    l_DComponent->m_strategyBalance = l_DComponent->CalculateStrategyBalance();

    l_DComponent->m_obstacleSensor_ptr->Update();

    if(l_DComponent->m_obstacleSensor_ptr->HowMany(l_DComponent->m_closestObjects_v3, 1000.0f, 50.0f) > 0)
    {
        l_DComponent->m_sfsm_ptr->PushState(&Avoid);
        return;
    }

    ////std::cout << "Mi carga de nitro es: " << l_DComponent->m_physicsData_ptr->chargeNITRO << std::endl;

    if(l_DComponent->m_obstacleSensor_ptr->HowManyNitros(l_DComponent->m_nitroZonesMap_m, 1600.0f, 60.0f) > 0)
    {
        float l_desirability_f = l_DComponent->GetGoToChargeDesirability(l_DComponent->m_obstacleSensor_ptr->GetNitroCandidates());
        /**  [WARNING]: Check if this is the best value, or if we have to modify the fuzzy rules set. 
         *   ALSO, now if we take a look, we're now working with std::map instead std::vectors, take this in count.
        */
        if(l_desirability_f > 0.6f)
        {
            ////std::cout << "Deseable ir a por nitro\n";
            if(l_DComponent->m_notCharge_ptr->GetElapsedTime() > 3.0f)
            {
                l_DComponent->m_sfsm_ptr->PushState(&GoToCharge);
            }
            else
            {
                //std::cout << "NO PUSH CHARGE\n";
            }
        }
    }

    l_DComponent->CalculateNitro();

    l_DComponent->CalculatePowerUp();
}

/**
 * @brief 
 * 
 * @param l_DComponent 
 */
void GoToCharge(AIDecisionComponent * l_DComponent)
{
    /** [WARNING]: Check if is neccessary to continue chargin nitro. 
     *  ANYWAY, I think this is not what we have to check to come back to follow the path.
     * [TO_DO]: think about any other condition to come back.
     * */
    // l_DComponent->m_obstacleSensor_ptr->HowMany(l_DComponent->m_nitroWaypointsMap_m[l_DComponent->m_selectedNitroWaypoints_n], 2000.0f, 70.0f) == 0 ||
    
    //A lot of time passed from last collision, go to charge
    if(l_DComponent->m_notCharge_ptr->GetElapsedTime() > 3.0f)
    {
        if(glm::distance(*l_DComponent->m_currentPosition_v3, l_DComponent->m_nitroWaypointsMap_m[l_DComponent->m_selectedNitroWaypoints_n][l_DComponent->m_indexNW_n]) <= 500.0f ||
        l_DComponent->m_physicsData_ptr->chargeNITRO == 100)  
        {
            l_DComponent->m_indexNW_n++;

            if(l_DComponent->m_indexNW_n > (int)(l_DComponent->m_nitroWaypointsMap_m[l_DComponent->m_selectedNitroWaypoints_n].size() - 1))
            {
                ////std::cout << l_DComponent->GetEntityId() << " FindClosest\n";
                l_DComponent->m_indexNW_n = 0;
                l_DComponent->m_sfsm_ptr->PopState();
                l_DComponent->FindClosest(l_DComponent->m_nitroWaypointsMap_m[l_DComponent->m_selectedNitroWaypoints_n][l_DComponent->m_indexNW_n]);

                return;
            }
        }

        //std::cout << l_DComponent->GetEntityId() << " Go to charge " << l_DComponent->m_selectedNitroWaypoints_n << " - " << l_DComponent->m_indexNW_n << std::endl;

        SteerResult result = l_DComponent->m_steerModule_ptr->SteerForSeek(l_DComponent->m_nitroWaypointsMap_m[l_DComponent->m_selectedNitroWaypoints_n][l_DComponent->m_indexNW_n]);

        *l_DComponent->m_currentVelocity_v3 = result.m_resultant_v3;

        l_DComponent->CalculateSpinRotation(result.m_desiredVelocity_v3, *l_DComponent->m_currentVelocity_v3);
    }
    //If had been a collision in last 3 seconds, not go to charge
    else
    {
        //std::cout << "POP CHARGE\n";
        l_DComponent->m_sfsm_ptr->PopState();
    }
}


/**
 * @brief 
 * 
 * @param l_DComponent 
 */
void LOD(AIDecisionComponent * l_DComponent)
{
    ////std::cout << l_DComponent->GetEntityId() << " LOD " << std::endl;

    // Calculate LOD velocity
    l_DComponent->CalculateLODVelocity();

    // Calculate desired velocity
    *l_DComponent->m_currentVelocity_v3 = l_DComponent->m_steerModule_ptr->LODFollowPath(l_DComponent->m_path_ptr, l_DComponent->m_LODVelocity_f);

    // Calculate angle spin
    l_DComponent->m_physicsData_ptr->RotSPIN = CalculateSpinAngle2(*l_DComponent->m_currentVelocity_v3);

}