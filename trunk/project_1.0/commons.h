
// constants used as addresses in internal node communication
static const unsigned int NODE_MANAGER = 0;
static const unsigned int DATA_MANAGER = 1;
static const unsigned int CONNECTION_MANAGER = 2;

// constants used as types of messages between peers and between submodules
// handshake message between peers
static const unsigned int HANDSHAKE_MSG = 0;
// message to data manager that connecitns has bee nestablished and data processing may be started
static const unsigned int CONNECTIONS_ESTABLISHED_MSG = 1;
// response for a handshake
static const unsigned int HANDSHAKE_RESPONSE = 2;

static const unsigned int NODE_1 = 1;
static const unsigned int NODE_2 = 2;
static const unsigned int NODE_3 = 3;
static const unsigned int NODE_4 = 4;

// should be a hash table (stl map<>) for retrieving addresses of nodes in a network
// returns sitch\s gate for certain node-name 
int nodeAddress(const char* nodeName)
{
	if( !strcmp("node1",nodeName) )
	{
		return 0;
	}
	
	else if( !strcmp("node2",nodeName) )
	{
		return 1;
	}
	else if( !strcmp("node3",nodeName) )
	{
		return 2;
	}
	else if( !strcmp("node4",nodeName) )
	{
		return 3;
	}
	else 
		return -1;
}



