#include <glm.hpp>
#include <string>
#include <vector>
#include <PICASSO.hpp>

#pragma pack(push, 1)
struct PlayerData
{
    unsigned char typeId; // Your type here
    unsigned int entityID;
    glm::vec3 m_position;// Your data here
    unsigned int howmanyplayers;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct InputDataPacket
{
    unsigned char typeId; // Your type here
    int roomID;
    unsigned int entityID;
    bool W;
    bool A;
    bool S;
    bool D;
    bool I;
    bool K;
    bool SPACE;
    bool J;
    bool L;
    glm::vec3   Position;
    glm::vec3   Rotation;
    glm::vec3   Velocity;
    float       accUP;
    float       accFWRD;

};
#pragma pack(pop)
#pragma pack(push, 1)
struct EntityUpdatePacket
{
    unsigned char    typeId; // Your type here
    unsigned int   entityID;
    unsigned int   roomID;
    glm::vec3   Position;
    glm::vec3   Rotation;
    glm::vec3   Velocity;
    float       accUP;
    float       accFWRD;
    int         checkpoint;
    int         next_checkpoint;
    int         lap;
};

#pragma pack(pop)
#pragma pack(push, 1)
struct MapLoadInformation
{
    unsigned char    typeId; // Your type here
    unsigned int     mapId;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct JoinRoomInformation
{
    unsigned char    typeId; // Your type here
    unsigned int     roomID;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct DisconnectionPacket
{
    unsigned char    typeId; // Your type here
    unsigned int     roomID;
    unsigned int   entityID;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SendPUPacket
{
    unsigned char    typeId; // Your type here
    unsigned int     roomID;
    unsigned int   entityID;
    unsigned int    powerUp1;
    unsigned int    powerUp2;
};
#pragma pack(pop)

struct ClientID
{
    unsigned int serverID;
    RakNet::SystemAddress sys_address;
    bool ready;
};