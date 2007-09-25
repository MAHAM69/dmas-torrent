#ifndef NODEMANAGER_H_
#define NODEMANAGER_H_

#include <string>
#include <omnetpp.h>
#include <iostream>
#include <sstream>
#include "HandshakeGenerator.h"
#include "NodeAddress.h"
#include "CVSParser.h"
//#include "StringConvert.h"
#include "NumberTools.h"
#include "NodeHandshakeMessage_m.h"
#include "RequestMessage_m.h"
#include "TrackerResponse_m.h"
#include "commons.h"

using namespace std;

/**
 *
 */
class NodeManager : public cSimpleModule,
		    public HandshakeGenerator,
		    public NodeAddress,
		    public CVSParser
{
    private: 
	/** peers list received from tracker */
	//char peersList[10][10];		

	string nodeName;
	char peerName[20];
		
	vector<string> peersList;
		
    protected:
	/** The following redefined virtual function holds the algorithm. */
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
	/** Sends handshake to peers addresses read from tracker's response. */
	void handshakeToPeers();
	void receiveTrackerResponse( cMessage *msg );
};

#endif /*NODEMANAGER_H_*/
