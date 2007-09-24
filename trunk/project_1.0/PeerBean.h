#ifndef PEERBEAN_H
#define PEERBEAN_H

#include <string>

using namespace std;

/**
 *
 */
class PeerBean {
private:
    char hash[20];
    char ID[20];
    double ip;
    int port;
    double left;

public:
    PeerBean( );
    PeerBean( PeerBean* );
    virtual ~PeerBean();
    char* getHash();
    void setHash( char* );
    char* getID();
    void setID( char* );
    double getIp();
    void setIp( double );
    int getPort();
    void setPort( int );
    double getLeft();
    void setLeft( double );
};

#endif /**PEERBEAN_H_*/
