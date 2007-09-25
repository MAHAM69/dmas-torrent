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
	void setPeerName(char* val)	{ strcpy(peerName, val); }
	void setAmChoking(bool val) 	{ amChoking = val; }
	void setAmInterested(bool val)	{ amInterested = val; }
	void setPeerChoking(bool val)	{ peerChoking = val; }
	void setPeerInterested(bool val){ peerInterested = val; }
	
	char* getPeerName()		{ return peerName; }
	bool getAmChoking()		{ return amChoking; }
	bool getAmInterested()		{ return amInterested; }
	bool getPeerChoking()		{ return peerChoking; }
	bool getPeerInterested()	{ return peerInterested; }
};

#endif /*PEERSCONNECTION_H_*/
