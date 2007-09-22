#include "DataManager.h"

Define_Module(DataManager);


void DataManager::initialize()
{
	connectionEstablished = false;
	strcpy(peerName, par("peer_name"));
}

void DataManager::handleMessage(cMessage *msg)
{	
	NodeMessage* myMsg = NULL; 
	myMsg = check_and_cast<NodeMessage*>(msg);
	
	if(myMsg != NULL)
	{
		if(!connectionEstablished)
		{
			// no message can be processed, if CONNECTION_ESTABLISHED_MESSAGE has not arrived
			if(myMsg->getType() == CONNECTIONS_ESTABLISHED_MSG ){
				// connections with peers were established so set the flag to true and process other messages
				connectionEstablished = true;				
				ev << "data manager understood: connection established\n";
			}
			delete myMsg;
		}
		else
		{
			// connection with peers has been established, messages other than CONNECTIIONS_ESTABLISHED may now be processed
		}
	}
}
