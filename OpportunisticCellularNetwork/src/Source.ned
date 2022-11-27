<<<<<<< HEAD
package opportunisticcellularnetwork;
=======
package opportunisticCellularNetwork;
>>>>>>> ff6c992a6123815f6068a2bef014605660aa7996

 // A source models an entity that forwards packets either from internal or esternal cell
 
simple Source

{ 
 	parameters:
 	    // uniform or exp delay 
 	    bool isDelayUniform = default(true);
 	    
 	    // max packet dimension
    	int maxPacketSize = 75;
    	
    	// max delay between consecutive packets arrivals  
    	double maxDelay @unit(ms) = 100ms;
    	
    	// rate between consecutive packets arrivals
    	double exponentialMean = default(0.5);

   gates:
        output out;
}
