#ifndef __OPPORTUNISTICCELLULARNETWORK_SOURCE_H_
#define __OPPORTUNISTICCELLULARNETWORK_SOURCE_H_

#include <omnetpp.h>

#include "../Messages/Packet_m.h"

using namespace omnetpp;

namespace opportunisticcellularnetwork {

/* +-------------------------------------------------------------------------------+
 * | Implements the Source simple module. See the NED file for more information.   |
 * +-------------------------------------------------------------------------------+
 */

class Source : public cSimpleModule
{
  private:
    cMessage *timerMessage = new cMessage("beep");
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

}

#endif
