#include "ConnectionManager.h"

Define_Module(ConnectionManager);


void ConnectionManager::initialize()
{
    // Creating node name.
    int peerId = par("peerId");
    strcpy( peerName, par("peer_name") );
    oss << peerName << peerId;
    nodeName = oss.str();
    oss.str("");
    strcpy( peerName, nodeName.c_str() );


	//scheduled message	
	printf("CM init\n");
	NodeMessage* scheduledMsg = new NodeMessage();
	scheduledMsg->setType(MSG_SCHEDULED_CHOKE);
	scheduleAt(simTime()+10, scheduledMsg);
	
	freeUploadSlots = NO_UPLOADS;
	
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
#ifdef DEBUG
	ev << "ConnectionManager::handleHandshake    message name: " << ((msg != NULL) ? msg->name() : "NULL") << endl;
#endif
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
			    
			    case MSG_HAVE:
				msgHave(myMsg);
				break;
			    
			    case MSG_REQUEST:
				msgRequest(myMsg);
				break;
				
			    case MSG_SCHEDULED_CHOKE:
				ev << "weszlo" << endl;
				msgScheduledChoke(myMsg);
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
    NodeMessage* response = new NodeMessage();

    PeerToPeerMessage* p2pMsg = check_and_cast <PeerToPeerMessage*>(myMsg);
    
    if (p2pMsg)
    {
	response->setDestination(p2pMsg->getSender());
        int i=0;
    	    
        for (i=0; i<connectionsList.size();i++)
        {
	    if (strcmp(connectionsList[i].getPeerName(), p2pMsg->getSender()) == 0)
	    {
		connectionsList[i].setPeerChoking(true);
		break;
	    }
	}
	response->setType(MSG_CHOKED);
    }
    else
    {
        int i=0;
	    
        for (i=0; i<connectionsList.size();i++)
        {
	    if (strcmp(connectionsList[i].getPeerName(), p2pMsg->getSender()) == 0)
	    {
		connectionsList[i].setPeerInterested(true);
		break;
	    }
	}
	freeUploadSlots--;
	response->setType(MSG_UNCHOKED);
    }
    send(response,"nodeOut");	
    
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
    
    PeerToPeerMessage* toSend = new PeerToPeerMessage();//wiadomosc do wyslania
    PeerToPeerMessage* temp = check_and_cast<PeerToPeerMessage*>(myMsg);
    int index;//index wektora connectionsList
    
    toSend->setDestination(myMsg->getDestination());
    
    //wyszukanie kanalu odpowiadajacego wylosowanej wiadomosci
    for (int i=0;i<connectionsList.size();i++)
    {
	if (strcmp(connectionsList[i].getPeerName(), temp->getDestination()) == 0)
	{
	    index = i;
	    break;
	}
    }
    connectionsList[index].setPeerChoking(false);
    for (int i=0;i<requestList.size();i++)
    {
	if (strcmp((check_and_cast<NodeMessage*>(requestList[i]))->getDestination(), temp->getDestination()) == 0)
	{
	    index = i;
	    break;
	}
    }
    toSend->setType(MSG_REQUEST);
    toSend->setDestination(temp->getDestination());
    send(requestList[index], "nodeOut");
}

void ConnectionManager::msgHave(NodeMessage* myMsg)
{
#ifdef DEBUG
	ev << "msgHave    message name: " << ((myMsg != NULL) ? myMsg->name() : "NULL") << endl;
#endif

    freeUploadSlots++;

    int i=0;
    
    NodeMessage* nodeMsg = new NodeMessage();
    
    nodeMsg->setType(MSG_START_REQUESTS);
    send(nodeMsg, "dataManagerOut");
    
}

void ConnectionManager::msgRequest(NodeMessage* myMsg)
{
#ifdef DEBUG
	ev << "msgRequest    message name: " << ((myMsg != NULL) ? myMsg->name() : "NULL") << endl;
#endif

    PeerToPeerMessage* temp = check_and_cast<PeerToPeerMessage*>(myMsg);
    PeerToPeerMessage* toSend = new PeerToPeerMessage();
    int index = 0;
    
    if (myMsg->arrivedOn("DataManagerIn"))
    {
	addRequest(myMsg);
	
	for (int i=0;i<connectionsList.size();i++)
	{
	    if (strcmp(connectionsList[i].getPeerName(), temp->getDestination()) == 0)
	    {
		index = i;
		break;
	    }
	}
	connectionsList[index].setPeerChoking(true);
	if (!connectionsList[index].getAmInterested())
	{
	    connectionsList[index].setAmInterested(true);
	    toSend->setType(MSG_INTERESTED);
	    toSend->setDestination(myMsg->getDestination());
	}
    }
    else
	send(myMsg, "dataManagerOut");
	
}

void ConnectionManager::chokeRandom()
{
#ifdef DEBUG
	ev << "chokeRandom " << endl;
#endif
    NodeMessage *x = new NodeMessage();

    PeerToPeerMessage* toSend = new PeerToPeerMessage();//wiadomosc do wyslania

    
    int rnd = getRandomInt(requestList.size());//index wektora requestList
    int index;//index wektora connectionsList
    
    NodeMessage* temp = requestList[rnd];
    
    toSend->setDestination((check_and_cast<NodeMessage*>(temp))->getDestination());
    
    //wyszukanie kanalu odpowiadajacego wylosowanej wiadomosci
    for (int i=0;i<connectionsList.size();i++)
    {
	if (strcmp(connectionsList[i].getPeerName(), temp->getDestination()) == 0)
	{
	    index = i;
	    break;
	}
    }

    //ustawianie flag
    if (!connectionsList[index].getAmInterested())
    {
	connectionsList[index].setPeerInterested(true);
	toSend->setType(MSG_INTERESTED);
    }
    send(toSend, "nodeOut");
}


void ConnectionManager::msgScheduledChoke(NodeMessage* msg)
{
#ifdef DEBUG
	ev << "msgScheduledChoke " << endl;
#endif
    printf("msgScheduledChoke\n");
    chokeRandom();
    scheduleAt(simTime()+10, msg);
}
