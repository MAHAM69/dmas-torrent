#include "NodeManager.h"

// The module class needs to be registered with OMNeT++
Define_Module(NodeManager);

void NodeManager::initialize()
{
    // initialize randomization	
    srand((unsigned)time(0) + id()); 

	// get peer name	
	strcpy(peerName, par("peer_name"));

    // Get peer name to string variable.	
    oss << peerName; //<< par("peer_name");
    nodeName = oss.str();
    oss.str("");

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
	
    double delay = getRandomFloat(100., 2);
#ifdef DEBUG    
    ev << "Node: " << nodeName << " initialize is delayed: " << delay << endl;
#endif
    sendDelayed(startMsg, delay, "connectionManagerOut");
//	send(startMsg, "connectionManagerOut");
	 
	
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
		char tmp[20];
		strcpy( tmp, peersList[i].c_str() );
		NodeHandshakeMessage* handshakeMessage = generateHandshake("info_hash",peerName, tmp, false);
		
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
    string cvs = "";
    // Convert char* to string
    oss << trackerResponse->getCvs();
    cvs = oss.str();
    oss.str("");
    cout << cvs << endl;
    
    string rest  = "";
    int i = 0;
    peersList.clear();
#ifdef DEBUG
	ev << "------------------------------" << endl;
        ev << "Name: " << nodeName << endl;
#endif
    while ( cvs != "" )
    {
	string *value = new string("");
	parse( cvs, *value, rest );
	cout << cvs << endl;
#ifdef DEBUG
        ev << "i=" << i << "   value: " << *value << endl;
#endif
	if ( i == 0 )
	{
	    if ( *value != nodeName )
	    {
		peersList.push_back( *value );
	    }
	}
	i = (i+1) % 5;
	cvs = rest;
    }
    cout << "Peers list (size: " << peersList.size() << "):" << endl;
    for ( int i = 0; i < peersList.size(); i++ )
    {
	cout << peersList[i] << endl;
    }
    handshakeToPeers();
}
