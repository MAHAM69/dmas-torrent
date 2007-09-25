#ifndef TRACKER_H
#define TRACKER_H

#include <omnetpp.h>
#include <sstream>
#include "PeerBean.h"
#include "NodeAddress.h"
#include "CVSParser.h"
#include "StringConvert.h"
#include "commons.h"
#include "TrackerResponse_m.h"
#include "RequestMessage_m.h"

using namespace std;

class Tracker : public cSimpleModule,
		public NodeAddress,
	        public CVSParser,
	        public StringConvert
{
    private:
	int ID;
	//int refreshTime;
	int peersCounter;
	
	PeerBean peers[MAX_PEERS];  //list of peers
	
	void addPeer( const double, const int, const double, const char*, const char* );
	void delPeer( const char* );
	void response( const char* );

    public:
	virtual void initialize();
	virtual void handleMessage(cMessage* msg);

};
#endif
