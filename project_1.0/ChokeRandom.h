#ifndef CHOKE_RANDOM
#define CHOKE_RANDOM

#include "Choke.h"
#include "commons.h"

//probability, that request will be accepted - in %
const int probAccept = 50;

class ChokeRandom :
		    public Choke
{
    public:
	ChokeRandom() {}
	virtual ~ChokeRandom() {}
	
	bool choked();

};

#endif
