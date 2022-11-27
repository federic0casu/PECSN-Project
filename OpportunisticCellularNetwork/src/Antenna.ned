package opportunisticcellularnetwork;


simple Antenna
{
    parameters:
        int stage = default(0);
        int population;
        int timeslot;
    // +-------------------------------------------------------------------------------+
    //  Scenario: queues of fixed dimension.
    	int queueDimension;
    // +-------------------------------------------------------------------------------+
    // +-------------------------------------------------------------------------------+
    //  Throughput: number of bytes sent at each timeslot
    	@signal[throughputSignal](type=int);
        @statistic[throughput](source="throughputSignal"; record=vector;);
    // +-------------------------------------------------------------------------------+
    gates:
        input in;
        output out;
}
