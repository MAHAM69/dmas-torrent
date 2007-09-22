#ifndef HANDSHAKEGENERATOR_H_
#define HANDSHAKEGENERATOR_H_

#include <string>
#include <omnetpp.h>

// careful! nodeMessage contains definition of NodeHandshakeMessage; name is misleading
#include "NodeHandshakeMessage_m.h"
#include "commons.h"

using namespace std;

/**
 *
 */
class HandshakeGenerator
{
    private:
    public:
	/** generates handshake message */
	NodeHandshakeMessage* generateHandshake(char* info_hash, char* peer_name, char* dest, bool response);
};

#endif /*HANDSHAKEGENERATOR_H_*/
