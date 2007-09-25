#include "PeerMessageGenerator.h"
#include <string>


/**
 * generates empty bitfield message
 */ 
PeerToPeerMessage* PeerMessageGenerator::generateBitfieldMessage(char* destination, char* sender)
{
	PeerToPeerMessage* peerMessage = new PeerToPeerMessage("Bitfield message");
	
	// '5' in bitTorrent spec corresponds to Bitfield message  
	peerMessage->setMessageId(5);
	peerMessage->setDestination(destination);
	// set bitfield as self message so it can be sent from connection manager to data manager
	peerMessage->setType(MSG_SELF_BITFIELD);
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
	peerMessage->setType(MSG_BITFIELD);
}

PeerToPeerMessage* PeerMessageGenerator::generateResponseForBitfieldMessage(PeerToPeerMessage* msgToRespond, char* bitfield, char* sender)
{
	PeerToPeerMessage* responseMsg = new PeerToPeerMessage("Bitfield response");
	
	unsigned int bitfieldLen = strlen(bitfield);
	
	responseMsg->setLength(bitfieldLen+1);
	
	responseMsg->setDestination(msgToRespond->getSender());
	
	responseMsg->setType(MSG_BITFIELD_RESPONSE);
	
	responseMsg->setSender(sender);
	
	responseMsg->setPayloadArraySize(bitfieldLen);
	
	for(unsigned int i=0;i<bitfieldLen;i++)
	{
		responseMsg->setPayload(i,bitfield[i]);
	}
	
	responseMsg->setMessageId(5);
	
	return responseMsg;
}


PeerToPeerMessage* PeerMessageGenerator::generateRequestMessage(char* destination, char* sender, char* payload)
{
	PeerToPeerMessage* peerMsg = new PeerToPeerMessage("Request");
	
	peerMsg->setDestination(destination);
	peerMsg->setSender(sender);
	
	peerMsg->setType(MSG_REQUEST);
	peerMsg->setMessageId(6);
	peerMsg->setLength(13);
		
	peerMsg->setPayloadArraySize(12);
	for(int i =0; i<12; i++)
		peerMsg->setPayload(i,payload[i]);
	
	return peerMsg;
}

PeerToPeerMessage* PeerMessageGenerator::generatePieceMessage(char* destination, char* sender, char* payload, unsigned int payloadLen){
	PeerToPeerMessage* pieceMsg = new PeerToPeerMessage("Piece");
	
	pieceMsg->setMessageId(7);
	pieceMsg->setDestination(destination);
	pieceMsg->setSender(sender);
	pieceMsg->setLength(payloadLen+1);
	pieceMsg->setType(MSG_PIECE);
	
	pieceMsg->setPayloadArraySize(payloadLen);
	
	for(int i =0;i<payloadLen;i++)
		pieceMsg->setPayload(i,payload[i]);
	
	return pieceMsg;
}

