#ifndef SWITCH_H_
#define SWITCH_H_

#include <string>
#include <omnetpp.h>
#include "NodeMessage_m.h"
#include "TrackerResponse_m.h"
#include "NodeAddress.h"
#include "commons.h"

using namespace std;

// switch passes messages to the intended node
class Switch :	public cSimpleModule,
		public NodeAddress
{
    protected:			
	virtual void handleMessage(cMessage *msg);
};

#endif /*SWITCH_H_*/
