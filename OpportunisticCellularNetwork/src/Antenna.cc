#include "Antenna.h"

namespace opportunisticcellularnetwork {

Define_Module(Antenna);

/*
 * +-------------------------------------------------------------------------------+
 * | Definition of Antenna's methods.                                              |
 * +-------------------------------------------------------------------------------+
 */

void Antenna::initialize()
{
    population = par("population").intValue();
    timeslot = (simtime_t)par("timeslot");

    throughputSignal = registerSignal("throughputSignal");

    lostPackets = sentPackets = 0;

    switch(par("stage").intValue()) {
        case 1:
        {
        // +-------------------------------------------------------------------------------+
        //  Scenario: queues of fixed dimension.
            // DEBUG: begin
            EV << "Antenna::initialize() - Scenario 1: queues of fixed dimension, population: " << population << endl;
            // DEBUG: end

            userQueueDimension = par("queueDimension").intValue();

            for(int i = 0; i < population; i++)
                userQueues.push_back(new UserQueue(i, userQueueDimension));

            scheduleAt(simTime(), new cMessage("TIMER"));

        // +-------------------------------------------------------------------------------+
        } break;
        default:
        {
        // +-------------------------------------------------------------------------------+
        //  Scenario: queues of infinite dimension.
            // DEBUG: begin
            EV << "Antenna::initialize() - Scenario 0: queues of infinite dimension, population: " << population << endl;
            // DEBUG: end

            for(int i = 0; i < population; i++)
                userQueues.push_back(new UserQueue(i));

            scheduleAt(simTime(), new cMessage("TIMER"));

        // +-------------------------------------------------------------------------------+
        } break;
    }
}

void Antenna::handleMessage(cMessage *msg)
{
    /* +--------------------------------------------------------------------------------+
     * | CODICE DI PROVA - AUTORE: Federico                                             |
     * | Per poter provare i metodi handleFrame() e handleCQI avevo bisogno di simulare |
     * | il comportamento dell'antenna. OVVIAMENTE questo codice deve essere scritto    |
     * | nuovamente da chi ha questo compito. Quindi eliminate tutto :) !               |
     * +--------------------------------------------------------------------------------+
    if(msg->isSelfMessage())
    {
        // DEBUG: begin
        EV << "Antenna::handleMessage() - A new timeslot has just begun!" << endl;
        // DEBUG: end

        for(int i = 0; i < population; i++)
            send(new cMessage("CQI"), "out", i);

        simtime_t delay = simTime() + timeslot;
        scheduleAt(delay, new cMessage("TIMER"));

        // DEBUG: begin
        EV << "Antenna::handleMessage() - scheduleAt(" << delay << ", beep)" << endl;
        // DEBUG: end

        // Since the message is no more useful, it will be 'deleted' to avoid any memory leak.
        delete(msg);
    }
    // If a new CQI arrives...
    else if(msg->arrivedOn("inCellular"))
    {
        handleCQI(msg);
    }
       +--------------------------------------------------------------------------------+
       | FINE CODICE DI PROVA                                                           |
       +--------------------------------------------------------------------------------+
    */

    //else if(We have queues of finite dimension and a new packet has just arrived)
    //{
    //  if(user's queue is full)
    //  {
    //      lostPackets += 1;
    //  }
    //  else
    //  {
    //      packet handling
    //  }
    //  delete(Packet message);
    //}
}

void Antenna::handleCQI(cMessage* msg)
{
// +----------------------------------------------------------------------------------+
// | This method is used to manage CQIs. When a timeslot begins each user sends a CQI |
// | packet. The Antenna stores CQIs in std::vector<CQIPacket> CQIs. Each element of  |
// | CQIs is a couple whose fields are the user's id and the current CQI.             |
// +----------------------------------------------------------------------------------+

    CQIMessage *cqi = check_and_cast<CQIMessage*>(msg);
    int id = cqi->getId();
    int CQI = cqi->getCQI();

    // DEBUG: begin
    EV << "Antenna::handleMessage() - A new CQI has just arrived! id=" << id << ", CQI=" << CQI << endl;
    // DEBUG: end

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
// +-----------------------------------------------------------------------------------+
// | This method is used to compose a new Frame. The Antenna composes a frame made up  |
// | of 25 RBs. The antenna serves its users using an opportunistic policy: backlogged |
// | users are served by decreasing CQI. When a user is considered for service, its    |
// | queue is emptied, if the number of unallocated RBs is large enough.               |
// +-----------------------------------------------------------------------------------+

    // Opportunistic policy: users are served by decreasing CQI -> CQIs has to be sorted
    // in a decreasing order.
    std::sort(CQIs.begin(), CQIs.end(), [](CQIPacket *X, CQIPacket *Y){ return X->getCQI()>Y->getCQI();});

    // Once std::vector CQIs has been sorted the Antenna can pack a new Frame.
    int remainingRBs = 25, currentUser, queuedBytes, currentCQI;

    // To store the total number of bytes sent in a timeslot.
    int bytesToSend = 0;

    // Frame to send
    Frame* frameToSend = new Frame();

    for(int i = 0; i < population; i++)
    {
        // Retrieving the user under service.
        currentUser = CQIs[i]->getId();

        // Retrieving the total number of bytes that the current user has in his/her queue.
        queuedBytes = queuedBytesById(currentUser);

        // The current user has an empty queue.
        if(queuedBytes == 0)
        {
            // DEBUG: begin
            EV << "Antenna::handleFrame() - user " << currentUser << " has an empty queue!" << endl;
            // DEBUG: end
            continue;
        }

        // DEBUG: begin
        if(queuedBytes == -1)
        {
            EV << "Antenna::handleFrame() - FATAL ERROR - user " << currentUser << " not recognized!" << endl;
            continue;
        }
        // DEBUG: end

        // Retrieving the CQI sent by the current user.
        currentCQI = CQIs[i]->getCQI();

        // Number of required RBs.
        int numRBs = queuedBytes/CQI_to_BYTES(currentCQI);

        // Checking if another RB is needed.
        if(queuedBytes > numRBs*CQI_to_BYTES(currentCQI))
            numRBs++;

        // An user can be served if and only if the number of unallocated RBs is large enough.
        if(numRBs > remainingRBs)
        {
            // DEBUG: begin
            EV << "Antenna::handleFrame() - user " << currentUser << " cannot be served! remainingRBs=" << remainingRBs <<", currentCQI=" << currentCQI << "(" << CQI_to_BYTES(currentCQI) << " bytes), queuedBytes=" << queuedBytes << endl;
            // DEBUG: end
            continue;
        }

        remainingRBs -= numRBs;

        bytesToSend += allocateRBs(getQueueById(currentUser)->getQueue(), frameToSend, currentCQI, currentUser);
    }

    // Recording throughtput's statistics
    emit(throughputSignal, bytesToSend);
}

int Antenna::queuedBytesById(int id)
{
    for(UserQueue *currentUser : userQueues)
    {
        if(currentUser->getId() == id)
            return currentUser->queuedBytes();
    }
    return -1;
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

int Antenna::allocateRBs(std::vector<std::pair<simtime_t,int>>* currentQueue, Frame* frameToSend, int currentCQI, int currentUser)
{
    int bytesToSend = 0;

    do {
        int allocatedBytes = 0;

        int currentPacketDimension = currentQueue->begin()->second;

        if(currentPacketDimension > CQI_to_BYTES(currentCQI))
        {
            allocatedBytes = CQI_to_BYTES(currentCQI);
            currentQueue->begin()->second -= allocatedBytes;
        }
        else
        {
            allocatedBytes = currentPacketDimension;
            currentQueue->erase(currentQueue->begin());
            sentPackets += 1;
        }

        //Allocating a new RB.
        frameToSend->addRB(currentUser, CQI_to_BYTES(currentCQI), allocatedBytes);

        // DEBUG: begin
        EV << "Antenna::handleFrame() - A new RB has been allocated - currentCQI=" << currentCQI << "(" << CQI_to_BYTES(currentCQI) << " bytes), allocated bytes=" << allocatedBytes << endl;
        // DEBUG: end

        // Updating the total number of bytes sent in a timeslot.
        bytesToSend += allocatedBytes;

    } while(!currentQueue->empty());

    return bytesToSend;
}

void Antenna::finish()
{
// +-----------------------------------------------------------------------------+
//  Packet loss (%): % of packets that cannot be queued
    if(par("stage").intValue() == 1)
        recordScalar("PacketLoss%", (lostPackets/sentPackets)*100);
// +-----------------------------------------------------------------------------+
}

}
