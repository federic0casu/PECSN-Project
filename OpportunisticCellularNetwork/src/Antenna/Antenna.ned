package opportunisticcellularnetwork.Antenna;

simple Antenna
{
    parameters:
        int stage       = default(0);
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
        @statistic[throughput](source="throughputSignal"; record=vector,mean;);
    // +-------------------------------------------------------------------------------+
    // +-------------------------------------------------------------------------------+
    //  Backologged Packets 
    	@signal[backloggedPacketsSignal](type=int);
        @statistic[backloggedPackets](source="backloggedPacketsSignal"; record=vector,mean;);
    // +-------------------------------------------------------------------------------+
    gates:
        input  inCellular[population];
        input  inSource[population];
        output out[population];
}