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
