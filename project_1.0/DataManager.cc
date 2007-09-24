#include "DataManager.h"

Define_Module(DataManager);


void DataManager::initialize()
{
	// initialize randomization	
	srand((unsigned)time(0) + id()); 
	
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
	
	// at first, copy bitfield to the currentBitfield, as -initially-they are the same
	currentBitfield = new char(blocksNumber +1);
	
	for(unsigned int i=0; i < blocksNumber; i++)
	{
		currentBitfield[i] = bitfield[i];
	}
	
	currentBitfield[blocksNumber] = '\0';
	
	requestsStarted = false;
	
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
					
					// it's possible now to send requests to CM (and CM will resolve them
					// meaning send out to the peer or not)
					if(!requestsStarted)
					{
						scheduleGenerateRequest();
						requestsStarted = true;
					}
				}
			}
			else if (myMsg->getType() == START_REQUESTS)
			{
				int whichBlock= chooseBlock();
				
				//whichBlock==-1 means that no blocks to download left
				if(whichBlock > -1)
				{
					// generate request message
					
					//testing auxillary functions
					
				}
				
								
				scheduleGenerateRequest();
			}
		}
	}
}

void DataManager::scheduleGenerateRequest()
{
	NodeMessage* startRequests = new NodeMessage(); 
	
	startRequests->setType(START_REQUESTS);
	
	scheduleAt(simTime() + 10, startRequests);
}

int DataManager::chooseBlock()
{
	// determine in which blocks client is interested
	char* blocksOfInterest = new char[blocksNumber +1];	
	
	for(unsigned int i =0; i < blocksNumber; i++)
	{
		blocksOfInterest[i] = 'n';
	}
	blocksOfInterest[blocksNumber] = '\0';
	
	
	
	// for testing purposes peersBirfields are changed
	peersBitfields.clear();
	
	int zeroElements = peersBitfields.size();
	
	char bitf1[20];
	
	for(int i=0; i< 20; i++)
		bitf1[i]='y';
	
	Bitfield* ex1 = new Bitfield("exxamplary",bitf1);
	peersBitfields.push_back(*ex1);
		
	
	for(int i=0; i<20; i++)
		currentBitfield[i] = 'y';
	
	
	// for each bitfield among peersBitfields
	for(unsigned int j =0; j < peersBitfields.size(); j++)
		// determine which block is available by comparing peers bitfields (so what is available)  
		// with what client has (currentBitfield), so that it determines 		
		// which blocks client is really interested in and set blocksOfInterest to yes
		for(unsigned int l=0; l < blocksNumber; l++)
		{
			char alpha = peersBitfields[j].getBitfield()[l];
			char beta = currentBitfield[l];
			if( peersBitfields[j].getBitfield()[l] ==  'y' && currentBitfield[l] == 'n')
				blocksOfInterest[l] = 'y';			
		}	
	
	
	
	unsigned int howManyBlocksOfInterest= 0;
	
	for(unsigned int i=0;i < blocksNumber; i++)
	{
		if(blocksOfInterest[i] == 'y')
			howManyBlocksOfInterest++;
	}
	
	if(howManyBlocksOfInterest > 0){
		
		
		// choose randomly which block to download
		int whichBlock = rand() % howManyBlocksOfInterest;
		
		// determine number of a block corresponding to the chosen 'whichBlock' 
		// in an array of blocks
		unsigned int blockNumber = 0;
		
		
		for(blockNumber=0; blockNumber < blocksNumber && whichBlock >= 0; blockNumber++)
		{
			if( blocksOfInterest[blockNumber] == 'y' )
			{
				whichBlock--;			
			}			
		}
		
		blockNumber--;
		
		delete []blocksOfInterest;
		return blockNumber;
	}
	else
	{
		delete []blocksOfInterest;
		return -1;
	}
	
}
