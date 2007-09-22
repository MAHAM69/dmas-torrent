//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003 Ahmet Sekercioglu
// Copyright (C) 2003-2005 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


#include "connection.h"
#include <string>
#include <omnetpp.h>
//#include <iostream>
// careful! nodeMessage contains definition of NodeHandshakeMessage; name is misleading
#include "NodeHandshakeMessage_m.h"
#include "commons.h"


using namespace std;

// generates handshake message 
NodeHandshakeMessage* generateHandshake(char* info_hash, char* peer_name, char* dest, bool response);

class NodeManager : public cSimpleModule
{
	private: 
		// peers list received from tracker
		//char peersList[10][10];		
		char peerName[20];		
		
		vector<char*> peersList;
		
	protected:
		// The following redefined virtual function holds the algorithm.
		virtual void initialize();
		virtual void handleMessage(cMessage *msg);
		// sends handshake to peers addresses read from tracker's response
		void handshakeToPeers();
		
};

// The module class needs to be registered with OMNeT++
Define_Module(NodeManager);


void NodeManager::initialize()
{
	// NodeManager::initialize is supposed to: simulate reading of .torrent file,
	// setting peer name, sending GET to tracker and reading tracker's response, sending handshake to known peers
	
	
	
	// get peer name	
	strcpy(peerName, par("peer_name"));	
	
	// exemplary initialization of peersList
	
	//char peers_table;[5][20];
		
	
	peersList.push_back("node2");
	peersList.push_back("node3");
	peersList.push_back("node4");
	
	int isNode1 = strcmp(peerName,"node1");
	
	// if is node1 then send handshake 
	if(isNode1 == 0){
		handshakeToPeers();
	}			
}

void NodeManager::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we just send it to the other module, through
    // gate `out'. 
	
	NodeHandshakeMessage *myMsg = NULL;
	myMsg = check_and_cast<NodeHandshakeMessage *>(msg);
	if(myMsg != NULL)
	{
		
	}
}

void NodeManager::handshakeToPeers()
{
	// sends handshake to peers listed on peersList
	for(unsigned int i =0; i < peersList.size(); i++)
	{
		/*
		// create a message of handshake 
		PeersHandshake* _handshake = new PeersHandshake("info_hash",peerName);
		
		// pass handshake to connection manager
		NodeHandshakeMessage* handshakeMessage = new NodeHandshakeMessage();
		
		handshakeMessage->setType(HANDSHAKE_MSG);		
		
		handshakeMessage->setDestination(peersList[i]);
		handshakeMessage->setHandshake(*_handshake);
		*/
		NodeHandshakeMessage* handshakeMessage = generateHandshake("info_hash",peerName,peersList[i], false);
		
		ev << peerName << " sending handshake to "<< peersList[i] << endl; 
			
		send(handshakeMessage,"connectionManagerOut");
	}
}



class ConnectionManager : public cSimpleModule
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
		
	protected:
		virtual void initialize();
		virtual void handleMessage(cMessage *msg);
		void handleHandshake(NodeHandshakeMessage* myHandshakeMsg);
};

Define_Module(ConnectionManager);


void ConnectionManager::initialize()
{
	strcpy(peerName,par("peer_name"));
	
	// initialize active connections - maximum number of 
	// uploads and downloads to be read from parameters (now 1,1)
	activeConnections = new ActiveConnections(1,1);	
	
	notifiedDataManager = false;
	
	/*
	NodeMessage *msg = new NodeMessage("messageName");
	msg->setDestination(CONNECTION_MANAGER);
	// examplary content	

	msg->setContentArraySize(10);
	for(int i =0; i < 10; i++)
		msg->setContent(i, i+20);

	send(msg,"nodeOut");
	*/
}


void ConnectionManager::handleMessage(cMessage *msg)
{	
	NodeMessage *myMsg = NULL;
	myMsg = check_and_cast<NodeMessage *>(msg);
	if(myMsg != NULL)
	{
		// message is correct (not NULL), processing message:
		
		// check if message is own message (same module, sent either by datamanager or nodeManager)
		// ConnectionManager as the only submodule of a peer has to check whether the incoming message
		// is own message or external message since it is the only one connection both externaly with 
		// the outside and internally with submodules of the same peer		
		int ownMessage = strcmp(myMsg->getDestination(), peerName);		
		if (ownMessage != 0)
		{
			// message is own message (destination is not peer )		
			if(myMsg->getType() == HANDSHAKE_MSG )
			{
				
				send(myMsg,"nodeOut");
			}
		}		
		else 
		{
			// message is NOT own message (hence: peer is intended receiver)
			if(myMsg->getType() == HANDSHAKE_MSG )
			{			
				// message is handshake so try casting to NodeHandshakeMessage
				NodeHandshakeMessage* myHandshakeMsg = NULL;
				myHandshakeMsg = check_and_cast<NodeHandshakeMessage *>(myMsg);
				
				if(myHandshakeMsg != NULL )
				{
					handleHandshake(myHandshakeMsg);
				}
			}
			else if (myMsg->getType() == HANDSHAKE_RESPONSE)
			{
				// here be genrating and sending of a bitfield message and adding sender of a responses to 
				// the list of connections- to be done tomorrow 
			}
			delete myMsg;
		}

	}
}

void ConnectionManager::handleHandshake(NodeHandshakeMessage* myHandshakeMsg)
{
	
	// if message is peer handshake, add it to list of connections				
	PeersConnected newConnection( myHandshakeMsg->getHandshake().peerId );
	
	connectionsList.push_back(newConnection);
						
	if( !notifiedDataManager )
	{
		//notify data manager that connections were established and it may start requesting data
		
		NodeMessage* conEstablished = new NodeMessage("connection established");
		conEstablished->setType(CONNECTIONS_ESTABLISHED_MSG);
		
		send(conEstablished,"dataManagerOut");
		notifiedDataManager = true;
	}
	
	
	char bubel[100]; 
	strcpy(bubel, "handshake received from\n");
	strcat(bubel, myHandshakeMsg->getHandshake().peerId);
	
	bubble(bubel);
	
	ev << peerName << " handshake message received from " <<  myHandshakeMsg->getHandshake().peerId << "\n";					
	
	// respond to handshake message - create own handshake message and send it to sender of a handshake
	NodeHandshakeMessage* handshakeResponse = 
		generateHandshake("info_hash", peerName, myHandshakeMsg->getHandshake().peerId, true);
	
	send(handshakeResponse,"nodeOut");
	
	
}


class DataManager : public cSimpleModule
{
	private:
		bool connectionEstablished;
		char peerName[20];
	protected:
		virtual void initialize();
		virtual void handleMessage(cMessage *msg);
};

void DataManager::initialize()
{
	connectionEstablished = false;
	strcpy(peerName, par("peer_name"));
}

void DataManager::handleMessage(cMessage *msg)
{	
	NodeMessage* myMsg = NULL; 
	myMsg = check_and_cast<NodeMessage*>(msg);
	
	if(myMsg != NULL)
	{
		if(!connectionEstablished)
		{
			// no message can be processed, if CONNECTION_ESTABLISHED_MESSAGE has not arrived
			if(myMsg->getType() == CONNECTIONS_ESTABLISHED_MSG ){
				// connections with peers were established so set the flag to true and process other messages
				connectionEstablished = true;				
				ev << "data manager understood: connection established\n";
			}
			delete myMsg;
		}
		else
		{
			// connection with peers has been established, messages other than CONNECTIIONS_ESTABLISHED may now be processed
		}
	}
}

Define_Module(DataManager);



// switch passes messages to the intended node
class Switch : public cSimpleModule
{	
	protected:			
			virtual void handleMessage(cMessage *msg);
};

void Switch::handleMessage(cMessage *msg)
{
	NodeMessage* myMsg = NULL;
	myMsg = check_and_cast<NodeMessage *>(msg);
	if(myMsg != NULL)
	{
		const char* dest = myMsg->getDestination();
		
		char bubel[50];
		
		strcpy(bubel, "sending to\n");
		strcat(bubel, myMsg->getDestination());
		
		bubble(bubel);
		
		// check which gate corresponds to the receiver
		int port = nodeAddress(dest);		
		send(myMsg,"out",port);
	}
}

Define_Module(Switch);



NodeHandshakeMessage* generateHandshake(char* info_hash, char* peer_name, char* dest, bool response)
{
	// create a message of handshake 
	PeersHandshake* _handshake = new PeersHandshake("info_hash",peer_name);
	
	// pass handshake to connection manager
	NodeHandshakeMessage* handshakeMessage = new NodeHandshakeMessage();
	if(!response)
		handshakeMessage->setType(HANDSHAKE_MSG);
	else
		handshakeMessage->setType(HANDSHAKE_RESPONSE);
	
	handshakeMessage->setDestination(dest);
	handshakeMessage->setHandshake(*_handshake);

	return handshakeMessage;
}

