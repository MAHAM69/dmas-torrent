// message sent by peers

cplusplus {{
#include "NodeMessage_m.h"
}};

class NodeMessage;

message PeerToPeerMessage extends NodeMessage
{
	fields:
		// length of a message
		unsigned int length;
		
		// type of a message sent
		unsigned char messageId;	
		
		// since OMNET++ message cannot carry any complicated structure inside user's defined class,
		// such as list, vector etc. the actual payload of a message is carried not inside 'message'
		// field but simply by the message
		unsigned char payload[];
}