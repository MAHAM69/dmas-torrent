#include "Switch.h"

Define_Module(Switch);

void Switch::handleMessage(cMessage *msg)
{
#ifdef DEBUG
	ev << "Switch::handleMessage    message name: " << ((msg != NULL) ? msg->name() : "NULL") << endl;
#endif

	if ( strcmp( msg->name(), MSG_GET ) == 0)
	{
	    send(msg, "out", 0);
	} 
	else if ( strcmp( msg->name(), MSG_TRACKER_RESPONSE ) == 0 )
	{
	    TrackerResponse* myMsg = NULL;
	    myMsg = check_and_cast<TrackerResponse *>(msg);
	    const char* dest = myMsg->getDestination();
	    char bubel[50];
	    strcpy(bubel, "tracker response to\n");
	    strcat(bubel, myMsg->getDestination());
	    bubble(bubel);

	    // check which gate corresponds to the receiver
	    int port = nodeAddress(dest);
	    send( msg, "out", port );
	    
	}
	else
	{
	    NodeMessage* myMsg = NULL;
	    myMsg = check_and_cast<NodeMessage *>(msg);
	    if(myMsg != NULL)
	    {
		const char* dest = myMsg->getDestination();
		
		char bubel[50];
		
		strcpy(bubel, "sending to\n");
		strcat(bubel, myMsg->getDestination());
		
		bubble(bubel);
		
		// check which gate corresponds to the receiver
		int port = nodeAddress(dest);		
		send(myMsg,"out",port);
	    }
	}
}
