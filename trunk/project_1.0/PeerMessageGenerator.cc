#include "PeerMessageGenerator.h"
#include <string>


/**
 * generates empty bitfield message
 */ 
PeerToPeerMessage* PeerMessageGenerator::generateBitfieldMessage(char* destination, char* sender)
{
	PeerToPeerMessage* peerMessage = new PeerToPeerMessage();
	
	// '5' in bitTorrent spec corresponds to Bitfield message  
	peerMessage->setMessageId(5);
	peerMessage->setDestination(destination);
	// set bitfield as self message so it can be sent from connection manager to data manager
	peerMessage->setType(SELF_BITFIELD_MSG);
	peerMessage->setSender(sender);
	
	return peerMessage;
}

/**
 * adds payload (bitfield) to the bitfield-message
 */ 
void PeerMessageGenerator::generateBitfieldMessage( PeerToPeerMessage* peerMessage, char* bitfield)		
{		
	// copying bitfield to a message payload
	unsigned int bitfieldLen =strlen(bitfield);
	peerMessage->setPayloadArraySize(bitfieldLen);
	
	for(unsigned int i=0; i< bitfieldLen; i++){
		peerMessage->setPayload(i,bitfield[i]);
	}
	
	// setting bitfield size "1 + length of payload"; 1 is reserved fro '/0', torrent spec does not specify i precisely
	peerMessage->setLength(bitfieldLen+1);
	
	// change type to bitfield, so the message can be sent out to external node
	peerMessage->setType(BITFIELD_MSG);
}

PeerToPeerMessage* PeerMessageGenerator::generateResponseForBitfieldMessage(PeerToPeerMessage* msgToRespond, char* bitfield, char* sender)
{
	PeerToPeerMessage* responseMsg = new PeerToPeerMessage();
	
	unsigned int bitfieldLen = strlen(bitfield);
	
	responseMsg->setLength(bitfieldLen+1);
	
	responseMsg->setDestination(msgToRespond->getSender());
	
	responseMsg->setType(BITFIELD_RESPONSE);
	
	responseMsg->setSender(sender);
	
	responseMsg->setPayloadArraySize(bitfieldLen);
	
	for(unsigned int i=0;i<bitfieldLen;i++)
	{
		responseMsg->setPayload(i,bitfield[i]);
	}
	
	responseMsg->setMessageId(5);
	
	return responseMsg;
}

PeerToPeerMessage* PeerMessageGenerator::generateRequestMessage(char* destination, char* sender, int blockIndex, int offset, int pieceLength)
{
	PeerToPeerMessage* peerMsg = new PeerToPeerMessage();
	
	peerMsg->setType(REQUEST_MSG);
	peerMsg->setMessageId(6);
	peerMsg->setLength(13);
	
	peerMsg->setDestination(destination);
	peerMsg->setSender(sender);
	
	//char* payload = requestIntToChar(blockIndex,offset,pieceLength);
		
	char payload[12];
	peerMsg->setPayloadArraySize(12);
	for(int i =0; i<12; i++)
		peerMsg->setPayload(i,payload[i]);
	
	return peerMsg;
}
