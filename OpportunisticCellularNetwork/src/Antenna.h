#ifndef __OPPORTUNISTICCELLULARNETWORK_ANTENNA_H_
#define __OPPORTUNISTICCELLULARNETWORK_ANTENNA_H_

#include <omnetpp.h>
#include <vector>

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
    vector<FifoQueue> queues;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void handleCQI();
    virtual void handleFrame();
};

}

#endif
