import
    "ConnectionManager",
    "DataManager",
    "NodeManager";
    
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
	    display:
		"i=block/process,cyan";

	nodeManager: NodeManager            
	    parameters:
		peer_name=node_name;
	    display: 
		"i=block/process,gold";
            
	connectionManager: ConnectionManager
	    parameters:
		peer_name = node_name;
	    display:
		"i=block/process,gold";

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
