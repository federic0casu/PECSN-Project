//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "Source.h"
#include "Packet_m.h"



#define DEBUG ;

namespace opportunisticcellularnetwork {

    Define_Module(Source);

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

        // generating packet size and timestamp
        int size = intuniform(0,par("maxPacketSize"));
        packet->setSize(size);
        packet->setTimestamp(simTime());

    #ifdef DEBUG
        EV << "Packet with size : " << packet->getSize() << " sent at " << packet->getTimestamp() << endl;
    #endif

        // sending out the message
        send(packet, "out");

        // re-scheduling timer
        simtime_t delay = getDelay(par("isDelayUniform"));
        scheduleAt(simTime() + delay, msg);
    }

}

