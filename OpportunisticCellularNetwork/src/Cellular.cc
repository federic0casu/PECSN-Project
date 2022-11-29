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
/* AUTHOR : FEDERICO
 * +--------------------------------------------------------------------------------+
 * | CODICE DI PROVA -                                                              |
 * | Per poter provare i metodi handleFrame() e handleCQI avevo bisogno di simulare |
 * | il comportamento dell'antenna. OVVIAMENTE questo codice deve essere scritto    |
 * | nuovamente da chi ha questo compito. Quindi eliminate tutto :) !               |
 * +--------------------------------------------------------------------------------+
 */
    if(strcmp(msg->getName(), "CQI") == 0)
    {
        #ifdef DEBUG
        EV << getName() << getId() <<"::handleMessage() - A new CQI REQUEST is just arrived!" << endl;
        #endif

        int CQI = uniform(1, 15);
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
