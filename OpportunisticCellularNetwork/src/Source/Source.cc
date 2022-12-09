#include "Source.h"

#define DEBUG ;
//#define TEST ;

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
        delay = par("TEST_RATE").doubleValue();
        EV << getName() << par("id").intValue()%2 << "::initialize() - TEST " << par("TEST").intValue() << ": PACKET SIZE = " << par("TEST_SIZE").intValue() << ", ARRIVAL RATE = " << delay << " s" << endl;
    }
    else
    #endif


    double mean = 1/(par("rate").doubleValue()*1000);
    delay = exponential(mean, 0);

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
    EV << getName() << par("id").intValue()%2 << "::handleMessage() - Packet (Size=" << packet->getSize() << ") sent at " << packet->getTimestamp() << endl;
    #endif

    // sending out the message
    send(packet, "out");

    // re-scheduling timer
    simtime_t delay;
    #ifdef TEST
    delay = par("TEST_RATE").doubleValue();
    scheduleAt(simTime() + delay, msg);
    return;
    #endif



    double mean = 1/(par("rate").doubleValue()*1000);
    delay = exponential(mean, 0);

    scheduleAt(simTime() + delay, msg);

}

}

