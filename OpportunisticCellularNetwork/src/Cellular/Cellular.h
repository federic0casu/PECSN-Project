#ifndef __OPPORTUNISTICCELLULARNETWORK_CELLULAR_H
#define __OPPORTUNISTICCELLULARNETWORK_CELLULAR_H

#include <algorithm>
#include <omnetpp.h>
#include <vector>

#include "../Utility/Constant.h"

#include "../Messages/CQI_m.h"
#include "../Messages/Packet_m.h"
#include "../Messages/RBsPacket_m.h"

#include "../Utility/CQIPacket.h"
#include "../Source/Source.h"

using namespace omnetpp;


namespace opportunisticcellularnetwork {

/*
 * +-------------------------------------------------------------------------------+
 * | Implements the Cellular simple module. See the NED file for more information. |
 * +-------------------------------------------------------------------------------+
 */


class Cellular : public cSimpleModule
{
  private:
    int receivedBytesTS;
    int id;
    bool typeCQI_;
    double p_;
// +-------------------------------------------------------------------------------+
//  Statistics
    simsignal_t userResponseTimeSignal;
    simsignal_t userThroughputSignal;
// +-------------------------------------------------------------------------------+
    void handleCQI();
    void sendCQI();
    void handleRB(cMessage*);
    int calculateCQI();
  public:
    virtual void initialize();
    virtual void handleMessage(cMessage*);
};

};

#endif
