#include "Tracker.h"

#include "commons.h"
#include "TrackerResponse_m.h"
#include "RequestMessage_m.h"
#include "NodeAddress.h"
#include "omnetpp.h"

Define_Module(Tracker);

void Tracker::initialize()
{
    //refreshTime=10;
    //send=NULL;
    //receive=NULL;
    peersCounter = 0;
}

void Tracker::handleMessage(cMessage* msg)
{
    RequestMessage* myMsg  = NULL;
    myMsg = check_and_cast<RequestMessage *>(msg);
    
    char* new_ID;
    double new_ip = 0;
    int new_port = 0;
    char new_hash[20];
    char* event;
    double left;
    
    
    strcpy(new_ID, myMsg->getID());
    new_ip = myMsg->getIp();
    new_port = myMsg->getPort();
    strcpy(new_hash, myMsg->getInfo_hash());
    strcpy(event, myMsg->getEvent());
    left = myMsg->getLeft();
    
    
    if (strcmp(event, "stopped")==0)
	delPeer(new_ID);
    else
    {
	if (!strcmp(event, "started")==0)
	    addPeer(new_ip, new_port, left, new_ID, new_hash);
	response(new_ID );
    }

}




void Tracker::addPeer(double ip, int port, double left, char* ID, char* hash_info)
{
    
    bool exists=false;
    for(int i=0;i<peersCounter;i++)
	if (ip == peers[i].getIp())
	{
	    exists=true;
	    break;
	}
    if ((!exists) && (peersCounter < MAX_PEERS - 1))
    {
	peersCounter++;
	peers[peersCounter].setIp( ip );
	peers[peersCounter].setPort( port );
	peers[peersCounter].setID( ID );
	peers[peersCounter].setHash( hash_info );
	peers[peersCounter].setLeft( left );
    }
    
}

void Tracker::delPeer(char* ID)
{
    for(int i=0; i<peersCounter; i++)
	if ( strcmp(peers[i].getID(), ID )==0 )
	{
	    peers[i].setIp( peers[peersCounter].getIp() );
	    peers[i].setPort( peers[peersCounter].getPort() );
	    peers[i].setID( peers[peersCounter].getID() );
	    peers[i].setHash( peers[peersCounter].getHash() );
	    peers[i].setLeft( peers[peersCounter].getLeft() );
	    peersCounter--;
	}
}

void Tracker::response(char* ID)
{

    TrackerResponse *responseMsg = NULL;
    int port = nodeAddress(ID);
    int seeders = 0;
    int leechers = 0;
    PeerBean peerList[NO_PEERS]; //list of peers to be sent in response
    
    for (int i=0; i<peersCounter; i++)
    {
	if (peers[i].getLeft() == 0)
	    seeders++;
	else 
	    leechers++;
	peerList[i] = peers[i]; //only first NO_PEERS peers on the tracker's list will be added to the response list
    }
    
    responseMsg->setInterval( REQ_INTERVAL );
    responseMsg->setNo_seeders( seeders );
    responseMsg->setNo_leechers( leechers );
    responseMsg->setPeers( NO_PEERS, peerList );

    send(responseMsg, "trackerOut", port);

}

