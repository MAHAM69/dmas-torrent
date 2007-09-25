#ifndef PEERMESSAGEGENERATOR_H_
#define PEERMESSAGEGENERATOR_H_

#include <string>
#include <omnetpp.h>
#include <cstdlib>

#include "commons.h"
#include "P2PMessage_m.h"
#include "NumberTools.h"

using namespace std;

class PeerMessageGenerator : public cNumberTools
{
	private:
		
	public:		
		PeerToPeerMessage* generateBitfieldMessage(char* destination, char* sender);
		void generateBitfieldMessage( PeerToPeerMessage* peerMessage, char* bitfield);
		PeerToPeerMessage* generateResponseForBitfieldMessage(PeerToPeerMessage* msgToRespond, char* bitfield, char* sender);
		
		PeerToPeerMessage* generateRequestMessage(char* destnation, char* sender, char* payload);
		PeerToPeerMessage* generatePieceMessage(char* destination, char* sender, char* payload, unsigned int payloadLen);
};


#endif /*PEERMESSAGEGENERATOR_H_*/
