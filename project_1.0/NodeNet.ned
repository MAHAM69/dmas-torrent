import
    "Node",
    "Tracker",
    "Switch";

module NodeNet
	parameters:
		numOfNodes: numeric const;
	submodules:
		tracker: Tracker;
			display: "i=misc/building;";
		node: Node[ numOfNodes ];
			parameters:
			    id = index,
			    node_name = "node";
			display: "i=device/pc2;";
		switch : Switch;
			gatesizes:
			    in[ numOfNodes+1 ],
			    out[ numOfNodes+1 ];
			display: "i=misc/cloud;";

	connections:		
		tracker.trackerOut --> switch.in[0];
		tracker.trackerIn <-- switch.out[0];

		for i=0..numOfNodes-1 do
		    node[ i ].out  --> switch.in[ i+1 ];
		    node[ i ].in  <--  switch.out[ i+1 ];
		endfor;
endmodule

network examplaryNode : NodeNet
    parameters:
	numOfNodes	= input( 4, "Number of nodes?" );
endnetwork
