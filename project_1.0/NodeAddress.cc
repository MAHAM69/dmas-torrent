#include "NodeAddress.h"

// should be a hash table (stl map<>) for retrieving addresses of nodes in a network
// returns sitch\s gate for certain node-name 
int NodeAddress::nodeAddress(const char* nodeName)
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
