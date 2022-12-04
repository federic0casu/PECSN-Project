#ifndef __OPPORTUNISTICCELLULARNETWORK_CELLULAR_H
#define __OPPORTUNISTICCELLULARNETWORK_CELLULAR_H

#include <algorithm>
#include <omnetpp.h>
#include <vector>

#include "CQIMessage_m.h"
#include "CQIPacket.h"
#include "Packet_m.h"
#include "Source.h"
#include "Frame.h"

#include "UserQueue.h"

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
    // signals
    simsignal_t userResponseTimeSignal;
    simsignal_t userThroughputSignal;

    int receivedBytesTS = 0;

    int id_;
    bool typeCQI_;
    int CQI_;

  public:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    void extractRB(Frame* frame, Frame *detected); //Extract RBs by Cellular ID
    int calculateCQI();

};

};

#endif
