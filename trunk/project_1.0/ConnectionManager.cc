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

/**
 * answer to the handshake by generating and sending handshake response; add sender of the handshake to the list
 * of established connection 
 */ 
void ConnectionManager::handleHandshake(NodeHandshakeMessage* myHandshakeMsg)
{
	ev << "handleHandshake" << endl;	
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
	ev << "handleHandshakeResponse" << endl;
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
    ev << "msgHandShake" << endl;
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

    if (myMsg == NULL)
    	ev << "NULLLLLLLLLLLLLLLLLL" << endl;
    
    NodeHandshakeMessage* myHandshakeMsg = NULL;
    myHandshakeMsg = check_and_cast<NodeHandshakeMessage *>(myMsg);
    if (myHandshakeMsg == NULL)
	ev << "DRUUUUUUUUUUUUUUUGI NULL" << endl;
    ev << "msgHandshakeResponse" << endl;	
    if(myHandshakeMsg != NULL )
    {
	// add sender of the handhsake response to the 
	// connection established list
	handleHandshakeResponse(myHandshakeMsg);
		
	// send sketch of bitfield message to data manager, 
	// with destination to the handshaked node 
															
	PeerToPeerMessage* peerMessage = 
		this->generateBitfieldMessage(myHandshakeMsg->getHandshake().getPeerId(), peerName); 					

	ev << "asdfasdfasdf" << endl;					
	send(peerMessage,"dataManagerOut");

	delete myMsg;
    }

}

void ConnectionManager::msgConnectionsEstablished(NodeMessage* myMsg)
{
    ev << "msgConnectionEstablished" << endl;
}

void ConnectionManager::msgBitField(NodeMessage* myMsg)
{
    ev << "msgBitField" << endl;
    send(myMsg,"dataManagerOut");				
}

void ConnectionManager::msgSelfBitField(NodeMessage* myMsg)
{
    ev << "msgSelfBitField" << endl;
}

void ConnectionManager::msgInterested(NodeMessage* myMsg)
{
    ev << "msgInterested" << endl;
    ChokeRandom* choke = new ChokeRandom();
    NodeMessage* response = new NodeMessage();

    //TODO: wiadomosc typu MSG_INTERESTED ma miec pole ID zawierajace id wysylajacego    
    //response->setDestination(myMsg->par("ID"));
    if (choke->choked())
    {
	response->setType(MSG_CHOKED);
    }
    else
    {
	response->setType(MSG_UNCHOKED);
    }
    send(response,"nodeOut");	

    delete myMsg;
}

void ConnectionManager::msgChoked(NodeMessage* myMsg)
{
    ev << "msgChoked" << endl;
    delete myMsg;
}

void ConnectionManager::msgUnchoked(NodeMessage* myMsg)
{
    ev << "msgUnchoked" << endl;
    delete myMsg;
}
