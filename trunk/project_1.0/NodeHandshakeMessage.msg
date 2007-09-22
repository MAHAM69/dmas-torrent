
cplusplus {{
#include "PeersHandshake.h"
#include "nodeMessage_m.h"
}};

class noncobject PeersHandshake;
class NodeMessage;

message NodeHandshakeMessage extends NodeMessage
{
    fields:    	
		// torrent-like handshake message (see:definition of PeersHandshake class)
			PeersHandshake handshake;			
}