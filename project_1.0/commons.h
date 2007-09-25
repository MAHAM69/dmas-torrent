#ifndef COMMONS_H_
#define COMMONS_H_

#define DEBUG 1


#define MSG_GET "GET"
#define MSG_TRACKER_RESPONSE "TRACKER_RESPONSE"

#define TRACKER_PORT 0
#define ACTIVE_PEER  1

// default piece size 16kB
#define PIECE_SIZE 16


// constants used as addresses in internal node communication
static const unsigned int NODE_MANAGER = 0;
static const unsigned int DATA_MANAGER = 1;
static const unsigned int CONNECTION_MANAGER = 2;


//----------------------------------------------------
// constants used as types of messages between peers and between submodules
// handshake message between peers
static const unsigned int MSG_HANDSHAKE = 0;
// message to data manager that connecitns has bee nestablished and data processing may be started
static const unsigned int MSG_CONNECTIONS_ESTABLISHED = 1;
// response for a handshake; message identical to HANDSHAKE_MSG but no response is expected
static const unsigned int MSG_HANDSHAKE_RESPONSE = 2;

static const unsigned int MSG_BITFIELD = 3;
// bitfield message sent by connection manager to data manager to fill payload 
static const unsigned int MSG_SELF_BITFIELD = 4;
// response for a bitfield; message identical to BITFIELD_MSG but no response is expected
static const unsigned int MSG_BITFIELD_RESPONSE = 5;

//intrested message
static const unsigned int MSG_INTERESTED = 6;

static const unsigned int MSG_NOT_INTERESTED = 7;
//choked/unchoked messages
static const unsigned int MSG_CHOKED = 8;
static const unsigned int MSG_UNCHOKED = 9;

static const unsigned int MSG_REQUEST = 10;


static const unsigned int MSG_HAVE = 11;

// LOCAL SUBMODULES SELF-MESSAGES
// scheduled by Data manager to start requests for blocks
static const unsigned int MSG_START_REQUESTS = 101; 

//---------------------------------------

static const unsigned int NODE_1 = 1;
static const unsigned int NODE_2 = 2;
static const unsigned int NODE_3 = 3;
static const unsigned int NODE_4 = 4;

//maximal length of peer list in tracker
const int MAX_PEERS = 100; 

//number of peers sent in tracker's response
const int NO_PEERS=2;

//interval in seconds that the client should 
//wait between sending regular requests to tracker
const double REQ_INTERVAL=100;


//max number of downloads for peer
const int NO_DOWNLOADS = 1;

//max number of uploads for peer
const int NO_UPLOADS = 2;

#endif /*COMMONS_H*/
