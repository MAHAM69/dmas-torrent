#ifndef CHOKE_H
#define CHOKE_H

#include "NumberTools.h"

//abstrakcyjna klasa Choke - z niej dziedzicza
//klasy implementujace algorytmy Choke
class Choke :
	public cNumberTools
{
    public:
    
    virtual bool choked() = 0;
};

#endif
