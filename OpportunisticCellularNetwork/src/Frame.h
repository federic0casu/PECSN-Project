#ifndef __OPPORTUNISTICCELLULARNETWORK_FRAME_H_
#define __OPPORTUNISTICCELLULARNETWORK_FRAME_H_

#include <vector>
#include "RB.h"
#include <omnetpp.h>

namespace opportunisticcellularnetwork {

class Frame;

}  // namespace opportunisticcellularnetwork


namespace opportunisticcellularnetwork {

class Frame : public ::omnetpp::cMessage {
protected:
    std::vector<RB*> frame;
    int usedRB = 0;
public:
    Frame();
    void addRB(int, int, int);
    int getUsedRB();
    int getIdRB(int);
    int getDimRB(int);
    int getBytesRB(int);
    virtual ~Frame();
};

}
 /* namespace opportunisticcellularnetwork */
#endif /* __OPPORTUNISTICCELLULARNETWORK_FRAME_H_ */


