#include "Cellular.h"

#define DEBUG ;
// #define TEST ;

namespace opportunisticcellularnetwork {

Define_Module(Cellular);

/*
 * +-------------------------------------------------------------------------------+
 * | Definition of Cellular's methods.                                             |
 * +-------------------------------------------------------------------------------+
 */

void Cellular::initialize()
{
    // Register throughput and response time signal
    userThroughputSignal = registerSignal("userThroughputSignal");
    userResponseTimeSignal = registerSignal("userResponseTimeSignal");

    receivedBytesTS = 0;

    id = par("id").intValue();
    typeCQI_ = par("typeCQI").boolValue();

    //Get the Parent parameter "population"
    int population = getParentModule()->par("population").intValue();

    //Scale the p_ value between [0,1]
    p_ = (double)(id + 0.5)/(population);

    #ifdef TEST
    EV << getName() << id << "::initialize() - TEST " << par("TEST").intValue() << ": CQI = " << calculateCQI() << endl;
    return;
    #endif

    #ifdef DEBUG
    if(!par("typeCQI").boolValue())
        // Scenario 0: uniform CQI.
        EV << getName() << id << "::initialize() - Scenario 0: CQI ~ U(1, 15)" << endl;
    else
        // Scenario 1: binomial CQI.
        EV << getName() << id << "::initialize() - Scenario 1: CQI ~ Bin(" << population << ", " << p_ << ")" << endl;
    #endif
}

void Cellular::handleMessage(cMessage *msg)
{
    if(strcmp(msg->getName(), "CQI") == 0)
        handleCQI();
    else
        handleRB(msg);

    // Since the message is no more useful, it will be 'deleted' to avoid any memory leak.
    delete(msg);
}

void Cellular::handleCQI()
{
/* +----------------------------------------------------------------------------------+
 * | The Antenna sends a CQI REQUEST to each cellular within the network. When a cell |
 * | receives a CQI REQUEST, it packs a CQI RESPONSE (CQIMessage) and it sends the    |
 * | response, with the current CQI, to the Antenna.                                  |
 * +----------------------------------------------------------------------------------+
 */

    #ifdef DEBUG
    EV << getName() << id <<"::handleCQI() - A new CQI REQUEST just arrived!" << endl;
    #endif

    // New CQI RESPONSE to send
    int CQI_ = calculateCQI();
    CQI* cqi = new CQI();
    cqi->setId(id);
    cqi->setCQI_(CQI_);

    send(cqi, "out");

    #ifdef DEBUG
    EV << getName() << id <<"::handleCQI() - A new CQI RESPONSE has just been sent! (CQI=" << CQI_ << ")" << endl;
    #endif
}

void Cellular::handleRB(cMessage* msg)
{
    RBsPacket* RBs = check_and_cast<RBsPacket*>(msg);
    receivedBytesTS = RBs->getUsedBytes();

    #ifdef DEBUG
    if(RBs->getUsedRBs() == 1)
        EV << getName() << id <<"::handleRB() - " << RBs->getUsedRBs() << " RB just arrived" << endl;
    else if(RBs->getUsedRBs() == 0)
        EV << getName() << id <<"::handleRB() - " << RBs->getUsedRBs() << " RBs have been allocated during this timeslot" << endl;
    else
        EV << getName() << id <<"::handleRB() - " << RBs->getUsedRBs() << " RBs just arrived" << endl;
    #endif

    for(int i = 0; i < RBs->getArrivalTimesArraySize(); i++)
    {
        emit(userResponseTimeSignal, simTime() - RBs->getArrivalTimes(i));
        #ifdef DEBUG
        EV << "\t\t\t\t\t\t" << getName() << id <<"::handleRB() -  ResponseTime = "<< simTime() - RBs->getArrivalTimes(i) << endl;
        #endif
    }

    emit(userThroughputSignal, receivedBytesTS);
    #ifdef DEBUG
    EV << "\t\t\t\t\t\t" << getName() << id <<"::handleRB() -  ThroughputTS = "<< receivedBytesTS << endl;
    #endif

    receivedBytesTS = 0;
}

int Cellular::calculateCQI()
{
    #ifdef TEST
    if(par("TEST_CQI").isSet())
        return par("TEST_CQI").intValue();
    #endif


    if(par("typeCQI").boolValue() == false)
        return intuniform(1, par("uniform_r").intValue(), 0);
    else
        // Shift the value [0,14] => [1,15]
        return binomial(par("rangeCQI").intValue() - 1, p_, 0) + 1;
}

};
