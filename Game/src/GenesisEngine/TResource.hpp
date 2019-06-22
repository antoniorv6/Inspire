#pragma once
#include <string>

class TResource
{
public:
    virtual ~TResource(){};
    virtual void LoadResource(const std::string& c_resourceDocument_str) = 0;
private:
};