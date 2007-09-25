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
#ifdef DEBUG
	ev << "Tracker::handleMessage   message name: " << ((msg != NULL) ? msg->name() : "NULL") << endl;
#endif

    if ( msg == NULL ) return;
    if ( strcmp( msg->name(), MSG_GET ) == 0 )
    {
        ev << "Received GET message from " << endl;
        
        RequestMessage* myMsg  = NULL;
        myMsg = check_and_cast<RequestMessage *>(msg);

	ev << "Event " << myMsg->getEvent() << endl;

        if (strcmp(myMsg->getEvent(), "stopped")==0)
        {
    	    delPeer(myMsg->getID());
    	}
        else
        {
	    if (strcmp(myMsg->getEvent(), "started")==0)
	    {
		addPeer(    myMsg->getIp(), 
			    myMsg->getPort(), 
			    myMsg->getLeft(),
			    myMsg->getID(),
			    myMsg->getInfo_hash() );
	    }
	    response( myMsg->getID() );
	}
	delete msg;
    }
}




void Tracker::addPeer( const double ip, const int port, const double left, const char* ID, const char* hash_info )
{
    bool exists=false;
    for(int i=0;i<peersCounter;i++)
	if (ip == peers[i].getIp())
	{
	    exists=true;
	    break;
	}
    if ((!exists) && (peersCounter < (MAX_PEERS-1)))
    {
	peers[peersCounter].setIp( ip );
	peers[peersCounter].setPort( port );
	peers[peersCounter].setID( ID );
	peers[peersCounter].setHash( hash_info );
	peers[peersCounter].setLeft( left );
	peersCounter++;
    }
    
}

void Tracker::delPeer( const char* ID )
{
    for(int i=0; i<peersCounter; i++)
	if ( strcmp(peers[i].getID(), ID )==0 )
	{
	    peersCounter--;
	    if ( peersCounter == 0 )
	    {
		return;
	    }
	    peers[i].setIp( peers[peersCounter].getIp() );
	    peers[i].setPort( peers[peersCounter].getPort() );
	    peers[i].setID( peers[peersCounter].getID() );
	    peers[i].setHash( peers[peersCounter].getHash() );
	    peers[i].setLeft( peers[peersCounter].getLeft() );
	}
}

void Tracker::response( const char* ID )
{

    TrackerResponse *responseMsg = NULL;
    int port = nodeAddress(ID);
    int seeders = 0;
    int leechers = 0;

#ifdef DEBUG
    ev << "Response to " << ID << endl;
    ev << "Peers counter: " << peersCounter << endl;
#endif

    ostringstream out;
    string s = "";
    for (int i=0; i < min( peersCounter, NO_PEERS ); i++)
    {
#ifdef DEBUG
	cout << "peers[" << i << "].ID = " << peers[i].getID() << endl;
	ev   << "peers[" << i << "].ID = " << peers[i].getID() << endl;
#endif
	if ( peers[i].getLeft() > 0.0 )
	    leechers++;
	else 
	    seeders++;
	out << peers[i].getID() << ";";
	out << peers[i].getHash() << ";";
	out << peers[i].getIp() << ";";
	out << peers[i].getPort() << ";";
	out << peers[i].getLeft() << ";";
    }
    s = out.str();

#ifdef DEBUG
ev << "Seeders: " << seeders << endl;
ev << "Leechers: " << leechers << endl;
#endif

    responseMsg = new TrackerResponse( MSG_TRACKER_RESPONSE );
    responseMsg->setDestination(ID);
    responseMsg->setInterval( REQ_INTERVAL );
    responseMsg->setNo_seeders( seeders );
    responseMsg->setNo_leechers( leechers );
    responseMsg->setCvs( s.c_str() );

    send(responseMsg, "trackerOut");
}

