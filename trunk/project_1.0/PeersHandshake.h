#ifndef PEERSHANDSHAKE_H_
#define PEERSHANDSHAKE_H_

#endif /*PEERSHANDSHAKE_H_*/

#include <iostream.h>
#include <string.h>

// class for storing handshake message between 2 peers
// see bitTorrent protocol specification on Wiki for reference
class PeersHandshake
{
	public:
		// length of a pstr; set to 19 by default
		int pstrlen;
		
		// name of the protocol; "BitTorrent protocol" by default
		char pstr[20];
		
		//8 reserved bytes; no functionality 
		//but needed for consistency with protocol
		char reserved[8];
		
		// 20-byte SHA1 hash of the info key in the metainfo file. 
		// This is the same info_hash that is transmitted in tracker requests. 
		char infoHash[20];
		
		//20-byte string used as a unique ID for the client;
		// in the simulation, simply name of the node (module 'Node')
		char peerId[20];
		
		
		PeersHandshake(char info_hash[], char peer_id[])
		{
			strcpy(infoHash,info_hash);
			strcpy(peerId,peer_id);
			
			strcpy(pstr,"BitTorrent protocol");
			pstrlen = 19;
		}
		
		PeersHandshake()
		{
			strcpy(pstr,"BitTorrent protocol");
			pstrlen = 19;
		}
};
