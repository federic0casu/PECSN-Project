#ifndef __OPPORTUNISTICCELLULARNETWORK_SOURCE_H_
#define __OPPORTUNISTICCELLULARNETWORK_SOURCE_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace opportunisticcellularnetwork {

/*
 * +-------------------------------------------------------------------------------+
 * | Implements the Source simple module. See the NED file for more information.   |
 * +-------------------------------------------------------------------------------+
 */

class Source : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

}

#endif
