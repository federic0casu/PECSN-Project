package opportunisticcellularnetwork;


simple Antenna
{
    parameters:
        int stage = default(0);
        int population;
        double timeslot @unit(s);
        int maxFrameDim = default(25);
    // +-------------------------------------------------------------------------------+
    //  Scenario: queues of fixed dimension
    	int queueDimension;
    // +-------------------------------------------------------------------------------+
    // +-------------------------------------------------------------------------------+
    //  GENERAL Throughput: TOTAL number of bytes sent at each timeslot 
    	@signal[throughputSignal](type=int);
        @statistic[throughput](source="throughputSignal"; record=vector;);
    // +-------------------------------------------------------------------------------+
    //  ResponseTime 
    	@signal[responseTimeSignal](type=simtime_t);
        @statistic[responseTime](source="responseTimeSignal"; record=vector;);
    // +-------------------------------------------------------------------------------+
    gates:
        input  inCellular[population];
        input  inSource[population];
        output out[population];
}
