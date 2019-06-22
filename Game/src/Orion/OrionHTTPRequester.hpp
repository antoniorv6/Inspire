#include <string>
#include <TCPInterface.h>
#include <HTTPConnection.h>
#include <thread>
#include <future>
#include <vector>

struct RoomData;
class OrionHTTPRequester
{
    public: 
    
    OrionHTTPRequester(std::string c_hostURL_str);
    ~OrionHTTPRequester();

    std::string GETRequest(std::string c_url_str);
    std::string POSTRequest(std::string c_url_str, std::string c_bodyRequest_str, std::string c_appContentType_s);

    bool Login(std::string c_username, std::string c_password);
    
    //--------REGISTER ROOM FUNCTIONS-------------//
    std::future<bool> RegisterRoom(int c_roomID, std::string c_roomName, std::string c_roomPassword);
    void RegisterRoomFunc(int c_roomID, std::string c_roomName, std::promise<bool> c_promise, std::string c_roomPassword);
    //--------------------------------------------//

    //--------DEMAND ROOM FUNCTIONS--------------//
    std::future<std::vector<RoomData>> GetAllRooms();
    void GetAllRoomsFunc(std::promise<std::vector<RoomData>> c_promise);
    //-------------------------------------------//

    //-------DELETE ROOMS FUNCTION--------------//
    void DeleteRoom(int c_roomID);
    //-----------------------------------------//

    private: 
        RakNet::TCPInterface*   m_tcpInterface_ptr;
        RakNet::HTTPConnection* m_HTTPConnection_ptr;

};