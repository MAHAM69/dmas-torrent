#include "PeersHandshake.h"

PeersHandshake::PeersHandshake()
{
    strcpy( this->pstr, "BitTorrent protocol" );
    this->pstrlen = 19;
}

PeersHandshake::PeersHandshake( char info_hash[], char peer_id[] )
{
    strcpy( this->infoHash, info_hash );
    strcpy( this->peerId, peer_id );
			
    strcpy( this->pstr,"BitTorrent protocol");
    this->pstrlen = 19;
}

PeersHandshake::~PeersHandshake()
{
}
		
int PeersHandshake::getLength()
{
    return pstrlen;
}

void PeersHandshake::setLength( int length )
{
    this->pstrlen = length;
}

char* PeersHandshake::getName()
{
    return pstr;
}

void PeersHandshake::setName( char* name )
{
    strcpy( this->pstr, name );
}

char* PeersHandshake::getReserved()
{
    return reserved;
}

void PeersHandshake::setReserved( char* reserved )
{
    strcpy( this->reserved, reserved );
}

char* PeersHandshake::getInfoHash()
{
    return infoHash;
}

void PeersHandshake::setInfoHash( char* infoHash )
{
    strcpy( this->infoHash, infoHash );
}

char* PeersHandshake::getPeerId()
{
    return peerId;
}

void PeersHandshake::setPeerId( char* peerId )
{
    strcpy( this->peerId, peerId );
}
