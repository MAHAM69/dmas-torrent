#include "PeerBean.h"

PeerBean::PeerBean()
{
    
}

PeerBean::PeerBean( PeerBean* peerBean )
{
    setHash( peerBean->getHash() );
    setID( peerBean->getID() );
    setIp( peerBean->getIp() );
    setPort( peerBean->getPort() );
    setLeft( peerBean->getLeft() );
}

PeerBean::~PeerBean()
{
}

char* PeerBean::getHash()
{
    return hash;
}

void PeerBean::setHash( char* hash )
{
    strcpy( this->hash, hash );
}

char* PeerBean::getID()
{
    return ID;
}

void PeerBean::setID( char* ID )
{
    strcpy( this->ID, ID );
}

double PeerBean::getIp()
{
    return ip;
}

void PeerBean::setIp( double ip )
{
    this->ip = ip;
}

int PeerBean::getPort()
{
    return port;
}

void PeerBean::setPort( int port )
{
    this->port = port;
}

double PeerBean::getLeft()
{
    return left;
}

void PeerBean::setLeft( double left )
{
    this->left = left;
}