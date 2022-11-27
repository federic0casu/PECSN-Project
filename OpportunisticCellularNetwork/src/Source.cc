#include "Source.h"
#include "Packet_m.h"

namespace opportunisticcellularnetwork {

Define_Module(Source);


Define_Module(Source);

#define DEBUG ;


simtime_t Source::getDelay(bool isUniform) {

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

    // generating packet size
    int size = intuniform(0,par("maxPacketSize"));
    packet->setSize(size);

#ifdef DEBUG
    EV << "Packet with size : " << packet->getSize() << " sent" << endl;
#endif

    // sending out the message
    send(packet, "out");

    // re-scheduling timer
    simtime_t delay = getDelay(par("isDelayUniform"));
    scheduleAt(simTime() + delay, msg);
}

}
