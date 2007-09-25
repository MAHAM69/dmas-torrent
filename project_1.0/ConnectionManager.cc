#include "ConnectionManager.h"

Define_Module(ConnectionManager);


void ConnectionManager::initialize()
{
	freeUploadSlots = NO_UPLOADS;
	
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
    ev << "CM receives " << msg->name() << endl;
    if ( strcmp( msg->name(), MSG_GET ) == 0 )
    {
	send(msg, "nodeOut");
    }
    else if ( strcmp( msg->name(), MSG_TRACKER_RESPONSE ) == 0 )
    {
    	
    	// temporarly, so that the simulation can continue; send handshake to some nodes;
    	send(msg,"nodeManagerOut");
    	
    }
    else
    {
#ifdef DEBUG
	ev << "handleHandshake    message name: " << ((msg != NULL) ? msg->name() : "NULL") << endl;
#endif
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
			// message is own message (destination is not this peer )						
				send(myMsg,"nodeOut");			
		}		
		else 
		{
			// message is NOT own message (hence: peer is intended receiver)
			ev << "Aktualny: " << peerName << "   Typ: " << myMsg->getType() << endl;
			
			switch (myMsg->getType())
			{
			    case MSG_HANDSHAKE:
				msgHandshake(myMsg);
				break;
			
			    case MSG_HANDSHAKE_RESPONSE:
				msgHandshakeResponse(myMsg);
				break;
				
			    case MSG_CONNECTIONS_ESTABLISHED:
				msgConnectionsEstablished(myMsg);
				break;

			    case MSG_BITFIELD_RESPONSE:				
			    case MSG_BITFIELD:
				msgBitField(myMsg);
				break;
				
			    case MSG_SELF_BITFIELD:
				msgSelfBitField(myMsg);
				break;
				
			    case MSG_INTERESTED:
				msgInterested(myMsg);
				break;
			
			    case MSG_NOT_INTERESTED:
				msgNotInterested(myMsg);
				break;
				
			    case MSG_CHOKED:
				msgChoked(myMsg);
				break;
				
			    case MSG_UNCHOKED:
				msgUnchoked(myMsg);
				break;
				
			    default:
				break;    
			};//end switch
		}//end if
	}//end if
    }
}

/**
 * answer to the handshake by generating and sending handshake response; add sender of the handshake to the list
 * of established connection 
 */ 
void ConnectionManager::handleHandshake(NodeHandshakeMessage* myHandshakeMsg)
{
#ifdef DEBUG
	ev << "handleHandshake    message name: " << ((myHandshakeMsg != NULL) ? myHandshakeMsg->name() : "NULL") << endl;
#endif
	// if message is peer handshake, add it to list of connections				
	PeersConnected newConnection( myHandshakeMsg->getHandshake().getPeerId() );
	
	connectionsList.push_back(newConnection);	
	
	if( !notifiedDataManager )
	{
		//notify data manager that connections were established and it may start requesting data
		
		NodeMessage* conEstablished = new NodeMessage("connection established");
		conEstablished->setType(MSG_CONNECTIONS_ESTABLISHED);
		
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

/**
 * adds sender of the handshake response to the list of connections
 */ 
void ConnectionManager::handleHandshakeResponse(NodeHandshakeMessage* myHandshakeMsg)
{				
#ifdef DEBUG
	ev << "handleHandshakeResponse    message name: " << ((myHandshakeMsg != NULL) ? myHandshakeMsg->name() : "NULL") << endl;
#endif
	// add to the list of connections
	PeersConnected newConnection( myHandshakeMsg->getHandshake().getPeerId() );
	
	connectionsList.push_back(newConnection);
	
	if( !notifiedDataManager )
	{
		//notify data manager that connections were established and it may start requesting data
		
		NodeMessage* conEstablished = new NodeMessage("connection established");
		conEstablished->setType(MSG_CONNECTIONS_ESTABLISHED);
		
		send(conEstablished,"dataManagerOut");
		notifiedDataManager = true;
	}
}


// message is handshake so try casting to NodeHandshakeMessage
void ConnectionManager::msgHandshake(NodeMessage* myMsg)
{
#ifdef DEBUG
	ev << "msgHandshake    message name: " << ((myMsg != NULL) ? myMsg->name() : "NULL") << endl;
#endif
    NodeHandshakeMessage* myHandshakeMsg = NULL;
    myHandshakeMsg = check_and_cast<NodeHandshakeMessage *>(myMsg);

    if(myHandshakeMsg != NULL )
    {
	handleHandshake(myHandshakeMsg);
    }

    delete myMsg;
}


void ConnectionManager::msgHandshakeResponse(NodeMessage* myMsg)
{
#ifdef DEBUG
	ev << "msgHandshakeResponse    message name: " << ((myMsg != NULL) ? myMsg->name() : "NULL") << endl;
#endif
    
    NodeHandshakeMessage* myHandshakeMsg = NULL;
    myHandshakeMsg = check_and_cast<NodeHandshakeMessage *>(myMsg);

    if(myHandshakeMsg != NULL )
    {
	// add sender of the handhsake response to the 
	// connection established list
	handleHandshakeResponse(myHandshakeMsg);
		
	// send sketch of bitfield message to data manager, 
	// with destination to the handshaked node 
															
	PeerToPeerMessage* peerMessage = 
		this->generateBitfieldMessage(myHandshakeMsg->getHandshake().getPeerId(), peerName); 					

	send(peerMessage,"dataManagerOut");

	delete myMsg;
    }

}

void ConnectionManager::msgConnectionsEstablished(NodeMessage* myMsg)
{
#ifdef DEBUG
	ev << "msgConnectionsEstablished    message name: " << ((myMsg != NULL) ? myMsg->name() : "NULL") << endl;
#endif
}

void ConnectionManager::msgBitField(NodeMessage* myMsg)
{
#ifdef DEBUG
	ev << "msgBitField    message name: " << ((myMsg != NULL) ? myMsg->name() : "NULL") << endl;
#endif
    send(myMsg,"dataManagerOut");				
}

void ConnectionManager::msgSelfBitField(NodeMessage* myMsg)
{
#ifdef DEBUG
	ev << "msgSelfBitField    message name: " << ((myMsg != NULL) ? myMsg->name() : "NULL") << endl;
#endif
}

void ConnectionManager::msgInterested(NodeMessage* myMsg)
{
#ifdef DEBUG
	ev << "msgInterested    message name: " << ((myMsg != NULL) ? myMsg->name() : "NULL") << endl;
#endif
    ChokeRandom* choke = new ChokeRandom();
    NodeMessage* response = new NodeMessage();

    PeerToPeerMessage* p2pMsg = check_and_cast <PeerToPeerMessage*>(myMsg);
    
    if (p2pMsg)
    {
	response->setDestination(p2pMsg->getSender());
	if ((choke->choked()) && (!(freeUploadSlots > 0)))
	{
	    response->setType(MSG_CHOKED);
	}
	else
	{
	    //TODO: gdzies trzeba zwalniac sloty...
	    //pytanie gdzie :)
	    freeUploadSlots--;
	    response->setType(MSG_UNCHOKED);
	}
	send(response,"nodeOut");	
    }
    delete myMsg;
}

void ConnectionManager::msgNotInterested(NodeMessage* myMsg)
{
#ifdef DEBUG
	ev << "msgNotInterested    message name: " << ((myMsg != NULL) ? myMsg->name() : "NULL") << endl;
#endif
}

void ConnectionManager::msgChoked(NodeMessage* myMsg)
{
#ifdef DEBUG
	ev << "msgChoked   message name: " << ((myMsg != NULL) ? myMsg->name() : "NULL") << endl;
#endif
    send(myMsg, "dataManagerOut");
    delete myMsg;
}

void ConnectionManager::msgUnchoked(NodeMessage* myMsg)
{
#ifdef DEBUG
	ev << "msgUnchoked    message name: " << ((myMsg != NULL) ? myMsg->name() : "NULL") << endl;
#endif
    
    send(myMsg, "dataManagerOut");
    delete myMsg;
}
