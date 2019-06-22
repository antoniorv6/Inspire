#pragma once
#include <string>
#include <CommonData.hpp>


class IComponent{
    public:
        IComponent(const std::string& id, int ent_id){
            ComponentId = id;
            EntityId = ent_id;
        };
        ~IComponent(){

        };
        std::string GetComponent() const { return ComponentId; };
        int GetEntityId() const { return EntityId; };
    private:
        std::string ComponentId = "";
        int         EntityId = 0;
};