#ifndef DATAMANAGER_H_
#define DATAMANAGER_H_

#include <string>
#include <omnetpp.h>
#include <vector>
#include <cstdlib>
#include <ctime>
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
    
    class SingleBlock
    {
    private:
    	unsigned int blockNum;
    	unsigned int blockSize;
    	// offset determines how much data is already downloaded
    	unsigned int offset;
    	
    	
    public:
    	SingleBlock(unsigned int block_num,unsigned int block_size): 
    		blockNum(block_num), blockSize(block_size)
    	{
    		offset = 0;
    	}
    	
    	unsigned int getBlockNumber(){
    		return blockNum;
    	}
    	
    	unsigned int getBlockSize(){
    		return blockSize;
    	}
    	unsigned int getOffset(){
    		return offset;
    	}
    };
    	
    /**
     * method chooses randomly which block to download from available ones
     */ 
    int chooseBlock();
    
    /**
     * schedules request for block generation
     */
    void scheduleGenerateRequest();
    
	bool connectionEstablished;
	char peerName[20];
	// number of blocks in a torrent
	unsigned int blocksNumber;
	// bitfield specifying which block is downloaded (completed) and which is not
	char* bitfield;
	
	// same as bitfield, except it also determines also which blocks are being downloaded
	// so that the DM does not request twice for the same block
	char* currentBitfield;
	
	vector<Bitfield> peersBitfields;
	
	// structure determining which blocks are downloaded and in what part (how much data)
	vector<SingleBlock> blocks; 
	
	// determines if requests generation has started or not 
	bool requestsStarted;
	
    public:
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
};

#endif /*DATAMANAGER_H_*/
