#include "Source.h"
#include "Packet_m.h"


#define DEBUG ;

namespace opportunisticcellularnetwork {

Define_Module(Source);

/* +-------------------------------------------------------------------------------+
 * | Definition of Source's methods.                                              |
 * +-------------------------------------------------------------------------------+
 */

simtime_t Source::getDelay(bool isUniform)
{

    if (isUniform)
        return uniform(0,par("maxDelay"));
    else
        return exponential((simtime_t)par("exponentialMean"));
}


// When a source gets initialized it starts by setting a timer
void Source::initialize()
{
    simtime_t delay = getDelay(par("isDelayUniform"));
    scheduleAt(simTime() + delay, timerMessage);
}


void Source::handleMessage(cMessage *msg)
{
    // When timer message arrives, a packet gets forwarded out
    Packet *packet = new Packet("packet");

    // generating packet size and timestamp
    int size = intuniform(0,par("maxPacketSize"));

    packet->setSize(size);
    packet->setTimestamp(simTime());
    packet->setIndex(par("id"));

    #ifdef DEBUG
    EV << getName() << getId() << "::handleMessage() - Packet with size=" << packet->getSize() << " sent at " << packet->getTimestamp() << endl;
    #endif

    // sending out the message
    send(packet, "out");

    // re-scheduling timer
    simtime_t delay = getDelay(par("isDelayUniform"));
    scheduleAt(simTime() + delay, msg);

}

}

