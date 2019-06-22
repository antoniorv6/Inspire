#pragma once

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <glm.hpp>
#include <RigidBodyComponent.h>

class Octree{
    public:
        struct Box{
            float width;
            glm::vec3 corner = {0, 0, 0};
        };

        Octree(int current_level, Box bound);
        ~Octree();

        void Split();
        void insert(RigidBodyComponent* component);
        int getIndex(RigidBodyComponent* component);
        Octree::Box getBounds(){ return bounds; };
        //std::vector<RigidBodyComponent*> getCloseObjects(std::vector<RigidBodyComponent*> objects, RigidBodyComponent* component);
        void getCloseObjects(std::vector<RigidBodyComponent*>* objects, RigidBodyComponent* component);
        std::vector<glm::vec3> GetCloseObjectsV3(RigidBodyComponent* component);
        std::vector<glm::vec3> GetCloseNitroZones(RigidBodyComponent* component);
        void clear();
        std::vector<Octree*> getNodes(){ return nodes; };
        std::vector<RigidBodyComponent*>* getAllCompPTR(){ return &all_components; };
        std::vector<RigidBodyComponent*>* getCompPTR(){ return &components; };

    private:
        int draw = false;
        const int MAX_ENTITIES = 2;
        const int MAX_LEVELS = 20;
        int level;  //Current level in the Octree
        std::vector<RigidBodyComponent*> components;
        std::vector<RigidBodyComponent*> all_components;
        Box bounds; //3D space that the node occupies
        std::vector<Octree*> nodes; //eight subnodes (childs)
};
