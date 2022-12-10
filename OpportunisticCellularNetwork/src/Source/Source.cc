#include "Source.h"

#define DEBUG ;
#define TEST ;

namespace opportunisticcellularnetwork {

Define_Module(Source);

/* +-------------------------------------------------------------------------------+
 * | Definition of Source's methods.                                              |
 * +-------------------------------------------------------------------------------+
 */

// When a source gets initialized it starts by setting a timer
void Source::initialize()
{
    simtime_t delay;
    #ifdef TEST
    if(par("TEST").isSet())
    {
        delay = par("TEST_PERIOD").doubleValue();
        EV << getName() << par("id").intValue()%2 << "::initialize() - TEST " << par("TEST").intValue() << ": PACKET SIZE = " << par("TEST_SIZE").intValue() << ", ARRIVAL PERIOD = " << par("TEST_PERIOD").doubleValue() << " s" << endl;
    }
    else
    #endif

    delay = exponential((simtime_t)par("exponentialMean"), 0);

    scheduleAt(simTime() + delay, timerMessage);
}


void Source::handleMessage(cMessage *msg)
{
    // When timer message arrives, a packet gets forwarded out
    Packet *packet = new Packet("packet");

    // generating packet size and timestamp
    int size;
    #ifdef TEST
    if(par("TEST").isSet())
        size = par("TEST_SIZE").intValue();
    else
    #endif
    size = intuniform(0,par("maxPacketSize"), 1);

    packet->setSize(size);
    packet->setTimestamp(simTime());
    packet->setIndex(par("id"));

    #ifdef DEBUG
    EV << getName() << par("id").intValue()%getParentModule()->par("population").intValue() << "::handleMessage() - Packet (Size=" << packet->getSize() << ") sent at " << packet->getTimestamp() << endl;
    #endif

    // sending out the message
    send(packet, "out");

    // re-scheduling timer
    simtime_t delay;
    #ifdef TEST
    delay = par("TEST_PERIOD").doubleValue();
    scheduleAt(simTime() + delay, msg);
    return;
    #endif

    delay = exponential((simtime_t)par("exponentialMean"), 0);
    scheduleAt(simTime() + delay, msg);

}

}

