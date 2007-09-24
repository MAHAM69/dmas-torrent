#ifndef CHOKE_RANDOM
#define CHOKE_RANDOM

#include "NumberTools.h"

//probability, that request will be accepted - in %
const int probAccept = 50;

class ChokeRandom :
		    public cNumberTools
{
    public:
	ChokeRandom(){};
	~ChokeRandom(){};
	
	bool choked();

};

#endif
