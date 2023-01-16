#include "Antenna.h"

// #define DEBUG ;

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

    // Register throughput signal
    throughputSignal = registerSignal("throughputSignal");

    // Register backlogged packtes signal
    backloggedPacketsSignal = registerSignal("backloggedPacketsSignal");

    lostPackets = sentPackets = backloggedPackets = 0;

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

            simtime_t delay = simTime() + timeslot;
            scheduleAt(delay, new cMessage("TIMER"));
        } break;
        default:
        {
            // Scenario 0: queues of infinite dimension.
            #ifdef DEBUG
            EV << "Antenna::initialize() - Scenario 0: queues of infinite dimension, population: " << population << endl;
            #endif

            for(int i = 0; i < population; i++)
                userQueues.push_back(new UserQueue(i));

            simtime_t delay = simTime() + timeslot;
            scheduleAt(delay, new cMessage("TIMER"));
        } break;
    }
}

void Antenna::handleMessage(cMessage *msg)
{
    // A TIMESLOT expires
    if(msg->isSelfMessage())
        handleSelfMessage(msg);
    // A new CQI arrives
    else if(msg->arrivedOn("inCellular"))
    {
        if(strcmp(msg->getName(), "END") == 0)
        {
            for(int i = 0; i < population; i++)
                send(new cMessage("END"), "out", i);
            delete(msg);
            endSimulation();
        }
        else
            handleCQI(msg);
    }
    // A new PACKET arrives
    else
        handlePacket(msg);
}

void Antenna::finish()
{
    #ifdef DEBUG
    EV << "Antenna::finish()" << endl;
    #endif
// +-----------------------------------------------------------------------------+
//  Packet loss (%): % of packets that cannot be queued
    if(par("stage").intValue() == 1)
        recordScalar("PacketLoss%", (lostPackets/sentPackets)*100);
// +-----------------------------------------------------------------------------+
}

void Antenna::handlePacket(cMessage *msg) {

/* +----------------------------------------------------------------------------------+
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
        #ifdef DEBUG
        EV << "Antenna::handlePacket() - QUEUE (id=" << gateIndex << ") IS FULL : PACKET LOSS, SIZE=" << size << endl;
        #endif
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

        backloggedPackets++;

        emit(backloggedPacketsSignal, backloggedPackets);
    }

    // Since the message is no more useful, it will be 'deleted' to avoid any memory leak.
    delete(msg);
}

void Antenna::handleSelfMessage(cMessage *msg)
{
/* +--------------------------------------------------------------------------------+
 * | This metod is used to handle the CQI request process. For each user registered |
 * | to the network, the Antenna requests a CQI at the beginning of each timeslot.  |
 * +--------------------------------------------------------------------------------+
 */

    for(int i = 0; i < population; i++)
        send(new cMessage("CQI"), "out", i);

    simtime_t delay = simTime() + timeslot;
    scheduleAt(delay, new cMessage("TIMER"));

    #ifdef DEBUG
    EV << "Antenna::handleSelfMessage() - NEW TIMESLOT - scheduleAt(" << delay << ", beep)" << endl;
    #endif

    // Since the message is no more useful, it will be 'deleted' to avoid any memory leak.
    delete(msg);
}

void Antenna::handleCQI(cMessage* msg)
{
/* +----------------------------------------------------------------------------------+
 * | This method is used to manage CQIs. When a timeslot begins each user sends a CQI |
 * | packet. The Antenna stores CQIs in std::vector<CQIPacket> CQIs. Each element of  |
 * | CQIs is a couple whose fields are the user's id and the current CQI.             |
 * +----------------------------------------------------------------------------------+
 */

    CQI *cqi = check_and_cast<CQI*>(msg);
    int id = cqi->getId();
    int CQI_ = cqi->getCQI_();

    #ifdef DEBUG
    EV << "Antenna::handleCQI() - A new CQI RESPONSE has just arrived! id=" << id << ", CQI=" << CQI_ << endl;
    #endif

    CQIPacket *tmp = new CQIPacket(id, CQI_);
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
 * | This method is used to compose a new Frame. The Antenna composes a frame made up  |
 * | of 25 RBs. The antenna serves its users using an opportunistic policy: backlogged |
 * | users are served by decreasing CQI. When a user is considered for service, its    |
 * | queue is emptied, if the number of unallocated RBs is large enough.               |
 * +-----------------------------------------------------------------------------------+
 */

    #ifdef DEBUG
    showCQIs();
    #endif

    // To model the (random) delay at wich the CQI responses arrive at the Antenna.
    shuffle();

    #ifdef DEBUG
    showCQIs();
    #endif

    // Opportunistic policy: users are served by decreasing CQI -> CQIs has to be sorted
    // in a decreasing order.
    std::sort(CQIs.begin(), CQIs.end(), [](CQIPacket *X, CQIPacket *Y) { return X->getCQI()>Y->getCQI(); });

    // Once std::vector CQIs has been sorted the Antenna can pack a new Frame.
    int remainingRBs = par("maxFrameDim"), currentUser, currentCQI;

    // To store the total number of bytes sent in a timeslot.
    int bytesToSend = 0;

    for(int i = 0; i < population; i++)
    {
        // Retrieving the user under service.
        currentUser = CQIs[i]->getId();

        // Retrieving the current CQI sent by the user under service.
        currentCQI = CQIs[i]->getCQI();

        bytesToSend += serveUser(currentUser, currentCQI, &remainingRBs);
    }

    // Recording throughtput's statistics
    emit(throughputSignal, bytesToSend);

    // Recording backlog's statistics
    emit(backloggedPacketsSignal, backloggedPackets);

    #ifdef DEBUG
    EV << "Antenna::handleFrame() - TROUGHPUT = " << bytesToSend << endl;
    EV << "Antenna::handleFrame() - BACKLOG = " << backloggedPackets << endl;
    #endif
}

void Antenna::shuffle()
{
    for (int i = 0; i < CQIs.size() - 1; i++)
    {
        // generate a random number 'j' such that 'i <= j < n-1' and
        // swap the element present at index 'j' with the element
        // present at current index 'i'
        int j = i + intuniform(0, CQIs.size()-1-i);
        std::swap(CQIs[i], CQIs[j]);
    }
}

void Antenna::showCQIs()
{
    EV << "CQIs : [";
    for(int i = 0; i < CQIs.size(); i++)
        if(i == 0)
            EV << "<" << CQIs[i]->getId() << ", " << CQIs[i]->getCQI() << ">";
        else
            EV << ", <" << CQIs[i]->getId() << ", " << CQIs[i]->getCQI() << ">";
    EV << "]" << endl;
}

int Antenna::serveUser(int user, int CQI, int* remainingRB)
{
    int bytesToSend = 0;
    int dimRB = CQI_to_BYTES(CQI);
    int remainingBytesFrame = (*remainingRB) * dimRB;

    // Retrieving the current user queue
    UserQueue* queue = getQueueById(user);
    std::pair<simtime_t, int> pkt;

    // RBs to send
    RBsPacket* RBs = new RBsPacket();
    RBs->setDestinationUser(user);

    #ifdef DEBUG
    queue->showQueue();
    EV << "Antenna::serveUser() - RB dimension = " << dimRB << endl;
    #endif

    while(!queue->getQueue()->empty())
    {
        // Retrieving the packet arrival time
        pkt.first = queue->getQueue()->begin()->first;

        // Retrieving the packet size
        pkt.second = queue->getQueue()->begin()->second;

        // If there is enough space to allocate the packet into the Frame...
        if (pkt.second <= remainingBytesFrame){

            queue->getQueue()->erase(queue->getQueue()->begin());

            remainingBytesFrame -= pkt.second;
            bytesToSend += pkt.second;

            // Store the arrival time of the current packet to allow the
            // destination user to compute ResponseTime.
            RBs->appendArrivalTimes(pkt.first);

            #ifdef DEBUG
            EV << "\t\t\t\t\t\tA NEW packet has been allocated (size=" << pkt.second << ")" << endl;
            #endif

            backloggedPackets--;
        }
        else
        {
            #ifdef DEBUG
            EV << "Antenna::serveUser() - user" << user << " can no longer be served (packetSize=" << pkt.second << ")" << endl;
            #endif
            break;
        }
    }

    // We need to calculate used RBs
    int allocatedFrameBytes = (*remainingRB) * dimRB - remainingBytesFrame;
    int allocatedRBs = (allocatedFrameBytes % dimRB) ? allocatedFrameBytes/dimRB + 1: allocatedFrameBytes/dimRB;

    #ifdef DEBUG
    EV << "Antenna::serveUser() - Number of used bytes for user" << user << " are " << allocatedFrameBytes << endl;
    EV << "Antenna::serveUser() - Number of used RBs for user" << user << " are " << allocatedRBs << endl;
    #endif

    (*remainingRB) -= allocatedRBs;

    #ifdef DEBUG
    EV << "Antenna::serveUser() - Number of remaining RBs are " << *remainingRB << endl;
    #endif

    RBs->setUsedRBs(allocatedRBs);
    RBs->setUsedBytes(bytesToSend);

    send(RBs, "out", user);

    return bytesToSend;
}

int Antenna::CQI_to_BYTES(int CQI)
{
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
    for(UserQueue *currentUser : userQueues)
    {
        if(currentUser->getId() == id)
            return currentUser;
    }
    return NULL;
}

}
