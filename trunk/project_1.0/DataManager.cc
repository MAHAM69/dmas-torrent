#include "DataManager.h"

Define_Module(DataManager);


void DataManager::initialize()
{
	connectionEstablished = false;
	strcpy(peerName, par("peer_name"));
	// here be receiving number of blocks of torrent from node manager 
	// for testing purposes only, number of blocks set to 20
	blocksNumber = 20;
	bitfield = new char(blocksNumber + 1);
	
	// randomly generate which block is completed and which is not
	for(unsigned int i =0; i < blocksNumber; i++)
	{		
		if ( (rand() %2 ) == 1)
			bitfield[i] = 'n';
		else
			bitfield[i] = 'y';
	}
	bitfield[blocksNumber] = '\0';	
	
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
				ev << peerName << " data manager understood: connection established with " << myMsg->getDestination() << endl;
			}
			delete myMsg;
		}
		else
		{
			// connection with peers has been established, messages other than CONNECTIIONS_ESTABLISHED may now be processed
			if(myMsg->getType() == SELF_BITFIELD_MSG)
			{
				PeerToPeerMessage* bitfieldMsg = check_and_cast<PeerToPeerMessage*>(myMsg);
				
				int alpha = strlen(bitfield);
				
				ev << peerName << "bitfield length: " << strlen(bitfield) << endl;
				
				generateBitfieldMessage(bitfieldMsg,bitfield);
				send(bitfieldMsg,"connectionManagerOut");
			}
			else if (myMsg->getType() == BITFIELD_MSG || myMsg->getType() == BITFIELD_RESPONSE)
			{
				
				PeerToPeerMessage* peerMsg = check_and_cast<PeerToPeerMessage*> (myMsg);
				
				// check if peers bitfield size equals expected bitfield size (client's bitfield size)
				// that is: message length -1 == bitfield size, because 1byte is for message overhead  
				unsigned int peersBitfieldSize =   peerMsg->getLength();
				if( (peersBitfieldSize -1 )== blocksNumber)
				{
					char* payload = new char(blocksNumber+1);
					
					for(unsigned int i =0; i< blocksNumber; i++)
					{
						payload[i] = peerMsg->getPayload(i);
					}
					payload[blocksNumber] = '\0';
					
					Bitfield* bitfield = new Bitfield(peerMsg->getSender(),payload);
					peersBitfields.push_back(*bitfield);
					
					ev << peerName << " data manager successfully added bitfield from " << peerMsg->getSender(); 
					
					if(myMsg->getType() == BITFIELD_MSG)
						ev << " received bitfield\n";
					else if (myMsg->getType() == BITFIELD_RESPONSE)
						ev << " received response for bitfield\n";
					
					for(unsigned int i=0; i<blocksNumber;i++)
						ev << payload[i] << " ";
					ev << endl;
					
					if(peerMsg->getType() == BITFIELD_MSG )
					{
						// message was bitfield message, so generate response for bitifield message and send  it back
						PeerToPeerMessage* responseMsg= generateResponseForBitfieldMessage(peerMsg,this->bitfield,peerName);
						
						send(responseMsg,"connectionManagerOut");
					}
				}
			}
		}
	}
}
