#include "Cellular.h"

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
}

void Cellular::handleMessage(cMessage *msg)
{
    /* +--------------------------------------------------------------------------------+
     * | Handshake Antenna-Device                                                       |
     * | The Antenna sends a CQI request                                                |
     * | The Device reply with its CQI (perceived quality of network)                   |
     * +--------------------------------------------------------------------------------+
     */
    EV << "MESSAGGIO " << msg->getName() << endl;

    if(strcmp(msg->getName(), "CQI") == 0)
    {
        #ifdef DEBUG
        EV << getName() << getId() <<"::handleMessage() - A new CQI REQUEST is just arrived!" << endl;
        #endif

        int CQI = uniform(1, 15); // Should we consider a random CQI?
        int id = par("id").intValue();
        CQIMessage * cqi = new CQIMessage();
        cqi->setId(id);
        cqi->setCQI(CQI);

        send(cqi, "out");

        #ifdef DEBUG
        EV << getName() << getId() <<"::handleMessage() - A new CQI RESPONSE has just been sent! CQI=" << CQI << endl;
        #endif
    }


    /* +--------------------------------------------------------------------------------+
     * | Frame Detection                                                                |
     * | All the devices receive the frames (broadcast), so the cellular has to         |
     * | detect which frame is addressed to it                                          |
     * +--------------------------------------------------------------------------------+
     */

    if(strcmp(msg->getName(), "Frame") == 0)
    {
        #ifdef DEBUG
        EV << getName() << getId() <<"::handleMessage() - Frame detection" << endl;
        #endif

        int CQI = uniform(1, 15); // Random CQI?
        int id = par("id").intValue();
        CQIMessage * cqi = new CQIMessage();
        cqi->setId(id);
        cqi->setCQI(CQI);

        send(cqi, "out");

        #ifdef DEBUG
        EV << getName() << getId() <<"::handleMessage() - A new CQI RESPONSE has just been sent! CQI=" << CQI << endl;
        #endif
    }



    // Since the message is no more useful, it will be 'deleted' to avoid any memory leak.
    delete(msg);
    /*
     *  +--------------------------------------------------------------------------------+
     *  | FINE CODICE DI PROVA                                                           |
     *  +--------------------------------------------------------------------------------+
     */
}

};
