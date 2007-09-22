#ifndef PEERSCONNECTION_H_
#define PEERSCONNECTION_H_

#include <vector>
#include <iostream>

using namespace std;

/**
 *
 */
class PeersConnected
{
    private:
	/** name of the peer that sent handshake */
	char peerName[20];
	/** am I choking connection */
	bool amChoking;
	/** am i interested */
	bool amInterested;
	
	/** peer is chocking */
	bool peerChoking;
	/** peer interested */
	bool peerInterested;

    public:
	PeersConnected(char* peer_name);
};

#endif /*PEERSCONNECTION_H_*/
