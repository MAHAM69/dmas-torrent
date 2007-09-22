//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003 Ahmet Sekercioglu
// Copyright (C) 2003-2005 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


simple NodeManager
	
	parameters:
		peer_name : string;

    gates:
        in: dataManagerIn;
        in: connectionManagerIn;

		out: dataManagerOut;
       	out: connectionManagerOut;
endsimple


simple DataManager
	parameters:
		peer_name : string;
		
    gates:
	in: connectionManagerIn;
	in: nodeManagerIn;
	
	out: connectionManagerOut;
	out: nodeManagerOut;	
endsimple


simple ConnectionManager
	parameters:
		peer_name : string;
		
    gates:
	in: dataManagerIn;
	in: nodeManagerIn;
	in: nodeIn;
	out: dataManagerOut;
	out: nodeManagerOut;
	out: nodeOut;
endsimple


simple Switch
    gates:
        in: in[];
        out: out[];
endsimple


//
// Here we make the model look a bit prettier in the GUI. We assign
// the "proc1" icon (bitmaps/proc1.gif), and paint it cyan for `tic'
// and yellow for `toc'.
//
module Node
	parameters:
		node_name : string;
		
	gates:
	    in: in;
	    out: out;
	
				
    submodules:
        dataManager: DataManager        	            
            parameters:
            	peer_name = node_name;
            display: "i=block/process,cyan";
            
        nodeManager: NodeManager            
            parameters:
            	peer_name=node_name;
            display: "i=block/process,gold";
            
		connectionManager: ConnectionManager
	    	parameters:
	    		peer_name = node_name;
	    	display: "i=block/process,gold";

    connections:
	nodeManager.dataManagerOut --> dataManager.nodeManagerIn;
	nodeManager.dataManagerIn <-- dataManager.nodeManagerOut;

	nodeManager.connectionManagerOut --> connectionManager.nodeManagerIn;
	nodeManager.connectionManagerIn <-- connectionManager.nodeManagerOut;

	dataManager.connectionManagerOut --> connectionManager.dataManagerIn;
	dataManager.connectionManagerIn <-- connectionManager.dataManagerOut;
	
	out <-- connectionManager.nodeOut;	
	in --> connectionManager.nodeIn;
	
endmodule

module NodeNet
	submodules:
		node1: Node;
			display: "i=device/pc2;";
		node2: Node;
			display: "i=device/pc2;";
		node3: Node;			
			display: "i=device/pc2;";
		node4: Node;
			display: "i=device/pc2;";
		switch : Switch;
			display: "i=misc/cloud;";
	
	connections:		
		//node1.out --> node2.in;
		//node1.in <-- node2.out;
		node1.out --> switch.in++;
		node1.in <-- switch.out++;
		
		node2.out --> switch.in++;
		node2.in <-- switch.out++;
		
		node3.out --> switch.in++;
		node3.in <-- switch.out++;
		
		node4.out --> switch.in++;
		node4.in <-- switch.out++;
		 
		

endmodule

network examplaryNode : NodeNet
endnetwork

