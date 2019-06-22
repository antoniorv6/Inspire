#include <Octree.h>
#include <RenderManager.h>

Octree::Octree(int current_level, Box bound)
{
    bounds = bound;
    level = current_level;
    
}

Octree::~Octree()
{
    for(auto node : nodes)
    {
        delete node;
        node = nullptr;
    }
}

void Octree::insert(RigidBodyComponent* component)
{
    
    all_components.push_back(component);

    if(nodes.size() != 0){
        int index = getIndex(component);
        if(index != -1){
            nodes[index]->insert(component);

            return;
        }
    }

    components.push_back(component);

    if(((int)components.size() > MAX_ENTITIES) && (level < MAX_LEVELS)){
        if(nodes.size() ==  0){
            Split();
        }

        unsigned int i=0;
        while (i < components.size()) {
            int index = getIndex(components[i]);
            if(index != -1) {
                nodes[index]->insert(components[i]);
                components.erase(components.begin() + i);
            }
            else{
                i++;
            }
        }
    } 
}

void Octree::Split()
{
    float suSize = bounds.width / 2;
    float x = bounds.corner.x;
    float y = bounds.corner.y;
    float z = bounds.corner.z;

    float xx = x+suSize;
    float yy = y+suSize;
    float zz = z+suSize;

    nodes.push_back(new Octree(level+1, Box{suSize, glm::vec3(x,y,z)})); 
    nodes.push_back(new Octree(level+1, Box{suSize, glm::vec3(xx,y,z)}));
    nodes.push_back(new Octree(level+1, Box{suSize, glm::vec3(x,yy,z)}));
    nodes.push_back(new Octree(level+1, Box{suSize, glm::vec3(xx,yy,z)}));
    nodes.push_back(new Octree(level+1, Box{suSize, glm::vec3(x,y,zz)}));
    nodes.push_back(new Octree(level+1, Box{suSize, glm::vec3(xx,y,zz)}));
    nodes.push_back(new Octree(level+1, Box{suSize, glm::vec3(x,yy,zz)}));
    nodes.push_back(new Octree(level+1, Box{suSize, glm::vec3(xx,yy,zz)}));

}

int Octree::getIndex(RigidBodyComponent* component)
{
    int index = -1;
    float mid[] = {0,0,0};

    mid[0] = bounds.corner.x + (bounds.width / 2);
    mid[1] = bounds.corner.y + (bounds.width / 2);
    mid[2] = bounds.corner.z + (bounds.width / 2);

    if(component->getBounding()=="Rectangle"){
        
        int bin[] = {-1,-1,-1};
        glm::vec3 max = component->getRectangleMax();
        glm::vec3 min = component->getRectangleMin();

        for(unsigned int i=0; i<3 ; i++){
            if(min[i] > mid[i])
                bin[i] = 1;
            else if(min[i] < mid[i] && max[i] < mid[i])
                bin[i] = 0;
            else 
                 return index;
        }

        index = bin[0] + 2*bin[1] + 4*bin[2];


    }else{ //Bounding == Sphere
        
        int bin[] = {-1,-1,-1};
        glm::vec3 pos = (*component->getPosition());
        float radius   = component->getRadius();

        for(unsigned int i=0; i<3 ; i++){
            if(pos[i] - radius > mid[i]){
                bin[i]=1;
            }
            else if(pos[i] - radius < mid[i] && pos[i] + radius < mid[i]){
                bin[i]=0;
            }
            else
                return index;
        }

        index = bin[0] + 2*bin[1] + 4*bin[2];

    }
    return index;
}



// std::vector<RigidBodyComponent*> Octree::getCloseObjects(std::vector<RigidBodyComponent*> objects, RigidBodyComponent* component)
// {
//    int index = getIndex(component);

//    if (index != -1 && nodes.size()!=0){
//      objects = nodes[index]->getCloseObjects(objects, component);
//    }

//    else if (index == -1 && nodes.size()!=0){
//        objects.insert(objects.end(), all_components.begin(), all_components.end());
//        return objects;
//    }
    
//    objects.insert(objects.end(), components.begin(), components.end());

//    return objects;
// }

void Octree::getCloseObjects(std::vector<RigidBodyComponent*>* objects, RigidBodyComponent* component)
{
   int index = getIndex(component);

   if (index != -1 && nodes.size()!=0){
     nodes[index]->getCloseObjects(objects, component);
   }

   else if (index == -1 && nodes.size()!=0){
       (*objects).insert((*objects).end(), all_components.begin(), all_components.end());
       return;
   }
    
   (*objects).insert((*objects).end(), components.begin(), components.end());

   return;
}

/**
 * @brief: Returns a Vec3 vector with the closest objects.
 *         This method is for AI functionalities.
 * 
 * @param objects 
 * @param component 
 * @return std::vector<glm::vec3> 
 */
std::vector<glm::vec3> Octree::GetCloseObjectsV3(RigidBodyComponent* component) 
{
    /** Obstains the closest RigidBodyComponents. */
    std::vector<RigidBodyComponent*> l_closestObstacles_v3;
    
    getCloseObjects(&l_closestObstacles_v3, component);
    /** New glm::vec3 vector. */
    std::vector<glm::vec3> l_obstaclesPositions_v3;
    /** Reverse memory for this vector. Optimization purposes. */
    l_obstaclesPositions_v3.reserve(l_closestObstacles_v3.size());
    /** Map the RigidBodyComponent's positions. */
    for(size_t i = 0; i < l_closestObstacles_v3.size(); i++)
    {
        l_obstaclesPositions_v3.emplace_back(*l_closestObstacles_v3[i]->getPosition());
    }
    
    return l_obstaclesPositions_v3;
}

/**
 * @brief 
 * 
 * @param component 
 * @return std::vector<glm::vec3> 
 */
std::vector<glm::vec3> Octree::GetCloseNitroZones(RigidBodyComponent* component)
{
    /** Obstains the closest RigidBodyComponents. */
    std::vector<RigidBodyComponent*> l_closestObstacles_v3;
    
    getCloseObjects(&l_closestObstacles_v3, component);
    /** New glm::vec3 vector. */
    std::vector<glm::vec3> l_obstaclesPositions_v3;
    /** Reverse memory for this vector. Optimization purposes. */
    l_obstaclesPositions_v3.reserve(5);
    /** Map the RigidBodyComponent's positions. */
    for(size_t i = 0; i < l_closestObstacles_v3.size(); i++)
    {
        if(l_closestObstacles_v3[i]->getNitroZone())
        {
            l_obstaclesPositions_v3.emplace_back(*l_closestObstacles_v3[i]->getPosition());
        }
    }
    
    return l_obstaclesPositions_v3;
}


void Octree::clear()
{
   components.clear();
 
   for (unsigned int i = 0; i < nodes.size(); i++) {
     if (nodes.size() != 0) {
       nodes[i]->clear();
       delete nodes[i];
       nodes[i] = nullptr;
     }
   }
}