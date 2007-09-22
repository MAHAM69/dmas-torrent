#include "PeersConnected.h"

PeersConnected::PeersConnected( char* peer_name )
{
    strcpy(peerName, peer_name);
    // standard (after handshake) settings of a connection
    amChoking = true;
    amInterested = false;
    peerChoking = true;
    peerInterested = false;
}
