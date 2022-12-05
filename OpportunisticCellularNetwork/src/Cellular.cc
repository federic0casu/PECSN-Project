#include "Cellular.h"
#include "Constant.h"
#include "Frame.h"

#define DEBUG ;

namespace opportunisticcellularnetwork {

Define_Module(Cellular);

/*
 * +-------------------------------------------------------------------------------+
 * | Definition of Cellular's methods.                                             |
 * +-------------------------------------------------------------------------------+
 */

void Cellular::initialize()
{
    // register statistics
    userThroughputSignal = registerSignal("userThroughputSignal");
    userResponseTimeSignal = registerSignal("userResponseTimeSignal");

    receivedBytesTS = 0;

    id_ = par("id").intValue();
    typeCQI_ = par("typeCQI").boolValue();
    CQI_ = calculateCQI();
    //binomial(par("binomial_n").intValue(), par("binomial_p").intValue());
}

void Cellular::handleMessage(cMessage *msg)
{

    /* +--------------------------------------------------------------------------------+
     * | Handshake                                                                      |
     * | The Antenna sends a CQI request to all the cellular.                           |
     * | The cell discriminate the msg by the object name (getName), then it sends the  |
     * | response with the CQI                                                          |
     * +--------------------------------------------------------------------------------+
     */
    if(strcmp(msg->getName(), "CQI") == 0)
    {
        #ifdef DEBUG
        EV << getName() << par("id").intValue() <<"::handleMessage() - A new CQI REQUEST is just arrived!" << endl;
        #endif

        // emit past TS received bytes
        emit(userThroughputSignal, receivedBytesTS);

        // reset n of bytes received in past TS
        receivedBytesTS = 0;

        // New cqi message to send
        int cell_id = par("id").intValue();
        CQIMessage * cqi = new CQIMessage();
        CQI_ = calculateCQI(); //Calculate every timeslot
        cqi->setId(cell_id);
        cqi->setCQI(CQI_);

        send(cqi, "out");

        #ifdef DEBUG
        EV << getName() << par("id").intValue() <<"::handleMessage() - A new CQI RESPONSE has just been sent! CQI=" << CQI_ << endl;
        #endif
    }
    else
    {
        /* +--------------------------------------------------------------------------------+
            * | Frame Detection                                                                |
            * | All the devices receive the frames (broadcast), so the cellular has to         |
            * | detect which RBs inside the frame  addressed to it                             |
            * | Format: |RB=dest|dim|bytes|RB=   |                                             |
            * +--------------------------------------------------------------------------------+
            */

        #ifdef DEBUG
        EV << getName() << par("id").intValue() <<"::handleMessage() - Frame detection" << endl;
        #endif


        Packet* packetInfo = check_and_cast<Packet*>(msg);
        receivedBytesTS += packetInfo->getSize();
        emit(userResponseTimeSignal, simTime() - packetInfo->getTimestamp());

        #ifdef DEBUG
        EV << getName() << par("id").intValue() <<"::handleMessage() - Frame detection - frame, received packet - size = "<< packetInfo->getSize() << endl;
        #endif


    }


    // Since the message is no more useful, it will be 'deleted' to avoid any memory leak.
    delete(msg);

}

/* +--------------------------------------------------------------------------------+
 * | Extract and check ID                                                           |
 * | Extract the RBs from the Frame. This function can be used                      |
 * | by devices to check the recipient of the RBs (destinatario)                    |
 * +--------------------------------------------------------------------------------+
 */
void Cellular::extractRB(Frame* frame, Frame *detected)
{
    int id = par("id").intValue();
    for(int i = 0; i < FRAME_SIZE; i++){ //Rem: FRAME_SIZE 25
        //This is a single RB
        if(id == frame->getIdRB(i)){
            detected->addRB(id, frame->getDimRB(i), frame->getBytesRB(i));  //Store correct RBs
        }
    }
}

int Cellular::calculateCQI()
{
    if(par("typeCQI").boolValue() == false){
        return uniform(1, par("uniform_r").intValue());
    } else {
        return binomial(par("binomial_n").intValue(), par("binomial_p").intValue());
    }
}

};
