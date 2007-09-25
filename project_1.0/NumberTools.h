#ifndef NUMBERTOOLS_H_
#define NUMBERTOOLS_H_

#include <string>
#include <sstream>
#include <iostream>
#include <ctime>

using namespace std;

/**
 * @author: Alek Malaszkiewicz
 */
class cNumberTools
{
private:
protected:
public:
    cNumberTools();
    virtual ~cNumberTools();
    int str2int( string );
    float str2float( string );
    string int2str( int, int );
    int getRandomInt( int );
    float getRandomFloat( float, int );
};

#endif /*NUMBERTOOLS_H_*/
