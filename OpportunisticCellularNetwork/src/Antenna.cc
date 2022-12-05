#include "Antenna.h"

#define DEBUG ;

namespace opportunisticcellularnetwork {

Define_Module(Antenna);

/* +-------------------------------------------------------------------------------+
 * | Definition of Antenna's methods.                                              |
 * +-------------------------------------------------------------------------------+
 */

void Antenna::initialize()
{
/* +-------------------------------------------------------------------------------+
 * | This method handles the Antenna's setup at the beginning of the simulation.   |
 * +-------------------------------------------------------------------------------+
 */

    // Retrieving the number of cellulars within the network (omnetpp.ini)
    population = par("population").intValue();

    // Retrieving timeslot duration (omnetpp.ini)
    timeslot = (simtime_t)par("timeslot").doubleValue();

    // Registering signals
    throughputSignal = registerSignal("throughputSignal");
    responseTimeSignal = registerSignal("responseTimeSignal");

    lostPackets = sentPackets = 0;

    switch(par("stage").intValue()) {
        case 1:
        {
            // Scenario 1: queues of fixed dimension.
            #ifdef DEBUG
            EV << "Antenna::initialize() - Scenario 1: queues of fixed dimension, population: " << population << endl;
            #endif

            userQueueDimension = par("queueDimension").intValue();

            for(int i = 0; i < population; i++)
                userQueues.push_back(new UserQueue(i, userQueueDimension));

            scheduleAt(simTime(), new cMessage("TIMER"));
        } break;
        default:
        {
            // Scenario 0: queues of infinite dimension.
            #ifdef DEBUG
            EV << "Antenna::initialize() - Scenario 0: queues of infinite dimension, population: " << population << endl;
            #endif

            for(int i = 0; i < population; i++)
                userQueues.push_back(new UserQueue(i));

            scheduleAt(simTime(), new cMessage("TIMER"));
        } break;
    }

    #ifdef DEBUG
    EV << "Antenna::initialize() - END" << endl;
    #endif
}

void Antenna::handleMessage(cMessage *msg)
{
    // A TIMESLOT expires
    if(msg->isSelfMessage())
    {
        handleSelfMessage(msg);
    }
    // A new CQI arrives
    else if(msg->arrivedOn("inCellular"))
    {
        handleCQI(msg);
    }
    // A new PACKET arrives
    else
    {
        handlePacket(msg);
    }
}

void Antenna::finish()
{
// +-----------------------------------------------------------------------------+
//  Packet loss (%): % of packets that cannot be queued
    if(par("stage").intValue() == 1)
        recordScalar("PacketLoss%", (lostPackets/sentPackets)*100);
// +-----------------------------------------------------------------------------+
}

void Antenna::handlePacket(cMessage *msg) {

/* +----------------------------------------------------------------------------------+
 * | AUTHOR : DANIEL                                                                  |
 * +----------------------------------------------------------------------------------+
 * | This metod is used to handle the packets incoming from the sources. Whenever a   |
 * | new packet arrives we check the gate number and the packet is stored in the queue|
 * | associated with the gate the packet came from.                                   |
 * +----------------------------------------------------------------------------------+
 */
    Packet *packet = check_and_cast<Packet*>(msg);

    int size = packet->getSize();
    int gateIndex = packet->getIndex();
    UserQueue *queue = getQueueById(gateIndex);

    // If we are in stage 1 (finite queues), we record a new lost packet and finish
    if (par("stage").intValue() && queue->getFreeSlots() == 0)
    {
        EV << "Antenna::handlePacket() - QUEUE (id=" << gateIndex << ") IS FULL : PACKET LOSS, SIZE=" << size << endl;
        lostPackets++;
    }
    else
    {
        #ifdef DEBUG
        EV << "Antenna::handlePacket() - New packet incoming from source (id="<< gateIndex << "), SIZE=" << size << endl;
        #endif

        // push packet into user queue
        queue->addPacket(size);

        #ifdef DEBUG
        queue->showQueue();
        #endif
    }

    // Since the message is no more useful, it will be 'deleted' to avoid any memory leak.
    delete(msg);
}

void Antenna::handleSelfMessage(cMessage *msg)
{
/* +--------------------------------------------------------------------------------+
 * | AUTHOR : FEDERICO                                                              |
 * +--------------------------------------------------------------------------------+
 * | This metod is used to handle the CQI request process. For each user registered |
 * | to the network, the Antenna requests a CQI at the beginning of each timeslot.  |
 * +--------------------------------------------------------------------------------+
 */
    #ifdef DEBUG
    EV << "Antenna::handleMessage() - NEW TIMESLOT" << endl;
    #endif

    for(int i = 0; i < population; i++)
        send(new cMessage("CQI"), "out", i);

    simtime_t delay = simTime() + timeslot;
    scheduleAt(delay, new cMessage("TIMER"));

    #ifdef DEBUG
    EV << "Antenna::handleMessage() - scheduleAt(" << delay << ", beep)" << endl;
    #endif

    // Since the message is no more useful, it will be 'deleted' to avoid any memory leak.
    delete(msg);
}

void Antenna::handleCQI(cMessage* msg)
{
/* +----------------------------------------------------------------------------------+
 * | AUTHOR : FEDERICO                                                                |
 * +----------------------------------------------------------------------------------+
 * | This method is used to manage CQIs. When a timeslot begins each user sends a CQI |
 * | packet. The Antenna stores CQIs in std::vector<CQIPacket> CQIs. Each element of  |
 * | CQIs is a couple whose fields are the user's id and the current CQI.             |
 * +----------------------------------------------------------------------------------+
 */

    CQIMessage *cqi = check_and_cast<CQIMessage*>(msg);
    int id = cqi->getId();
    int CQI = cqi->getCQI();

    #ifdef DEBUG
    EV << "Antenna::handleMessage() - A new CQI RESPONSE has just arrived! id=" << id << ", CQI=" << CQI << endl;
    #endif

    CQIPacket *tmp = new CQIPacket(id, CQI);
    CQIs.push_back(tmp);

    // Since the message is no more useful, it will be 'deleted' to avoid any memory leak.
    delete(msg);

    if(CQIs.size() == population)
    {
        // Each user has just sent his/her CQI packet. The Antenna must compose a new Frame.
        handleFrame();
        // To remove all the elements of the std::vector<CQIPacket*> CQIs.
        CQIs.clear();
    }
}

void Antenna::handleFrame()
{
/* +-----------------------------------------------------------------------------------+
 * | AUTHOR : FEDERICO                                                                 |
 * +-----------------------------------------------------------------------------------+
 * | This method is used to compose a new Frame. The Antenna composes a frame made up  |
 * | of 25 RBs. The antenna serves its users using an opportunistic policy: backlogged |
 * | users are served by decreasing CQI. When a user is considered for service, its    |
 * | queue is emptied, if the number of unallocated RBs is large enough.               |
 * +-----------------------------------------------------------------------------------+
 */

    // Opportunistic policy: users are served by decreasing CQI -> CQIs has to be sorted
    // in a decreasing order.
    std::sort(CQIs.begin(), CQIs.end(), [](CQIPacket *X, CQIPacket *Y){ return X->getCQI()>Y->getCQI();});

    // Once std::vector CQIs has been sorted the Antenna can pack a new Frame.
    int remainingRBs = par("maxFrameDim"), currentUser, currentCQI;

    // To store the total number of bytes sent in a timeslot.
    int bytesToSend = 0;

    // Frame to send
    Frame* frameToSend = new Frame();

    for(int i = 0; i < population; i++)
    {
        // Retrieving the user under service.
        currentUser = CQIs[i]->getId();

        // Retrieving the current CQI sent by the user under service.
        currentCQI = CQIs[i]->getCQI();

        #ifdef DEBUG
        EV << "Antenna::handleFrame() - user" << currentUser << " is currently under service (CQI=" << currentCQI << ")" << endl;
        #endif

        bytesToSend += serveUser(currentUser, currentCQI, &remainingRBs, frameToSend);
    }

    delete(frameToSend);

    // Recording throughtput's statistics
    emit(throughputSignal, bytesToSend);
}

int Antenna::serveUser(int currentUser, int currentCQI, int *remainingRBs, Frame* frame)
{
    // Retrieving the queue associated to the user under service.
    UserQueue* currentQueue = getQueueById(currentUser);

    int bytesAllocated = 0, currentPacketSize, startingPacketSize = -1, remainingBytesUsedByCurrentRB = CQI_to_BYTES(currentCQI);

    while(!currentQueue->getQueue()->empty())
    {
        // Service policy is FIFO -> Packets must be served starting from the oldest.
        currentPacketSize = currentQueue->getQueue()->begin()->second;

        if(startingPacketSize == -1)
            startingPacketSize = currentQueue->getQueue()->begin()->second;

        if(currentPacketSize > CQI_to_BYTES(currentCQI)*(*remainingRBs))
        {
            #ifdef DEBUG
            EV << "Antenna::serveUser() - user" << currentUser << " can no longer be served (packetSize=" << startingPacketSize << ", remainingRBs=" << *remainingRBs << ")" << endl;
            #endif
            if(remainingBytesUsedByCurrentRB != 0)
            {
                (*remainingRBs)--;
                bytesAllocated += (CQI_to_BYTES(currentCQI) - remainingBytesUsedByCurrentRB);
                frame->addRB(currentUser, CQI_to_BYTES(currentCQI), CQI_to_BYTES(currentCQI) - remainingBytesUsedByCurrentRB);
                #ifdef DEBUG
                EV << "Antenna::serveUser() - A NEW RB has been allocated (size=" << CQI_to_BYTES(currentCQI) << ", allocatedBytes=" << CQI_to_BYTES(currentCQI)-remainingBytesUsedByCurrentRB << ", remainingRBs=" << *remainingRBs << ")" << endl;
                #endif
            }
            return bytesAllocated;
        }

        if(remainingBytesUsedByCurrentRB < currentPacketSize)
        {
            currentQueue->getQueue()->begin()->second -= remainingBytesUsedByCurrentRB;
            remainingBytesUsedByCurrentRB = CQI_to_BYTES(currentCQI);
            (*remainingRBs)--;
            bytesAllocated += CQI_to_BYTES(currentCQI);
            frame->addRB(currentUser, CQI_to_BYTES(currentCQI), CQI_to_BYTES(currentCQI));
            #ifdef DEBUG
            EV << "Antenna::serveUser() - A NEW RB has been allocated (size=" << CQI_to_BYTES(currentCQI) << ", allocatedBytes=" << CQI_to_BYTES(currentCQI) << ", remainingRBs=" << *remainingRBs << ")" << endl;
            #endif
        }
        else
        {
            remainingBytesUsedByCurrentRB -= currentQueue->getQueue()->begin()->second;

            // Statistics
            Packet * packet = new Packet("packet");
            packet->setSize(startingPacketSize);
            packet->setTimestamp(currentQueue->getQueue()->begin()->first);
            send(packet, "out", currentUser);
            // End statistics

            #ifdef DEBUG
            EV << "Antenna::serveUser() - A NEW PACKET has been allocated (packetSize=" << startingPacketSize << ", remainingRBs=" << *remainingRBs << ")" << endl;
            #endif

            startingPacketSize = -1;
            currentQueue->getQueue()->erase(currentQueue->getQueue()->begin());

            // If the current packet is the last one the Antenna must allocate the packet even if the RB
            // is not entirely used.
            if(currentQueue->getQueue()->empty())
            {
                (*remainingRBs)--;
                bytesAllocated += (CQI_to_BYTES(currentCQI) - remainingBytesUsedByCurrentRB);
                frame->addRB(currentUser, CQI_to_BYTES(currentCQI), CQI_to_BYTES(currentCQI) - remainingBytesUsedByCurrentRB);
                #ifdef DEBUG
                EV << "Antenna::serveUser() - A NEW RB has been allocated (size=" << CQI_to_BYTES(currentCQI) << ", allocatedBytes=" << CQI_to_BYTES(currentCQI)-remainingBytesUsedByCurrentRB << ", remainingRBs=" << *remainingRBs << ")" << endl;
                #endif
            }
        }
    }

    #ifdef DEBUG
    if(currentQueue->getQueue()->empty() && bytesAllocated == 0)
        EV << "Antenna::serveUser() - user" << currentUser << " has an empty queue!" << endl;
    #endif

    return bytesAllocated;
}

int Antenna::CQI_to_BYTES(int CQI)
{
//  AUTHOR : FEDERICO
    switch(CQI)
    {
        case 1 : return 3;
        case 2 : return 3;
        case 3 : return 6;
        case 4 : return 11;
        case 5 : return 15;
        case 6 : return 20;
        case 7 : return 25;
        case 8 : return 36;
        case 9 : return 39;
        case 10: return 50;
        case 11: return 63;
        case 12: return 72;
        case 13: return 80;
        case 14: return 93;
        case 15: return 93;
        default: return 0;
    }
}

UserQueue* Antenna::getQueueById(int id)
{
//  AUTHOR : FEDERICO
    for(UserQueue *currentUser : userQueues)
    {
        if(currentUser->getId() == id)
            return currentUser;
    }
    return NULL;
}

}
