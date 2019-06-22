#include <ResourceManager.hpp>
#include <TResourceMesh.hpp>
#include <TResourceShader.hpp>
#include <TResourceTexture.hpp>
#include <TResourceCubeMap.hpp>
#include <TResourceFont.hpp>

/**
 * @brief Destroy the Resource Manager:: Resource Manager object
 * 
 */
ResourceManager::~ResourceManager()
{
   //For all the elements into the std::map, it deletes the pointer in memory
   for(auto& l_resource : m_ResourceMap_map)
   {
       delete l_resource.second;
       l_resource.second = nullptr;
   }
   std::map<std::string, TResource*> empty;
   //The map is cleared to delete all entries, which have now only a std::string as first value
   m_ResourceMap_map.swap(empty);
   //std::cout<<"[SUCCESS] Resource Manager erased"<<std::endl;
}

/**
 * @brief 
 * 
 * @param c_resourceDocument_str 
 * @return const TResource* 
 */
const TResource* ResourceManager::GetMesh(const std::string& c_resourceDocument_str)
{
    //We check first if the mesh is already loaded into our std::map
    if(!(m_ResourceMap_map.count(c_resourceDocument_str))) 
    {
        //If it doesn't exist in memory, we load it from disk
        //std::cout<<c_resourceDocument_str<<" does not exist in memory, charging it..."<<std::endl;

        LoadMesh(c_resourceDocument_str);
    
        //std::cout<<"[SUCCESS] "<<c_resourceDocument_str<<" model loaded correctly"<<std::endl;
    }

    /*In this point of code, we now are ensured that the entry exists in the std::map, so we make a direct
    access into it*/
    return m_ResourceMap_map[c_resourceDocument_str]; 
}

/**
 * @brief 
 * 
 * @param c_resourceDocument_str 
 * @return const TResource* 
 */
const TResource* ResourceManager::GetTexture(const std::string& c_resourceDocument_str)
{
    //We check first if the texture is already loaded into our std::map
    if(!(m_ResourceMap_map.count(c_resourceDocument_str)))
    {
        //If it doesn't exist in memory, we load it from disk
        //std::cout<<c_resourceDocument_str<<" does not exist in memory, charging it..."<<std::endl;

        LoadTexture(c_resourceDocument_str);
    
        //std::cout<<"[SUCCESS] "<<c_resourceDocument_str<<" texture loaded correctly"<<std::endl;
    }

    return m_ResourceMap_map[c_resourceDocument_str];
}

/**
 * @brief 
 * 
 * @param c_resourceDocument_str 
 * @return const TResource* 
 */
const TResource* ResourceManager::GetShader(const std::string& c_resourceDocument_str)
{
    if(!(m_ResourceMap_map.count(c_resourceDocument_str)))
    {
        //If it doesn't exist in memory, we load it from disk
        //std::cout<<c_resourceDocument_str<<" does not exist in memory, charging it..."<<std::endl;

        LoadShader(c_resourceDocument_str);
    
        //std::cout<<"[SUCCESS] "<<c_resourceDocument_str<<" shader loaded correctly"<<std::endl;
    }

    return m_ResourceMap_map[c_resourceDocument_str];
}

const TResource* ResourceManager::GetFont(const std::string& c_resourceDocument_str)
{
    if(!(m_ResourceMap_map.count(c_resourceDocument_str)))
    {
        //If it doesn't exist in memory, we load it from disk
        //std::cout<<c_resourceDocument_str<<" does not exist in memory, charging it..."<<std::endl;

        LoadShader(c_resourceDocument_str);
    
        //std::cout<<"[SUCCESS] "<<c_resourceDocument_str<<" shader loaded correctly"<<std::endl;
    }

    return m_ResourceMap_map[c_resourceDocument_str];
}

/**
 * @brief 
 * 
 * @param c_resourceDocument_str 
 */
void ResourceManager::LoadMesh(const std::string& c_resourceDocument_str)
{
    TResource* l_newMesh = new TResourceMesh();             //Create a new pointer to the resource
    l_newMesh->LoadResource(c_resourceDocument_str);        //Init it
    m_ResourceMap_map[c_resourceDocument_str] = l_newMesh;  //Store it into the std::map
}

/**
 * @brief 
 * 
 * @param c_resourceDocument_str 
 */
void ResourceManager::LoadTexture(const std::string& c_resourceDocument_str)
{
    TResource* l_newTexture = new TResourceTexture();
    l_newTexture->LoadResource(c_resourceDocument_str);
    m_ResourceMap_map[c_resourceDocument_str] = l_newTexture;
}

/**
 * @brief 
 * 
 * @param c_resourceShader_str 
 */
void ResourceManager::LoadShader(const std::string& c_resourceShader_str)
{
    TResource* l_newShader = new TResourceShader();
    l_newShader->LoadResource(c_resourceShader_str);
    m_ResourceMap_map[c_resourceShader_str] = l_newShader;
}

/**
 * @brief 
 * 
 * @param c_resourceDocument_str 
 */
void ResourceManager::LoadFont(const std::string& c_resourceDocument_str)
{
    TResource* l_newFont = new TResourceFont();
    l_newFont->LoadResource(c_resourceDocument_str);
    m_ResourceMap_map[c_resourceDocument_str] = l_newFont;
}

/**
 * @brief 
 * 
 * @param l_facesDir_vs 
 */
TResourceCubeMap* ResourceManager::LoadCubeMap(const std::vector<std::string>& l_facesDir_vs)
{
    TResourceCubeMap* l_cubeMap_ptr = new TResourceCubeMap();
    l_cubeMap_ptr->LoadCubeMap(l_facesDir_vs);
    m_ResourceMap_map["skybox"] = reinterpret_cast<TResource*>(l_cubeMap_ptr);
    return l_cubeMap_ptr;
}