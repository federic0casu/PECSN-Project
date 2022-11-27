#include "Cellular.h"

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
     * | CODICE DI PROVA - AUTORE: Federico                                             |
     * | Per poter provare i metodi handleFrame() e handleCQI avevo bisogno di simulare |
     * | il comportamento dell'antenna. OVVIAMENTE questo codice deve essere scritto    |
     * | nuovamente da chi ha questo compito. Quindi eliminate tutto :) !               |
     * +--------------------------------------------------------------------------------+
    if(strcmp(msg->getName(), "CQI") == 0)
    {
        // DEBUG: begin
        EV << getName() << getId() <<"::handleMessage() - A new CQI REQUEST is just arrived!" << endl;
        // DEBUG: end

        int CQI = uniform(1, 15);
        int id = par("id").intValue();
        CQIMessage * cqi = new CQIMessage();
        cqi->setId(id);
        cqi->setCQI(CQI);

        send(cqi, "out");

        // DEBUG: begin
        EV << getName() << getId() <<"::handleMessage() - A new CQI RESPONSE has just been sent! CQI=" << CQI << endl;
        // DEBUG: end
    }

    // Since the message is no more useful, it will be 'deleted' to avoid any memory leak.
    delete(msg);
      +--------------------------------------------------------------------------------+
      | FINE CODICE DI PROVA                                                           |
      +--------------------------------------------------------------------------------+
   */
}

};
