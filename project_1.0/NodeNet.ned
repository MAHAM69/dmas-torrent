import
    "Node",
    "Switch",
    "Tracker";

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
		tracker: Tracker;
			display: "i=misc/building;";
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
		
		tracker.trackerOut --> switch.in++;
		tracker.trackerIn <-- switch.out++;

endmodule

network examplaryNode : NodeNet
endnetwork