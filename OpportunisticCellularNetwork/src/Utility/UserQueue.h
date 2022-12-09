#ifndef __OPPORTUNISTICCELLULARNETWORK_USERQUEUE_H_
#define __OPPORTUNISTICCELLULARNETWORK_USERQUEUE_H_

#include <utility>
#include <vector>
#include <omnetpp.h>

using namespace omnetpp;

namespace opportunisticcellularnetwork {

class UserQueue {
private:
    int id;
// +-------------------------------------------------------------------------------+
//  Scenario: queues of fixed dimension.
    int queueDimension;
    int freeSlots;
// +-------------------------------------------------------------------------------+
    std::vector<std::pair<simtime_t,int>> queue;
public:
    UserQueue(int);
    UserQueue(int, int);
    virtual int getId();
    virtual int getQueueDimension();
    virtual int getFreeSlots();
    virtual void addPacket(int);
    virtual void showQueue();
    virtual void resetQueue();
    virtual std::vector<std::pair<simtime_t,int>>* getQueue();
    virtual ~UserQueue();
};

}

#endif /* __OPPORTUNISTICCELLULARNETWORK_USERQUEUE_H_ */
