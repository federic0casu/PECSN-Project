namespace opportunisticcellularnetwork;

message RBsPacket {
    int destinationUser;
    int usedRBs;
    int usedBytes;
    simtime_t arrivalTimes[];
}
