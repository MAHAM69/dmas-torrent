#ifndef SINGLEACTIVE_H_
#define SINGLEACTIVE_H_

#include <vector>
#include <iostream>

using namespace std;

/**
 * Class represents single active upload or download connection, listend in 'ActiveConnections'
 */
class SingleActive
{
    private:
	char peerName[20];

    public:
	SingleActive( char* peer_name );
};

#endif /*SINGLEACTIVE_H_*/
