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
    void setHash( const char* );
    char* getID();
    void setID( const char* );
    double getIp();
    void setIp( const double );
    int getPort();
    void setPort( const int );
    double getLeft();
    void setLeft( const double );
    PeerBean& operator =(const PeerBean&);
};

#endif /**PEERBEAN_H_*/
