#include "ChokeRandom.h"

bool ChokeRandom::choked()
{
    
    if (getRandomInt(100) < probAccept)
	return true;
    else
	return false;
}
