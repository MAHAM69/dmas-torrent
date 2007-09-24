#ifndef CONNECTIONMANAGER_H_
#define CONNECTIONMANAGER_H_

#include <string>
#include <omnetpp.h>
#include "HandshakeGenerator.h"
#include "NodeHandshakeMessage_m.h"
#include "PeersConnected.h"
#include "ActiveConnections.h"
#include "commons.h"
#include "P2PMessage_m.h"
#include "PeerMessageGenerator.h"

using namespace std;

/**
 *
 */
class ConnectionManager:    public cSimpleModule,
			    public HandshakeGenerator,
		    	public PeerMessageGenerator
{
	private:
		char peerName[20];
		
		// flag determines if DataManager has been notified about established connections
		// flag is set to true, if connections has been established and it's possible
		// to send bitfields messages and requests
		bool notifiedDataManager;			
		
		// list of peers from which handshake was received and 
		vector<PeersConnected> connectionsList;
		
		// list of active connections		
		ActiveConnections* activeConnections;
		
		
		void handleHandshake(NodeHandshakeMessage* myHandshakeMsg);
		void handleHandshakeResponse(NodeHandshakeMessage* myHandshakeMsg);
		
	protected:
		virtual void initialize();
		virtual void handleMessage(cMessage *msg);
		
};

#endif /*CONNECTIONMANAGER_H_*/