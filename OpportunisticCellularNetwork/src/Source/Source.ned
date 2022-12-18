package opportunisticcellularnetwork.Source;


 // A source models an entity that forwards packets either from internal or esternal cell
 
simple Source

{ 
 	parameters:
 	    int id;
 	    
 	    // uniform or exp delay 
 	    bool   isDelayUniform           = default(false);
 	    
 	    // max packet dimension
    	int    maxPacketSize = 75;
    	
    	double RATE                     = default(1);

    	int    TEST                     = default(0);
    	double TEST_PERIOD @unit(s)     = default(0s);
    	int    TEST_SIZE                = default(0);
    	
   gates:
        output out;
}
