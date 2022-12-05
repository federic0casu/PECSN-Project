#include "Source.h"
#include "Packet_m.h"


#define DEBUG ;

namespace opportunisticcellularnetwork {

Define_Module(Source);

/* +-------------------------------------------------------------------------------+
 * | Definition of Source's methods.                                              |
 * +-------------------------------------------------------------------------------+
 */

// When a source gets initialized it starts by setting a timer
void Source::initialize()
{
//  AUTHOR : DANIEL
    simtime_t delay = exponential((simtime_t)par("exponentialMean"), 0);
    scheduleAt(simTime() + delay, timerMessage);
}


void Source::handleMessage(cMessage *msg)
{
//  AUTHOR : DANIEL

    // When timer message arrives, a packet gets forwarded out
    Packet *packet = new Packet("packet");

    // generating packet size and timestamp
    int size = intuniform(0,par("maxPacketSize"), 1);

    packet->setSize(size);
    packet->setTimestamp(simTime());
    packet->setIndex(par("id"));

    #ifdef DEBUG
    EV << getName() << par("id").intValue()*2 << "::handleMessage() - Packet with size=" << packet->getSize() << " sent at " << packet->getTimestamp() << endl;
    #endif

    // sending out the message
    send(packet, "out");

    // re-scheduling timer
    simtime_t delay = exponential((simtime_t)par("exponentialMean"), 0);
    scheduleAt(simTime() + delay, msg);

}

}

