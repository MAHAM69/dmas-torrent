#include "NodeAddress.h"

// should be a hash table (stl map<>) for retrieving addresses of nodes in a network
// returns sitch\s gate for certain node-name 
int NodeAddress::nodeAddress( const char* nodeName )
{
	if( !strcmp("tracker", nodeName ))
	{
		return TRACKER_PORT;
	}
	else
	{
	    cout << "Node: " << nodeName << " | ";
	    oss << nodeName;
	    string s = oss.str();
	    oss.str("");
	    string value = "";	    
	    for ( int i = 4; i < s.length(); i++ ) value += s[i];
	    cout << "Value: " << value << endl;
	    if ( value == "" ) return -1;
	    int result = str2int( value );
	    // because tracker uses address 0.
	    result++;
	    return result;
	}
}
