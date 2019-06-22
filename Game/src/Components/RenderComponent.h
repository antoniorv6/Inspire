#pragma once

#include <string>
#include <IComponent.h>

class RenderComponent : public IComponent {
    public:
        RenderComponent(const std::string& id, const std::string& type, const std::string& file, int ent_id) : IComponent(id, ent_id)
        {
            Folder = "";
            File = file;
            Type = type;
        }
        ~RenderComponent(){};
        
        void setNode(unsigned int node){ Nodeid = node; };
        unsigned int getNode()    const { return Nodeid; };
        std::string getType()     const { return Type; };
        std::string getMeshPath() const {return Folder+File;}

    private:
        unsigned int Nodeid;
        std::string File;
        std::string Folder;
        std::string Type;
};