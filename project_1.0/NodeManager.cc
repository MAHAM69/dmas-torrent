#include "NodeManager.h"
// The module class needs to be registered with OMNeT++
Define_Module(NodeManager);


void NodeManager::initialize()
{
	// get peer name	
	strcpy(peerName, par("peer_name"));	

	// NodeManager::initialize is supposed to: simulate reading of .torrent file,
	// setting peer name, sending GET to tracker and reading tracker's response, sending handshake to known peers
	
	RequestMessage* startMsg = NULL;
	
	startMsg = new RequestMessage( MSG_GET );
	
	startMsg->setInfo_hash("1");
	startMsg->setID( peerName );
	startMsg->setIp( nodeAddress( peerName ) );
	startMsg->setLeft(4096);//1024*1024*1024);
	startMsg->setPort(1234);
	startMsg->setEvent("started");
	ev << "Node: " << peerName << endl;
	
	send(startMsg, "connectionManagerOut");
	 
	
	// exemplary initialization of peersList
	
	//char peers_table;[5][20];
		
/*	
	peersList.push_back("node2");
	peersList.push_back("node3");
	peersList.push_back("node4");
	
	int isNode1 = strcmp(peerName,"node1");
	
	// if is node1 then send handshake 
	if(isNode1 == 0){
		handshakeToPeers();
	}			
*/	
}

void NodeManager::handleMessage(cMessage *msg)
{
#ifdef DEBUG
    ev << "NodeManager::handleMessage   message name: " << ((msg != NULL) ? msg->name() : "NULL") << endl;
#endif
    if ( msg == NULL ) return;
    if ( strcmp( msg->name(), MSG_TRACKER_RESPONSE ) == 0 )
    {
	receiveTrackerResponse( msg );
	delete msg;
    }
    else
    {
	// Unknown message.
	delete msg;
    }
/*    
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we just send it to the other module, through
    // gate `out'. 

	NodeMessage* myMsg = NULL;
	myMsg = check_and_cast<NodeMessage *>(msg);
	if(myMsg != NULL)
	{		
		if(strcmp( msg->name(), MSG_TRACKER_RESPONSE ) == 0)
		{
			// examplary!! initialize handshake; for testing purposes only; here should be triggered reading peersList from tracker
			// and only after thath handshake
			
			
			peersList.push_back("node2");
			peersList.push_back("node3");
			peersList.push_back("node4");
			
			int isNode1 = strcmp(peerName,"node1");
			
			// if is node1 then send handshake 
			if(isNode1 == 0){
				handshakeToPeers();
			}					
		}
		
		delete myMsg;				
	}
*/	
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

/**
 *
 */
void NodeManager::receiveTrackerResponse( cMessage *msg )
{
    TrackerResponse* trackerResponse = NULL;
    trackerResponse = check_and_cast<TrackerResponse *>(msg);
    if ( trackerResponse == NULL ) return;
    ostringstream out;
    string cvs = "";
    out << trackerResponse->getCvs();
    cvs = out.str();
    
    string value = "";
    string rest  = "";
    int i = 0;
    peersList.clear();
    while ( cvs != "" )
    {
	value = "";
	parse( cvs, value, rest );
	cout << cvs << " | " << value << " | " << rest << endl;
	char ID[20];
	if ( i == 0 )
	{
	    strcpy( ID, value.c_str() );
#ifdef DEBUG
            ev << "i=" << i << "   ID: " << ID << endl;
#endif
	    if ( strcmp( peerName, ID ) != 0 )
	    {
		peersList.push_back( ID );
	    }
	}
	i = (i+1) % 5;
	cvs = rest;
    }
    handshakeToPeers();
}
