#ifndef PEERMESSAGEGENERATOR_H_
#define PEERMESSAGEGENERATOR_H_

#include <string>
#include <omnetpp.h>

#include "commons.h"
#include "P2PMessage_m.h"

using namespace std;

class PeerMessageGenerator
{
	private:
	public:		
		PeerToPeerMessage* generateBitfieldMessage(char* destination, char* sender);
		void generateBitfieldMessage( PeerToPeerMessage* peerMessage, char* bitfield);
		PeerToPeerMessage* generateResponseForBitfieldMessage(PeerToPeerMessage* msgToRespond, char* bitfield, char* sender);
};


#endif /*PEERMESSAGEGENERATOR_H_*/
