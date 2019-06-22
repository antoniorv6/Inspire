#pragma once

#include <string>
#include <IComponent.h>

class InputComponent : public IComponent {
    public:
        InputComponent(const std::string& id, int l_entityId_s) : IComponent(id, l_entityId_s){}
        ~InputComponent(){};

};