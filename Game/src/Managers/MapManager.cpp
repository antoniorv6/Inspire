#include <MapManager.hpp>
#include <rapidxml.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <glm.hpp>
#include <Entity.h>
#include <Utils.hpp>

#include <EntityManager.h>
#include <WaypointManager.hpp>
#include <PhysicsManager.h>
#include <RenderManager.h>
#include <CheckpointManager.hpp>
#include <GenericData.hpp>

#include <GUI.hpp>

/**
 * @brief Construct a new Map Manager:: Map Manager object
 * 
 */
MapManager::MapManager()
{
	m_initialPositions_vec.reserve(4);
    m_levels_vec.reserve(1);
    m_levels_vec.emplace_back("assets/level.xml");
}

/**
 * @brief Destroy the Map Manager:: Map Manager object
 * 
 */
MapManager::~MapManager()
{
	
}

/**
 * @brief 
 * 
 * @param c_genericData 
 */
void MapManager::Init(GenericData *&c_genericData)
{	
	checkpointManager = c_genericData->g_checkPointManager_ptr;
    entityManager = c_genericData->g_entityManager_ptr;
    render = c_genericData->g_renderManager_ptr;
    checkpointManager = c_genericData->g_checkPointManager_ptr;
    physicsManager = c_genericData->g_physicsManager_ptr;
	waypointManager = c_genericData->g_waypointManager_ptr;

	//Reset paths variables (por multiple loads of a Level).


	m_paths_v.reserve(3);
	m_paths_v.push_back(std::vector<glm::vec3>());
	m_paths_v.push_back(std::vector<glm::vec3>());
	m_paths_v.push_back(std::vector<glm::vec3>());
}	

/**
 * @brief: reads an XML document and creates our Game's entities.
 * 
 * @param c_level_i 
 * @param c_loadPhysics_b 
 * @param c_loadRender_b 
 * @param c_onlineMatch_b 
 */
void MapManager::LoadLevel(unsigned int c_level_i, bool c_loadPhysics_b, bool c_loadRender_b, bool c_onlineMatch_b)
{
    using namespace rapidxml;

	xml_document<> xml;
	xml_node<>* root_node;

	std::ifstream file(m_levels_vec[c_level_i]);
	std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	xml.parse<0>(&buffer[0]);

	root_node = xml.first_node("object");

	//Identifiers counter
	unsigned int id = 1;
	unsigned int checkpoint_num = 0;
	//Additional atributes

	m_selectedWaypointsVector_n = 0;
	m_waypointsNitros_v.clear();
	for(unsigned int i = 0; i < m_paths_v.size(); i++){
		m_paths_v[i].clear();
	}

	bool nitro;
	bool checkpoint;
	bool electroshock;
	//Transform attributes
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 translationGL;
	glm::vec3 rotationGL;
	glm::vec3 scaleGL;

	//For lights loading.
	glm::vec3 auxRotation = glm::vec3(0.0f);
	glm::vec3 auxScale    = glm::vec3(0.0f);
	//Other stuff
	char   * objectName;
	Entity * scene;

	for (xml_node<> * object = root_node; object; object = object->next_sibling()) 
	{
		
		objectName = object->first_attribute("name")->value();

		translation.x = atof(object->first_attribute("translate")->value());
		translation.y = atof(object->first_attribute("translateY")->value());
		translation.z = atof(object->first_attribute("translateZ")->value());

		rotation.x = atof(object->first_attribute("rotateX")->value());
		rotation.y = atof(object->first_attribute("rotateY")->value());
		rotation.z = atof(object->first_attribute("rotateZ")->value());

		//For lights loading.
		auxRotation = rotation;

		scale.x = atof(object->first_attribute("scale")->value());
		scale.y = atof(object->first_attribute("scaleY")->value());
		scale.z = atof(object->first_attribute("scaleZ")->value());

		auxScale = scale;

		nitro = false;
		checkpoint = false;
		electroshock = false;

		translationGL = translation;
		rotationGL	  = rotation;
		scaleGL		  = scale;
		
		translation = {-translation.x, -translation.z, translation.y};
		rotation    = {rotation.x, rotation.z, -rotation.y};
		scale 		= {scale.x, scale.z, scale.y};

		// Parse WaypointComponent
		if(strcmp(object->first_attribute("waypoint")->value(),"True") == 0)
		{
			if(!c_onlineMatch_b)
			{

				//show_vector("Posicion Waypoint", translation);
				m_paths_v[(int)rotation.x].push_back(translation);
				
				// glm::vec3 pos = translation;
				// pos = { -pos.x, pos.z, -pos.y}; //irrlitch axis to openGL axis
				// render->AddDebugCube(id, pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(50.0f, 50.0f, 50.0f));
				
				
				//waypointManager->CreateWayPoint(object->first_attribute("name")->value(), translation);
			}
			id++;
		}
		else if(object->first_attribute("particles")!=NULL && strcmp(object->first_attribute("particles")->value(),"True") == 0)
		{
			std::string l_texture  = "no_texture.jpg";

			if(object->first_attribute("texture")!=NULL && strcmp(object->first_attribute("texture")->value(),"None") != 0)
			{
				l_texture = object->first_attribute("texture")->value();
			}

			render->GetRenderFacade()->AddParticlesEmitter("thunder", "thunderTexture.jpg", translation, (int)rotation.x);
		}
		else if(strcmp(object->first_attribute("wn")->value(),"True") == 0)
		{
			if(!c_onlineMatch_b)
			{
				std::string parent = object->first_attribute("nitroParent")->value();
				m_waypointsNitros_v[std::stoi(parent)].push_back(translation);
			}
		}
		else if(object->first_attribute("position")!=NULL && strcmp(object->first_attribute("position")->value(),"True") == 0)
		{
			m_initialPositions_vec.emplace_back(translation);
		}

		else if(object->first_attribute("light")!=NULL && strcmp(object->first_attribute("light")->value(),"True") == 0)
		{
			render->GetRenderFacade()->AddLight(translation, auxRotation, auxScale);
		}
		//RigidBodys
		else
		{
			scene = entityManager->CreateEntity("Scene"+std::to_string(id), translation);

			// Check if rendering option is enabled

			if(object->first_attribute("render")!=NULL && strcmp(object->first_attribute("render")->value(),"True") == 0)
			{
				if(c_loadRender_b)
				{
					if(object->first_attribute("lod")!=NULL && strcmp(object->first_attribute("lod")->value(),"True") == 0)
					{
						render->CreateComponent(scene, "LOD", objectName);
						render->AddNode(scene);
					}
					else
					{
						int outline = atoi(object->first_attribute("outline")->value());
						render->CreateComponent(scene, "Mesh", objectName);
						render->AddNode(scene, outline);
					}
					
				}
			}

			id++;

			// Parse CheckpointComponent
			if(object->first_attribute("checkpoint")!=NULL && strcmp(object->first_attribute("checkpoint")->value(),"True") == 0)
			{
				checkpoint = true;
				float vertical_limit = 800.0f; // default value
				bool change_of_zone = false;

				//Vertical limit from every checkpoint
				if(object->first_attribute("vlimit")!=NULL && strcmp(object->first_attribute("vlimit")->value(),"None") != 0)
				{
					std::string vertical_limit_str = object->first_attribute("vlimit")->value();
					vertical_limit = (float) std::stoi(vertical_limit_str);
				}

				//
				if(object->first_attribute("zone")!=NULL && strcmp(object->first_attribute("zone")->value(),"True") == 0)
				{
					change_of_zone = true;
				}

				checkpointManager->CreateComponent(scene, checkpoint_num, vertical_limit, change_of_zone);
				checkpoint_num++;
			}
			// [NEW]  Parse if this object has electroshock effect or not 
			if(object->first_attribute("electro")!=NULL && strcmp(object->first_attribute("electro")->value(),"True") == 0)
			{
				electroshock = true;
			}
			// [NEW]  Parse if this object is a energy zone or not 
			if(object->first_attribute("energyzone")!=NULL && strcmp(object->first_attribute("energyzone")->value(),"True") == 0)
			{
				nitro = true;
			}

			// Parse RigidbodyComponent
			if(strcmp(object->first_attribute("rigidbody")->value(),"True") == 0 && c_loadPhysics_b)
			{
				// std::cout << "StartLine: " << startline << std::endl;
				// std::cout << "Nitro: " << nitro << std::endl;

				physicsManager->CreateRigidBodyRectangle(scene, scale.x, scale.z, scale.y, 3000.0f, rotation.x, rotation.y, rotation.z, nitro, checkpoint, electroshock);
				
				//render->AddDebugCube(0, translationGL, rotationGL, scaleGL);
	
				//RigidBodyComponent* rigid = reinterpret_cast<RigidBodyComponent*>(entityManager->GetComponent(scene->GetId(), "RigidBody"));

				// //Show BoundingBox Vertex
				// for(size_t i = 0; i < 8; i++)
				// {
				// 	glm::vec3 pos = rigid->getRectangleVertex(i);
				// 	pos = { -pos.x, pos.z, -pos.y}; //irrlitch axis to openGL axis
				// 	//std::cout << "Vertice " << i << ": " << pos.x << " " << pos.y << " " << pos.z << std::endl;
				// 	render->AddDebugCube(i, pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(50.0f, 50.0f, 50.0f));
				// }
			}

			if(object->first_attribute("texture")!=NULL && strcmp(object->first_attribute("texture")->value(),"None") != 0)
			{
				render->GetRenderFacade()->AddTexture(scene->GetId(), object->first_attribute("texture")->value());
			}
			else
			{
				render->GetRenderFacade()->AddTexture(scene->GetId(), "no_texture.jpg");
			}
				
		} 
	
		m_GUI->AddProgressToLoadingBar(0.15);
		render->GetRenderFacade()->DrawGUI();

	}

}

/**
 * @brief: returns an avaiable path for an AI entity.
 * 
 * @return std::vector<glm::vec3> 
 */
std::vector<glm::vec3> MapManager::GetPath()
{
	m_selectedWaypointsVector_n++;

	if((unsigned int)m_selectedWaypointsVector_n-1 >= m_paths_v.size())
	{
		m_selectedWaypointsVector_n = 1;
	}

	return m_paths_v[m_selectedWaypointsVector_n-1];
}

void MapManager::SetGUI(GUI*& c_GUI)
{
	m_GUI = c_GUI;
}
void MapManager::SetPlayer(Entity* c_player)
{
	m_player = c_player;
}