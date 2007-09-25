#include "CVSParser.h"

CVSParser::CVSParser()
{
}

CVSParser::~CVSParser()
{
}

void CVSParser::parse( string s, string& value, string& rest )
{
    string::size_type separator;
    separator = s.find( ";", 0 );
    if ( (int)separator > 0 )
    {
	value = s.substr( 0, separator );
	rest  = s.substr( separator + 1, s.length() - separator );
    }
    else
    {
	value = s;
	rest = "";
    }
}
