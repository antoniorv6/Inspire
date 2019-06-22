struct ServerCallBack
{
    RakNet::MessageID m_id;
    void (OrionServer::*CallbackFunc)();
};

ServerCallBack m_callbacks [] = {
		{ID_JOIN_ROOM, 		&OrionServer::ProccessRoomJoin},
		{ID_PLAYER_INPUT, 	&OrionServer::UpdatePlayerInputs},
		{ID_CLIENTREADY, 	&OrionServer::ProccesPlayerReadyStatus},
		{	0,		0	}
	};