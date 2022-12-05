package opportunisticcellularnetwork;


 // A source models an entity that forwards packets either from internal or esternal cell
 
simple Source

{ 
 	parameters:
 	    int id;
 	    
 	    // uniform or exp delay 
 	    bool isDelayUniform = default(false);
 	    
 	    // max packet dimension
    	int maxPacketSize = 75;
    	
    	// rate between consecutive packets arrivals
    	double exponentialMean @unit(s) = 5ms;

   gates:
        output out;
}
