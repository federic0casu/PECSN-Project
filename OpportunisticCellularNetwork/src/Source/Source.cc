#include "Source.h"

// #define DEBUG ;
// #define TEST ;

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
    double mean = 0.0;

    timerMessage = new cMessage("beep");

    #ifdef TEST
    if(par("TEST_PERIOD").doubleValue() != 0)
    {
        delay = par("TEST_PERIOD").doubleValue();
        EV << getName() << par("id").intValue()%2 << "::initialize() - TEST " << par("TEST").intValue() << ": PACKET SIZE = " << par("TEST_SIZE").intValue() << ", ARRIVAL PERIOD = " << delay << " s" << endl;
    }
    else
    {
        mean = 1/(par("RATE").doubleValue()*1000);
        delay = exponential(mean, 0);
        EV << getName() << par("id").intValue()%2 << "::initialize() - TEST " << par("TEST").intValue() << ": PACKET SIZE = " << par("TEST_SIZE").intValue() << ", MEAN ARRIVAL RATE = " << mean << endl;
    }

    scheduleAt(simTime() + delay, timerMessage);
    return;
    #endif


    #ifdef DEBUG
    if(par("stage").intValue() == 0)
        EV << getName() << par("id").intValue()%2 << "::initialize() - Scenario 0 - queues of infinite dimension, population = " << getParentModule()->par("population").intValue() << ", EXPONENTIAL MEAN = " << par("RATE").doubleValue() << " ms" << endl;
    else
        EV << getName() << par("id").intValue()%2 << "::initialize() - Scenario 1 - queues of finite dimension, population = " << getParentModule()->par("population").intValue() << ", EXPONENTIAL MEAN = " << par("RATE").doubleValue() << " ms" << endl;
    #endif

    mean = 1/(par("RATE").doubleValue()*1000);
    delay = exponential(mean, 0);

    scheduleAt(simTime() + delay, timerMessage);

    #ifdef DEBUG
    EV << getName() << par("id").intValue()%2 << "::initialize() - simTime() + delay = " << simTime() + delay << endl;
    #endif
}


void Source::handleMessage(cMessage *msg)
{
    // When timer message arrives, a packet gets forwarded out
    Packet *packet = new Packet("packet");

    // Generating packet size and timestamp
    int size;
    #ifdef TEST
    if(par("TEST_SIZE").intValue() != 0)
        size = par("TEST_SIZE").intValue();
    else
    #endif
    size = intuniform(0, par("maxPacketSize"), 1);

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
    if(par("TEST_PERIOD").doubleValue() != 0)
    {
        delay = par("TEST_PERIOD").doubleValue();
        scheduleAt(simTime() + delay, msg);
        return;
    }
    #endif

    double mean = 1/(par("RATE").doubleValue()*1000);
    delay = exponential(mean, 0);

    scheduleAt(simTime() + delay, msg);

    #ifdef DEBUG
    EV << getName() << par("id").intValue()%2 << "::handleMessage() - simTime() + delay = " << simTime() + delay << endl;
    #endif
}

}
