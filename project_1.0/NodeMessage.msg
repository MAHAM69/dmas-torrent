// handshake message between nodes (peers) 

message NodeMessage
{
    fields:    	
    	// destination and type are fields necessary 
    	// for simulation purposes only; no torrent protocol here
    	string destination;
		unsigned int type;
}
