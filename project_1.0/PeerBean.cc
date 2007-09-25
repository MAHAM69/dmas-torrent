#include "PeerBean.h"

PeerBean::PeerBean()
{
    setHash( "" );
    setID( "" );
    setIp( 0.0 );
    setPort( 0 );
    setLeft( 0.0 );
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

void PeerBean::setHash( const char* hash )
{
    strcpy( this->hash, hash );
}

char* PeerBean::getID()
{
    return ID;
}

void PeerBean::setID( const char* ID )
{
    strcpy( this->ID, ID );
}

double PeerBean::getIp()
{
    return ip;
}

void PeerBean::setIp( const double ip )
{
    this->ip = ip;
}

int PeerBean::getPort()
{
    return port;
}

void PeerBean::setPort( const int port )
{
    this->port = port;
}

double PeerBean::getLeft()
{
    return left;
}

void PeerBean::setLeft( const double left )
{
    this->left = left;
}

PeerBean& PeerBean::operator =(const PeerBean& old)
{
    PeerBean *tmp = new PeerBean();
    tmp->setHash( old.hash );
    tmp->setID( old.ID );
    tmp->setIp( old.ip );
    tmp->setPort( old.port );
    tmp->setLeft( old.left );
    return *tmp;
}
