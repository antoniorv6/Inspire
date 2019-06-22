#include<MessageIdentifiers.h>

enum GameMessages
{
	ID_CREATE_PLAYER 		  = ID_USER_PACKET_ENUM + 1 , //Instruction to create a player entity
	ID_CREATE_EXTERMAL_ENTITY = ID_USER_PACKET_ENUM + 2 , //Instruction to create a external AI
	ID_PLAYER_INPUT			  = ID_USER_PACKET_ENUM + 3 , //Instruction to receive inputs from a player
	ID_UPDATE_PLAYER		  = ID_USER_PACKET_ENUM + 4 , //Instruction to update the main player's data
	ID_UPDATE_ENTITY		  = ID_USER_PACKET_ENUM + 5 , //Instruction to update an external entity data
	ID_LOAD_MAP				  = ID_USER_PACKET_ENUM + 6 , //Instruction to indicate a client to load a map
	ID_JOIN_ROOM			  = ID_USER_PACKET_ENUM + 7 , //Instruction to join a client into a room
	ID_CLIENTREADY            = ID_USER_PACKET_ENUM + 8 , //Instruction to sed a Ready status into a room
	ID_STARTGAME			  = ID_USER_PACKET_ENUM + 9 , //Instruction to order the clients to change their state
	ID_CREATEROOM			  = ID_USER_PACKET_ENUM + 10, //Instruction to create a custom room
	ID_ROOMCREATED			  = ID_USER_PACKET_ENUM + 11, //Instruction to confirm a room creation
	ID_LOADPOWERUPS			  = ID_USER_PACKET_ENUM + 12, //Instruction to load power ups
	ID_SYNC				      = ID_USER_PACKET_ENUM + 13,
	ID_SYNCENTITY             = ID_USER_PACKET_ENUM + 14,
	ID_CHARACTERSCREATED	  = ID_USER_PACKET_ENUM + 15,
	ID_DISCONNECTION	      = ID_USER_PACKET_ENUM + 16,
	ID_GETPOWERUP             = ID_USER_PACKET_ENUM + 17
};
