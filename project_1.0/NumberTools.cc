#include "NumberTools.h"

cNumberTools::cNumberTools()
{
}

cNumberTools::~cNumberTools()
{
}

int cNumberTools::str2int( string s )
{
    istringstream in;
    in.str( s );
    int value;
    in >> value;
    return value;
}

float cNumberTools::str2float( string s )
{
    string::size_type dot = s.find( ",", 0 );
    if ( (int)dot > 0 )
    {
	s = s.substr( 0, dot ) + "." + s.substr( dot + 1, s.length() - dot );
    }
    istringstream in;
    in.str( s );
    float value;
    in >> value;
    return value;
}

string cNumberTools::int2str( int value, int numOfDigits )
{
    ostringstream out;
    out << value;
    string s = out.str();
    while ( s.length() < numOfDigits )
    {
	s = "0" + s;
    }
    return s;
}

int cNumberTools::getRandomInt( int limit )
{
    return rand() % limit;
}

float cNumberTools::getRandomFloat( float limit, int precision)
{
    float fraction = (float)rand() / (float)rand();
    float multiplier = 1.0;
    for ( int i = 0; i < precision; i++ ) 
    {
	multiplier*=10.0;
    }
    int number = (int)( fraction*multiplier ) % (int)( limit*multiplier );
    fraction = (float)number / multiplier;
    return fraction;
}
