#ifndef __OPPORTUNISTICCELLULARNETWORK_ANTENNA_H_
#define __OPPORTUNISTICCELLULARNETWORK_ANTENNA_H_

#include <algorithm>
#include <omnetpp.h>
#include <vector>

// message types
#include "../Messages/CQI_m.h"
#include "../Messages/Packet_m.h"
#include "../Messages/RBsPacket_m.h"

#include "../Utility/CQIPacket.h"
#include "../Utility/UserQueue.h"

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
    simsignal_t backloggedPacketsSignal;
    int sentPackets;
    int lostPackets;
    int backloggedPackets;
// +-------------------------------------------------------------------------------+
    virtual void handleSelfMessage(cMessage*);
    virtual void handlePacket(cMessage*);
    virtual void handleCQI(cMessage*);
    virtual void handleFrame();
    virtual void showCQIs();
    virtual void shuffle();
    virtual int serveUser(int, int, int*);
    virtual int CQI_to_BYTES(int);
    virtual UserQueue* getQueueById(int);
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage*) override;
    virtual void finish() override;
};

}

#endif
