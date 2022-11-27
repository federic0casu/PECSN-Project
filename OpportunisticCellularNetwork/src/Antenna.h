#ifndef __OPPORTUNISTICCELLULARNETWORK_ANTENNA_H_
#define __OPPORTUNISTICCELLULARNETWORK_ANTENNA_H_

#include <algorithm>
#include <omnetpp.h>
#include <vector>
#include "CQIPacket.h"
#include "Frame.h"
#include "UserQueue.h"

using namespace omnetpp;

namespace opportunisticcellularnetwork {

/*
 * +-------------------------------------------------------------------------------+
 * | Implements the Antenna simple module. See the NED file for more information.  |
 * +-------------------------------------------------------------------------------+
 */

class Antenna : public cSimpleModule
{
  private:
    int population;
    simtime_t timeslot;
    std::vector<UserQueue*> userQueues;
    std::vector<CQIPacket*> CQIs;
// +-------------------------------------------------------------------------------+
//  Scenario: queues of fixed dimension.
    int userQueueDimension;
// +-------------------------------------------------------------------------------+
// +-------------------------------------------------------------------------------+
//  Statistics
    simsignal_t throughputSignal;
    int sentPackets;
    int lostPackets;
// +-------------------------------------------------------------------------------+
    virtual int queuedBytesById(int);
    virtual int CQI_to_BYTES(int);
    virtual UserQueue* getQueueById(int);
    virtual int allocateRBs(std::vector<std::pair<simtime_t,int>>*, Frame*, int, int);
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
    virtual void handleCQI(int, int);
    virtual void handleFrame();
};

}

#endif
