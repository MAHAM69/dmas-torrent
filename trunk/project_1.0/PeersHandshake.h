#ifndef PEERSHANDSHAKE_H_
#define PEERSHANDSHAKE_H_

#include <iostream>
#include <string>

/**
 * Class for storing handshake message between 2 peers
 * see bitTorrent protocol specification on Wiki for reference
 */
class PeersHandshake
{
    private:
	/** length of a pstr; set to 19 by default */
	int pstrlen;
		
	/** name of the protocol; "BitTorrent protocol" by default */
	char pstr[20];
		
	/** 8 reserved bytes; no functionality 
	 *  but needed for consistency with protocol
	 */
	char reserved[8];
		
	/** 20-byte SHA1 hash of the info key in the metainfo file. 
	 * This is the same info_hash that is transmitted in tracker requests.
	 */
	char infoHash[20];
		
	/** 20-byte string used as a unique ID for the client;
	 * in the simulation, simply name of the node (module 'Node')
	 */
	char peerId[20];
	
    public:
	PeersHandshake();
	PeersHandshake( char info_hash[], char peer_id[] );
	virtual ~PeersHandshake();

	/** Getter for length of name. */
	int getLength();
	/** Setter for length of name. */
	void setLength( int );

	/** Getter for name. */
	char* getName();
	/** Setter for name. */
	void setName( char* );

	/** Getter for reserved bytes. */
	char* getReserved();
	/** Setter for reserved bytes. */
	void setReserved( char* );

	/** Getter for hash. */
	char* getInfoHash();
	/** Setter for hash. */
	void setInfoHash( char* );

	/** Getter for peer id (name of the node). */
	char* getPeerId();
	/** Setter for peer id (name of the node). */
	void setPeerId( char* );
};

#endif /*PEERSHANDSHAKE_H_*/
