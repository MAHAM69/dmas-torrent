#include "ConnectionManager.h"

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
	PeersConnected newConnection( myHandshakeMsg->getHandshake().getPeerId() );
	
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
	strcat(bubel, myHandshakeMsg->getHandshake().getPeerId());
	
	bubble(bubel);
	
	ev << peerName << " handshake message received from " <<  myHandshakeMsg->getHandshake().getPeerId() << "\n";
	
	// respond to handshake message - create own handshake message and send it to sender of a handshake
	NodeHandshakeMessage* handshakeResponse = 
		generateHandshake("info_hash", peerName, myHandshakeMsg->getHandshake().getPeerId(), true);
	
	send(handshakeResponse,"nodeOut");
	
	
}
