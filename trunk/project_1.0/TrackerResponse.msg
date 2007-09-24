cplusplus {{
#include "PeerBean.h"
#include "commons.h"
}};

class noncobject PeerBean;

message TrackerResponse
{
    fields:
	double interval;
	int no_seeders;
	int no_leechers;
	PeerBean peers[NO_PEERS];
	
}