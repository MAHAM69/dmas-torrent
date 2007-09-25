cplusplus {{
#include "PeerBean.h"
#include "NodeMessage_m.h"
#include "commons.h"
}};

class noncobject PeerBean;
class NodeMessage;

message TrackerResponse extends NodeMessage
{
    fields:
	double interval;
	int no_seeders;
	int no_leechers;
	PeerBean peers[NO_PEERS];
	
}