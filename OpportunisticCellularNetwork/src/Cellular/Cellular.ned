package opportunisticcellularnetwork.Cellular;


simple Cellular
{
    parameters:
        int id;
        bool stop         = default(false);
        int packets       = default(0);
        bool typeCQI      = default(false);     //default: uniform
        int uniform_r     = default(0); 		// range for uniform random generator  
        int binomial_n    = default(0); 		// param n for binomial distribution
        double binomial_p = default(0); 		// param p for binomial distribution
    // +-------------------------------------------------------------------------------+
    //  USER Throughput: User's number of bytes received at each timeslot 
    	@signal[userThroughputSignal](type=int);
        @statistic[userThroughput](source="userThroughputSignal"; record=vector,mean;);
    // +-------------------------------------------------------------------------------+    
    // +-------------------------------------------------------------------------------+
    //  Response time
    	@signal[userResponseTimeSignal](type=simtime_t);
        @statistic[userResponseTime](source="userResponseTimeSignal"; record=vector,mean;);
    // +-------------------------------------------------------------------------------+
    	int TEST 	 	  = default(0);
    	int TEST_CQI      = default(0);
    gates:
        input in;
        output out;
}
