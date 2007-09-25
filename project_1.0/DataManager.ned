simple DataManager
    parameters:
	peerId : numeric,
	peer_name : string;
		
    gates:
	in: connectionManagerIn;
	in: nodeManagerIn;
	
	out: connectionManagerOut;
	out: nodeManagerOut;	
endsimple
