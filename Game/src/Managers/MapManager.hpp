#include <string>
#include <vector>
#include <map>
#include <glm.hpp>

class EntityManager;
class RenderManager;
class WaypointManager;
class PhysicsManager;
class CheckpointManager;
class GenericData;
class GUI;
class Entity;

class MapManager
{
    public:
        MapManager();
        ~MapManager();
        void Init(GenericData *&c_genericData);
        void LoadLevel(unsigned int c_level_i, bool c_loadPhysics_b, bool c_loadRender_b, bool c_onlineMatch_b);
        std::map<int, std::vector<glm::vec3>> GetNitroWaypoints() const { return m_waypointsNitros_v; };
        std::vector<glm::vec3>* GetInitialPosition() { return &m_initialPositions_vec; };
        std::vector<glm::vec3> GetPath();

        void SetGUI(GUI*& c_GUI);
        void SetPlayer(Entity* c_player);

    private:
        std::vector<std::string> m_levels_vec;
        std::map<int, std::vector<glm::vec3>> m_waypointsNitros_v;
        std::vector<glm::vec3> m_initialPositions_vec;
        std::vector<std::vector<glm::vec3>> m_paths_v;
        int m_selectedWaypointsVector_n = 0;
        
        EntityManager* entityManager           = nullptr;
        RenderManager* render                  = nullptr;
        WaypointManager* waypointManager       = nullptr;
        CheckpointManager* checkpointManager   = nullptr;
        PhysicsManager* physicsManager         = nullptr;

        Entity* m_player = nullptr;
        GUI*    m_GUI = nullptr;

};
    /*
	<object checkpoint="False" electro="False" energyzone="False" name="pCube129" nitroParent="0" obstacle="False" position="True" render="False" rigidbody="False" rotateX="0.0" rotateY="0.0" rotateZ="0.0" scale="1.0" scaleY="175.808980415" scaleZ="175.808980415" translate="-5380.60171502" translateY="57.6566657653" translateZ="-11523.7054848" waypoint="False" wn="False"/>
	<object checkpoint="False" electro="False" energyzone="False" name="pCube127" nitroParent="0" obstacle="False" position="True" render="False" rigidbody="False" rotateX="0.0" rotateY="0.0" rotateZ="0.0" scale="2.0" scaleY="175.808980415" scaleZ="175.808980415" translate="-5404.08102187" translateY="57.6566657653" translateZ="-10973.9234149" waypoint="False" wn="False"/>
	<object checkpoint="False" electro="False" energyzone="False" name="pCube130" nitroParent="0" obstacle="False" position="True" render="False" rigidbody="False" rotateX="0.0" rotateY="0.0" rotateZ="0.0" scale="3.0" scaleY="175.808980415" scaleZ="175.808980415" translate="-5994.66333917" translateY="57.6566657653" translateZ="-11523.7054848" waypoint="False" wn="False"/>
	<object checkpoint="False" electro="False" energyzone="False" name="pCube128" nitroParent="0" obstacle="False" position="True" render="False" rigidbody="False" rotateX="0.0" rotateY="0.0" rotateZ="0.0" scale="4.0" scaleY="175.808980415" scaleZ="175.808980415" translate="-5973.52011772" translateY="57.6566657653" translateZ="-10973.9234149" waypoint="False" wn="False"/>
    
    */
   