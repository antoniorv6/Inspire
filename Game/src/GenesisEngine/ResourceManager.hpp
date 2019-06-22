
#include <map>
class TResource;
class TResourceMesh;
class TResourceTexture;
class TResourceCubeMap;
#include<vector>

class ResourceManager 
{
    public:
        static ResourceManager* getInstance()
            {
                static ResourceManager instance;
                return &instance;
            }
        ~ResourceManager();

        const TResource* GetMesh(const std::string& c_resourceDocument_str);
        const TResource* GetTexture(const std::string& c_resourceDocument_str);
        const TResource* GetShader(const std::string& c_resourceDocument_str);
        const TResource* GetFont(const std::string& c_resourceDocument_str);

        void LoadMesh(const std::string& c_resourceDocument_str);
        void LoadTexture(const std::string& c_resourceDocument_str);
        void LoadShader(const std::string& c_resourceDoucment_str);
        void LoadFont(const std::string& c_resourceDocument_str);
        
        TResourceCubeMap* LoadCubeMap(const std::vector<std::string>&l_facesDir_vs);

    private:
        std::map<std::string, TResource*> m_ResourceMap_map;
};