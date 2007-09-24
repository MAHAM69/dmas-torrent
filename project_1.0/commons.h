#ifndef COMMONS_H_
#define COMMONS_H_

// constants used as addresses in internal node communication
static const unsigned int NODE_MANAGER = 0;
static const unsigned int DATA_MANAGER = 1;
static const unsigned int CONNECTION_MANAGER = 2;

// constants used as types of messages between peers and between submodules
// handshake message between peers
static const unsigned int HANDSHAKE_MSG = 0;
// message to data manager that connecitns has bee nestablished and data processing may be started
static const unsigned int CONNECTIONS_ESTABLISHED_MSG = 1;
// response for a handshake; message identical to HANDSHAKE_MSG but no response is expected
static const unsigned int HANDSHAKE_RESPONSE = 2;

static const unsigned int BITFIELD_MSG = 3;
// bitfield message sent by connection manager to data manager to fill payload 
static const unsigned int SELF_BITFIELD_MSG = 4;
// response for a bitfield; message identical to BITFIELD_MSG but no response is expected
static const unsigned int BITFIELD_RESPONSE = 5;

static const unsigned int NODE_1 = 1;
static const unsigned int NODE_2 = 2;
static const unsigned int NODE_3 = 3;
static const unsigned int NODE_4 = 4;

#endif /*COMMONS_H*/
