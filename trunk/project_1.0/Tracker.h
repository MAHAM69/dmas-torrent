#ifndef TRACKER_H
#define TRACKER_H

#include <omnetpp.h>
#include "PeerBean.h"
#include "NodeAddress.h"
#include "commons.h"
#include "TrackerResponse_m.h"
#include "RequestMessage_m.h"


using namespace std;

class Tracker: public cSimpleModule,
	       public NodeAddress
{
    private:
	int ID;
	//int refreshTime;
	int peersCounter;
	
	PeerBean peers[MAX_PEERS];  //list of peers
	
	void addPeer(double, int, double, char*, char*);
	void delPeer(char*);
	void response(char*);

    public:
	virtual void initialize();
	virtual void handleMessage(cMessage* msg);

};
#endif
