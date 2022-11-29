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
    int numberOfPacketsQueued;
// +-------------------------------------------------------------------------------+
    std::vector<std::pair<simtime_t,int>> queue;
public:
    UserQueue(int);
    UserQueue(int, int);
    virtual int getId();
    virtual int getQueueDimension();
    virtual void addPacket(int);
    virtual int queuedBytes();
    virtual void showQueue();
    virtual std::vector<std::pair<simtime_t,int>>* getQueue();
    virtual ~UserQueue();
};

}

#endif /* __OPPORTUNISTICCELLULARNETWORK_USERQUEUE_H_ */
