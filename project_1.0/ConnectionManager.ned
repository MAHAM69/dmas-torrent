simple ConnectionManager
    parameters:
	peerId : numeric,
	peer_name : string;
		
    gates:
	in: dataManagerIn;
	in: nodeManagerIn;
	in: nodeIn;
	out: dataManagerOut;
	out: nodeManagerOut;
	out: nodeOut;
endsimple
