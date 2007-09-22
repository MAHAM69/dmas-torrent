#ifndef DATAMANAGER_H_
#define DATAMANAGER_H_

#include <string>
#include <omnetpp.h>
#include "NodeMessage_m.h"

#include "connection.h"
#include "commons.h"

using namespace std;

/**
 *
 */
class DataManager : public cSimpleModule
{
    private:
	bool connectionEstablished;
	char peerName[20];
    public:
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
};

#endif /*DATAMANAGER_H_*/
