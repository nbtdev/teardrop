/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#if !defined(NETWORK_INCLUDED)
#define NETWORK_INCLUDED

#if defined(COS_TARGET_FINAL)
#define MASTER_SERVER_LOCATOR_HOST ("http://www.clashofsteel.net")
#else
// define your local test master server here
//#define MASTER_SERVER_LOCATOR_HOST ("http://192.168.1.67")
#define MASTER_SERVER_LOCATOR_HOST ("http://dayark.com")
#endif

#define MASTER_SERVER_LOCATOR_PATH ("/servers.php")

/*
	These will change whenever a protocol change breaks existing
	communications, and servers should reject clients that do not
	report the proper protocol version that the server is using.
*/
enum ProtocolVersion
{
	PROTOCOL_MASTER_SERVER	= 0x00000001,
	PROTOCOL_GAME_SERVER	= 0x00000001,
};

struct Packet;

namespace CoS
{
	class Allocator;

	namespace Net
	{
		/*
			common network message IDs; these are sub-IDs of the network 
			interface's control messages (which always show up at the front
			of all message traffic)
		*/
		enum MessageID
		{
			MSGID_NULL = 0,

			/*
				Used by game servers to advertise their existence to the master
				server.
			*/
			MSGID_ADVERTISE_SERVER,

			/*
				Used by game servers to stop advertising their existence to the master
				server.
			*/
			MSGID_UNADVERTISE_SERVER,

			/*
				Used by game clients to retrieve the list of available game servers from
				the master server.
			*/
			MSGID_REQUEST_SERVER_LIST,

			/*
				Used by game clients to ask a server directly for its details (useful for
				LAN server browsing). Server will respond with a standard server-info packet.
			*/
			MSGID_INTERROGATE_SERVER,

			/*
				Used by game clients to ask a server about friend/foe information on a given 
				target set.
			*/
			MSGID_QUERY_IFF,

			/*
				Used by game server to respond with friend/foe information on a given 
				target set.
			*/
			MSGID_RESPONSE_IFF,

			/*
				Used by game servers to update their current state with the master server.
				Format of the data is:

				Map name (RakNet::RakString)
			*/
			MSGID_UPDATE_SERVER_INFO,

			/*
				Used by master server when sending game server list to game clients. List
				comes in the following format:

				Server GUID (RakNet::RakNetGUID)
				Server Address (RakNet::SystemAddress)
				Server ping (unsigned int)
				Server name (RakNet::RakString)
				Current map name (RakNet::RakString)

				This message is sent only in response to a MSGID_REQUEST_SERVER_LIST message;
				individual server state updates are broadcast on a different message (and 
				possibly down a different channel)
			*/
			MSGID_RESPOND_SERVER_LIST,

			/********************************************************************
				CLIENT/GAME-SERVER PROTOCOL
			********************************************************************/

			/*
				Client --> Server: Client wishes to join the server
				Server --> Client(s): A client has successfully joined the server
			*/
			MSGID_PLAYER_JOIN_SERVER,

			/*
				Client --> Server: Client wishes to update the player's record on the server
				Server --> Client(s): Server broadcasts new player state to all connected clients
			*/
			MSGID_PLAYER_UPDATE_STATE,

			/*
				Client --> Server: Client wishes to update the player's variant data on the server
				Server --> Client(s): Server broadcasts new variant data to all connected clients
			*/
			MSGID_PLAYER_UPDATE_VARIANT,

			/*
				Client --> Server: Client is leaving the server
				Server --> Client(s): A client has successfully left the server
			*/
			MSGID_PLAYER_LEAVE_SERVER,

			/*
				Client --> Server: Client wishes to enter the game
				Server --> Client(s): A client has entered the current game
			*/
			MSGID_PLAYER_JOIN_GAME,

			/*
				Client --> Server: Client wishes to enter the game
				Server --> Client(s): A client has entered the current game
			*/
			MSGID_PLAYER_LEAVE_GAME,

			/*
				Server --> Client(s): A game has been launched
			*/
			MSGID_GAME_STARTED,

			/*
				Server --> Client(s): A game has ended
			*/
			MSGID_GAME_ENDED,

			//***************************************************************
			// Chat server messages
			//***************************************************************

			/*
				Message contains chat sent by a client, with optional 
				distribution control.
			*/
			MSGID_CHAT_MESSAGE,

			//***************************************************************
			// ONLY PROXYMESSAGE DERIVED MESSAGES CAN GO BEYOND HERE
			//***************************************************************
			MSGID_PROXY_MESSAGE_FLAG,

			/*
				Server --> Client(s): Authoritative position/orientation sync
			*/
			MSGID_PLAYER_POSITION_SYNC,

			/*
				Client --> Server: Client action (movement, etc)
			*/
			MSGID_PLAYER_COMMAND,

			// don't put anything after this identifier
			MSGID_NUM_MESSAGES
		};

		// port numbers for the various servers
		enum ServerPort
		{
			/*
				This is the port on which the master server listens
				for game server advertisements; game servers should
				connect to the master server on this port.
			*/
			PORT_MASTER_SERVER_ADVERTISE_LISTENER		= 54001,

			/*
				This is the port on which the game server listens for
				incoming client connections; game clients should connect
				to the game servers on this port.
			*/
			PORT_GAME_SERVER_CLIENT_LISTENER			= 55001,
		};

		// network server client limits
		enum ClientLimits
		{
			/*
				The master server needs to be able to deal with
				a lot; start with 1000 as "reasonable"
			*/
			CLIENT_LIMITS_MASTER_SERVER = 1000,

			/*
				Game server we'll play by ear as we see how many each
				can handle
			*/
			CLIENT_LIMITS_GAME_SERVER = 48,
		};

		/*
			Core networking interface class
		*/
		class Message;
		class Peer;

		class Network
		{
			static Allocator* s_pAllocator;

		public:
			// set/get the current allocator for this system
			static void setAllocator(Allocator* pAlloc);
			static Allocator* getAllocator();
			static const char* getMessageString(unsigned int id);

			typedef Message* (*MessageCreateFn)(const Packet*);
			static MessageCreateFn s_factory[];

			static Message* createMessage(const Packet*);

			static Peer* createPeer();
			static void destroyPeer(Peer*);

			// hopefully this is just a temp hack to allow
			// static init to happen for the messages
			static void initialize();

			static void* malloc(size_t sz);
			static void* realloc(void* mem, size_t sz);
			static void free(void* pMem);
		};

		// data packet sent with disconnected-ping (ID_PONG) responses
		// from a server to an interrogating client
		struct DiscoveryData
		{
			unsigned int mapId;
			unsigned int numPlayers;
			unsigned int bInProgress; // 0 or 1
			char mapName[96];
			char serverName[64];

			DiscoveryData();
		};
	}
}
//
//// handy defines for use in message definitions
//#define COS_NETMESSAGE(c) \
//	static CoS::Net::Message* __create(const Packet*); \
//	static const size_t ID; \
//	size_t getId() { return ID; } \
//	c() : Message() {} \
//	static struct MessageInitializer \
//	{ \
//		MessageInitializer(size_t id) \
//		{ \
//			c _c; \
//			CoS::Net::Network::s_factory[id] = __create; \
//		} \
//	} s_messageInit; \


//#define COS_NETMESSAGE_IMPL(c, id) \
//	const size_t c::ID = id; \
//	CoS::Net::Message* CoS::Net::c::__create(const Packet* packet) { return COS_NEW c(*packet); } \
//	CoS::Net::c::MessageInitializer CoS::Net::c::s_messageInit(id); \

// handy defines for use in message definitions
#define COS_NETMESSAGE(c, id) \
	static CoS::Net::Message* __create(const Packet*); \
	static const size_t ID = id; \
	size_t getId() { return ID; } \
	c() : Message() {} \
	static struct MessageInitializer \
	{ \
		MessageInitializer() \
		{ \
			c _c; \
			CoS::Net::Network::s_factory[id] = __create; \
		} \
	} s_messageInit; \

#define COS_NETMESSAGE_IMPL(c) \
	CoS::Net::Message* CoS::Net::c::__create(const Packet* packet) { return COS_NEW c(*packet); } \
	CoS::Net::c::MessageInitializer CoS::Net::c::s_messageInit; \

#define COS_PROXYMESSAGE(c, id) \
	static CoS::Net::Message* __create(const Packet*); \
	static const size_t ID = id; \
	size_t getId() { return ID; } \
	c() : ProxyMessage() {} \
	static struct MessageInitializer \
	{ \
		MessageInitializer() \
		{ \
			c _c; \
			CoS::Net::Network::s_factory[id] = __create; \
		} \
	} s_messageInit; \

#define COS_PROXYMESSAGE_IMPL(c) \
	CoS::Net::Message* CoS::Net::c::__create(const Packet* packet) { return COS_NEW c(*packet); } \
	CoS::Net::c::MessageInitializer CoS::Net::c::s_messageInit; \


#endif // NETWORK_INCLUDED
