#ifndef DATAMANAGER_H_
#define DATAMANAGER_H_

#include <string>
#include <omnetpp.h>
#include <vector>
#include <cstdlib>
#include "NodeMessage_m.h"
#include "commons.h"
#include "PeerMessageGenerator.h"

using namespace std;

/**
 *
 */
class DataManager : public cSimpleModule,
					public PeerMessageGenerator
{
    private:
    
    	/**
    	 * clas for storing information about peers bitfields
    	 */ 
    class Bitfield
    {
    	private:
    		char peerName[20];
    		char* _bitfield;
    	public:
    		Bitfield(const char* peer_name, char* bitfield)
    		{
    			strcpy(peerName, peer_name);
    			_bitfield = bitfield;
    		}
    		
    		char* getBitfield()
    		{
    			return _bitfield;
    		}
    		
    		char* getPeerName()
    		{
    			return peerName;
    		}
    };
    	
	bool connectionEstablished;
	char peerName[20];
	// number of blocks in a torrent
	unsigned int blocksNumber;
	// bitfield specifying which block is downloaded (completed) and which is not
	char* bitfield;
	
	vector<Bitfield> peersBitfields;
	
	
    public:
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
};

#endif /*DATAMANAGER_H_*/
