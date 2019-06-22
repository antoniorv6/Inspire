#pragma once
#include <IComponent.h>

#include <string>

class LineComponent: public IComponent {
    public:
        LineComponent(const std::string& id, int l_entityId_s) : IComponent(id, l_entityId_s){}
        ~LineComponent(){};
    private:
};