#ifndef __OPPORTUNISTICCELLULARNETWORK_CELLULAR_H
#define __OPPORTUNISTICCELLULARNETWORK_CELLULAR_H

#include <omnetpp.h>
#include "CQIMessage_m.h"

using namespace omnetpp;

namespace opportunisticcellularnetwork {

/*
 * +-------------------------------------------------------------------------------+
 * | Implements the Cellular simple module. See the NED file for more information. |
 * +-------------------------------------------------------------------------------+
 */

class Cellular : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

};

#endif
