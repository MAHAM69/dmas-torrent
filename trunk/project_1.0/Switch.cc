#include "Switch.h"

Define_Module(Switch);

void Switch::handleMessage(cMessage *msg)
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
