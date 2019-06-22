#include <document.h>
#include <prettywriter.h>
#include <vector>
#include <OrionHTTPStructs.hpp>
#include <iostream>

namespace OrionAPIResultsHandler
{
    rapidjson::Document JSONParse(const char* c_rawString_str)
    {
        std::cout<<c_rawString_str<<std::endl;
        rapidjson::Document resultDocument;
        
        if(resultDocument.Parse(c_rawString_str).HasParseError())
        {
            std::cout <<"Error parsing"<<std::endl;
        }

        return resultDocument;
    }

    bool CheckLoginResponse(const char* c_response)
    {
        rapidjson::Document l_documentToProccess = JSONParse(c_response);
        
        if(l_documentToProccess.HasMember("logged"))
        {
            //std::cout<<"It has the member"<<std::endl;
            return l_documentToProccess["logged"].GetBool();
        }
        //std::cout<<"It doesn't have the member"<<std::endl;
        return false;
    }

    bool CheckRoomRegister(const char* c_response)
    {
        rapidjson::Document l_documentToProccess = JSONParse(c_response);
        if(l_documentToProccess.HasMember("result"))
        {
            return l_documentToProccess["result"].GetBool();
        }

        return false;
    }

    bool CheckRoomDeletion(const char* c_response)
    {
        rapidjson::Document l_documentToProccess = JSONParse(c_response);
        if(l_documentToProccess.HasMember("deleted"))
        {
            return l_documentToProccess["deleted"].GetBool();
        }
        
        return false;
    }

    std::vector<RoomData> SetRoomResponse(const char* c_response)
    {
        std::vector<RoomData> l_resultVector;
        l_resultVector.reserve(20);
        rapidjson::Document l_documentToProccess = JSONParse(c_response);
        if(l_documentToProccess.HasMember("result"))
        {
            for(const auto& value: l_documentToProccess["result"].GetArray())
            {
               RoomData l_data;
               l_data.room_name = value["roomname"].GetString();
               l_data.roomID    = value["roomID"].GetInt();
               l_resultVector.emplace_back(l_data);
            }
        }
        return l_resultVector;
    }
}