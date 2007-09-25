#ifndef NODEMANAGER_H_
#define NODEMANAGER_H_

#include <string>
#include <omnetpp.h>
#include <iostream>
#include "HandshakeGenerator.h"
#include "NodeAddress.h"
#include "NodeHandshakeMessage_m.h"
#include "RequestMessage_m.h"
#include "commons.h"

using namespace std;

/**
 *
 */
class NodeManager : public cSimpleModule,
		    public HandshakeGenerator,
		    public NodeAddress
{
    private: 
	/** peers list received from tracker */
	//char peersList[10][10];		

	char peerName[20];
		
	vector<char*> peersList;
		
    protected:
	/** The following redefined virtual function holds the algorithm. */
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
	/** Sends handshake to peers addresses read from tracker's response. */
	void handshakeToPeers();
};

#endif /*NODEMANAGER_H_*/
