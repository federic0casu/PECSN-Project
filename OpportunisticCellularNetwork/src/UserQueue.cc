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
    this->queueDimension = queueDimension;
    this->numberOfPacketsQueued = 0;
// +-------------------------------------------------------------------------------+
}

UserQueue::~UserQueue()
{
    queue.clear();
}

int UserQueue::getId() { return id; }

void UserQueue::addPacket(int bytes)
{
    std::pair<simtime_t, int> packet(simTime(), bytes);
    queue.push_back(packet);
}

int UserQueue::queuedBytes()
{
    int numberOfBytes = 0;
    for(int i = 0; i < queue.size(); i++)
        numberOfBytes += queue[i].second;
    return numberOfBytes;
}

std::vector<std::pair<simtime_t,int>>* UserQueue::getQueue()
{
    return &queue;
}


void UserQueue::showQueue() {

    EV << "Queue user" << getId() << "[";

    for(int i = 0; i < queue.size(); i++) {
        EV <<", "<<  queue[i].second ;
    }

    EV << "] Queue end" << endl;

}


} /* namespace opportunisticcellularnetwork */

