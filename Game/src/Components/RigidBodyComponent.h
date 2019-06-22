#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include <bitset>
#include <string>
#include <IComponent.h>
#include <EntityManager.h>
#include <gtx/rotate_vector.hpp>
#include <gtx/normal.hpp>
#include <PICASSO.hpp>

class PhysicsManager;

struct Rectangle{
    glm::vec3 size;
    glm::mat3 axis;
    glm::vec3 vertex[8];
    glm::vec3 max;
    glm::vec3 min;
    std::vector<PICASSO::Plane> faces;
};
struct Sphere{
    float radius;
};

class RigidBodyComponent : public IComponent{
    friend class PhysicsManager;
    public:

        RigidBodyComponent(glm::vec3* pos_ptr, const std::string& id, float rad, float m, int id_ent) : IComponent(id, id_ent){
            position = pos_ptr;
            Sphere sph{rad};
            s_bounding = sph;
            bounding = "Sphere";
            mass = m;
        }
        RigidBodyComponent(glm::vec3* pos_ptr, const std::string& id, float w, float h, float d, float m, float rotx, float roty, float rotz, int id_ent, bool recharge, bool checkpoint, bool electroshock) : IComponent(id, id_ent){
            position = pos_ptr;
            Rectangle rect{};
            rect.size = {w/2, d/2, h/2};
            //rotx *= M_PI / 180;
            //roty *= M_PI / 180;
            //rotz *= M_PI / 180;

            rotx = glm::radians(rotx);
            roty = glm::radians(roty);
            rotz = glm::radians(rotz);
            
            /* COMPLETE ARRAY OF 8 VERTEX */
            for(unsigned int i=0; i<8; i++){
                std::string binary = std::bitset<3>(i).to_string(); //to binary
                int aux[3];
                for(int j=2; j>-1; j--){
                    aux[j] = (int) binary[j] - 48;
                    if(aux[j] == 0)
                        aux[j] = -1;
                }
                rect.vertex[i] = {rect.size.x * aux[0], rect.size.y * aux[1], rect.size.z * aux[2]};
                rect.vertex[i] = glm::rotateY(rect.vertex[i],roty);
                rect.vertex[i] = glm::rotateX(rect.vertex[i],rotx);
                rect.vertex[i] = glm::rotateZ(rect.vertex[i],rotz);
                rect.vertex[i] = {position->x + rect.vertex[i].x, position->y + rect.vertex[i].y, position->z + rect.vertex[i].z};
            }
            /* GET PLANES OF THE 6 BOX'S FACES */

                int index[18] = {0,2,4,1,3,5,0,1,2,0,1,4,2,6,7,4,6,7};
                int cont =0;
                rect.faces.reserve(6);
                //rect.faces.reserve(6);
                for(unsigned int i=0; i<18; i=i+3){
                    rect.faces.emplace_back(PICASSO::Plane{});
                    rect.faces[cont].point = rect.vertex[index[i]];
                    rect.faces[cont].normal = glm::triangleNormal(rect.vertex[index[i]],rect.vertex[index[i+1]],rect.vertex[index[i+2]]);
                    rect.faces[cont].d = glm::dot(rect.faces[cont].normal, rect.faces[cont].point);
                    cont++;
                }


            /* GET AXIS */
            rect.axis = {{1,0,0},{0,1,0},{0,0,1}};
            for(unsigned int i=0; i<3; i++){
                rect.axis[i] = glm::rotateZ(rect.axis[i],rotz);
                rect.axis[i] = glm::rotateX(rect.axis[i],rotx);
                rect.axis[i] = glm::rotateY(rect.axis[i],roty);
            }

            /* GET THE MAX AND THE MIN IN EACH AXIS*/
            rect.max = rect.vertex[0];
            rect.min = rect.vertex[0];
            for (int i = 1; i < 8; i++) {
                rect.max.x = rect.max.x > rect.vertex[i].x ? rect.max.x : rect.vertex[i].x;
                rect.max.y = rect.max.y > rect.vertex[i].y ? rect.max.y : rect.vertex[i].y;
                rect.max.z = rect.max.z > rect.vertex[i].z ? rect.max.z : rect.vertex[i].z;

                rect.min.x = rect.min.x < rect.vertex[i].x ? rect.min.x : rect.vertex[i].x;
                rect.min.y = rect.min.y < rect.vertex[i].y ? rect.min.y : rect.vertex[i].y;
                rect.min.z = rect.min.z < rect.vertex[i].z ? rect.min.z : rect.vertex[i].z;
            }

            r_bounding = rect;
            bounding = "Rectangle";
            mass = m;
            nitro_zone = recharge;
            checkpoint_zone = checkpoint;
            electroshock_effect = electroshock;
        }
        
        ~RigidBodyComponent(){};
        float getRadius()                                 const { return s_bounding.radius; };
        float getMass()                                   const { return mass; };
        glm::vec3 getRectangleSize()                      const { return r_bounding.size; };
        glm::vec3 getRectangleMax()                       const { return r_bounding.max; };
        glm::vec3 getRectangleMin()                       const { return r_bounding.min; };
        glm::mat3 getRectangleRotation()                  const { return r_bounding.axis; };
        std::vector<PICASSO::Plane> getRectangleFaces()   const { return r_bounding.faces; };
        glm::vec3 getRectanglePlaneNormal(int i)          const { return r_bounding.faces[i].normal; };
        glm::vec3 getRectanglePlanePoint(int i)           const { return r_bounding.faces[i].point; };
        glm::vec3 getRectangleVertex(int i)               const { return r_bounding.vertex[i]; };
        std::string getBounding()                         const { return bounding; };
        glm::vec3* getPosition()                          const { return position; };
        bool getNitroZone()                               const { return nitro_zone; };
        bool getCheckpointZone()                          const { return checkpoint_zone; };
        bool getElectroshock()                            const { return electroshock_effect; };
        void setCollision(bool value)                           { m_collision_b = value;      };
        bool getCollision()                               const { return m_collision_b;      }; 

    private:
        float mass;
        Rectangle r_bounding;
        Sphere s_bounding;
        std::string bounding;
        glm::vec3* position = nullptr;
        bool nitro_zone = false;
        bool checkpoint_zone = false;
        bool electroshock_effect = false;
        bool m_collision_b = true;
};