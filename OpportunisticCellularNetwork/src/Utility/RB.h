#ifndef __OPPORTUNISTICCELLULARNETWORK_RB_H_
#define __OPPORTUNISTICCELLULARNETWORK_RB_H_

#include <omnetpp.h>
#include <vector>

using namespace omnetpp;

namespace opportunisticcellularnetwork {

class RB {
private:
    int id;
    int dimension;
    int allocatedBytes;
    std::vector<std::pair<simtime_t, int>> chunks;
public:
    RB(int, int, int, std::vector<std::pair<simtime_t, int>>);
    int getId();
    int getDim();
    int getAllocatedBytes();
    std::vector<std::pair<simtime_t, int>> getChunks();
    virtual ~RB();
};

} /* namespace opportunisticcellularnetwork */

#endif /* __OPPORTUNISTICCELLULARNETWORK_RB_H_ */
// <3
