#include <OrionHTTPRequester.hpp>
#include <OrionAPIHandler.hpp>

#include <RakSleep.h>
#include <iostream>
#include <sstream>

OrionHTTPRequester::OrionHTTPRequester(std::string c_hostURL_str)
{
    m_tcpInterface_ptr = RakNet::OP_NEW<RakNet::TCPInterface>(__FILE__, __LINE__);
	m_tcpInterface_ptr->Start(0,64);

	m_HTTPConnection_ptr = RakNet::OP_NEW<RakNet::HTTPConnection>(__FILE__, __LINE__);
	m_HTTPConnection_ptr->Init(m_tcpInterface_ptr, c_hostURL_str.c_str());
}

OrionHTTPRequester::~OrionHTTPRequester()
{
    RakNet::HTTPConnection::DestroyInstance(m_HTTPConnection_ptr);
	RakNet::TCPInterface::DestroyInstance(m_tcpInterface_ptr);
    std::cout<<"Disconnected from OrionHTTPRequester"<<std::endl;
}

std::string OrionHTTPRequester::GETRequest(std::string c_url_str)
{
	std::string body;
    bool received = false;
    m_HTTPConnection_ptr->Get(c_url_str.c_str());

    while(!received)
	{
 
		m_HTTPConnection_ptr->Update();
 

    	if (RakNet::Packet *packet = m_tcpInterface_ptr->Receive()) {
            m_HTTPConnection_ptr->ProcessTCPPacket(packet);
            m_tcpInterface_ptr->DeallocatePacket(packet);
    	}

    	RakNet::RakString l_data_str = m_HTTPConnection_ptr->Read();

		if(!l_data_str.IsEmpty())
		{
			//std::cout << l_data_str.C_String() << std::endl;
			received = true;
			
			std::string response(l_data_str.C_String());
			std::stringstream iss(response);
			while(iss.good())
			{
				getline(iss, body, '\n');
			}
		}
	}
	
	return body;
}

std::string OrionHTTPRequester::POSTRequest(std::string c_url_str, std::string c_bodyRequest_str, std::string c_appContentType_s)
{
	std::string body;
    bool received = false;
    m_HTTPConnection_ptr->Post(c_url_str.c_str(), c_bodyRequest_str.c_str(), c_appContentType_s.c_str());

    while(!received)
	{
		m_HTTPConnection_ptr->Update();

    	if (RakNet::Packet *packet = m_tcpInterface_ptr->Receive()) {
            m_HTTPConnection_ptr->ProcessTCPPacket(packet);
            m_tcpInterface_ptr->DeallocatePacket(packet);
    	}

    	RakNet::RakString l_data_str = m_HTTPConnection_ptr->Read();

		if(!l_data_str.IsEmpty())
		{
			//std::cout << l_data_str.C_String() << std::endl;
			received = true;

			std::string response(l_data_str.C_String());
			std::stringstream iss(response);
			while(iss.good())
			{
				getline(iss, body, '\n');
			}
		}
	}

	return body;
}

bool OrionHTTPRequester::Login(std::string c_username, std::string c_password)
{
	std::string request = "{\"login\": \" " + c_username + " \", \"password\": \"" + c_password + "\"}";
	std::string result = this->POSTRequest("/users/APIRouter/login", request, "application/json");
	
	return OrionAPIResultsHandler::CheckLoginResponse(result.c_str());
}

//--------REGISTER ROOM FUNCTIONS-------------//
std::future<bool> OrionHTTPRequester::RegisterRoom(int c_roomID, std::string c_roomName, std::string c_roomPassword)
{
	std::promise<bool> l_promise;
	std::future<bool>  l_promiseReturn = l_promise.get_future();
	std::thread(&OrionHTTPRequester::RegisterRoomFunc, this, c_roomID, c_roomName, std::move(l_promise), c_roomPassword).detach();

	return l_promiseReturn;
}

void OrionHTTPRequester::RegisterRoomFunc(int c_roomID, std::string c_roomName, std::promise<bool> c_promise, std::string c_roomPassword)
{
	std::string request = "{\"roomID\": \" " + std::to_string(c_roomID) + " \", \"roomname\": \"" + c_roomName + "\"}";
	std::string result = this->POSTRequest("/rooms/registerRoom", request, "application/json");

	bool finalResult = OrionAPIResultsHandler::CheckRoomRegister(result.c_str());

	c_promise.set_value(finalResult);
}
//--------------------------------------------//
std::future<std::vector<RoomData>> OrionHTTPRequester::GetAllRooms()
{
	std::promise<std::vector<RoomData>> l_promise;
	std::future<std::vector<RoomData>>  l_promiseReturn = l_promise.get_future();
	std::thread(&OrionHTTPRequester::GetAllRoomsFunc, this, std::move(l_promise)).detach();
	
	return l_promiseReturn;
}

void OrionHTTPRequester::GetAllRoomsFunc(std::promise<std::vector<RoomData>> c_promise)
{
	std::string result = this->GETRequest("/rooms");
	std::vector<RoomData> rooms = OrionAPIResultsHandler::SetRoomResponse(result.c_str());
	c_promise.set_value(rooms);
}

void OrionHTTPRequester::DeleteRoom(int c_roomID)
{
	std::string request = "{\"roomID\": \" " + std::to_string(c_roomID) + "\" }"; 
	std::string result = this->POSTRequest("/rooms/deleteRoom", request, "application/json");

	if(!OrionAPIResultsHandler::CheckRoomDeletion(result.c_str()))
	{
		std::cout<<"[ERROR] Room "<<c_roomID<<" could not be deleted"<<std::endl;
	}
}
