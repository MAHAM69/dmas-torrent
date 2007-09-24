#include "HandshakeGenerator.h"


NodeHandshakeMessage* HandshakeGenerator::generateHandshake(char* info_hash, char* peer_name, char* dest, bool response)
{
	/** create a message of handshake */
	PeersHandshake* _handshake = new PeersHandshake("info_hash",peer_name);
	
	/** pass handshake to connection manager */
	NodeHandshakeMessage* handshakeMessage = new NodeHandshakeMessage();
	if(!response)
		handshakeMessage->setType(HANDSHAKE_MSG);
	else
		handshakeMessage->setType(HANDSHAKE_RESPONSE);
	
	handshakeMessage->setDestination(dest);
	handshakeMessage->setHandshake(*_handshake);

	return handshakeMessage;
}
