#include "UserQueue.h"

using namespace omnetpp;

namespace opportunisticcellularnetwork {

UserQueue::UserQueue(int id)
{
    this->id = id;
}

UserQueue::UserQueue(int id, int queueDimension)
{
    this->id = id;
// +-------------------------------------------------------------------------------+
//  Scenario: queues of fixed dimension.
    this->freeSlots = this->queueDimension = queueDimension;
// +-------------------------------------------------------------------------------+
}

UserQueue::~UserQueue()            { queue.clear();}

int UserQueue::getId()             { return id;}

int UserQueue::getQueueDimension() { return queueDimension;}

int UserQueue::getFreeSlots()      { return freeSlots;}

void UserQueue::resetQueue()       { freeSlots = queueDimension;}

void UserQueue::addPacket(int bytes)
{
    std::pair<simtime_t, int> packet(simTime(), bytes);
    queue.push_back(packet);

    freeSlots--;
}

void UserQueue::showQueue()
{
    EV << "Queue user" << getId() << ": [";

    for(int i = 0; i < queue.size(); i++) {
        if(i == 0) EV << queue[i].second;
        else EV <<", "<< queue[i].second;
    }

    EV << "]" << endl;
}

std::vector<std::pair<simtime_t,int>>* UserQueue::getQueue()
{
    return &queue;
}


} /* namespace opportunisticcellularnetwork */

