#ifndef __OPPORTUNISTICCELLULARNETWORK_FRAME_H_
#define __OPPORTUNISTICCELLULARNETWORK_FRAME_H_

#include <vector>
#include "RB.h"

namespace opportunisticcellularnetwork {

class Frame {
private:
    std::vector<RB*> frame;
public:
    Frame();
    void addRB(int, int, int);
    virtual ~Frame();
};

} /* namespace opportunisticcellularnetwork */

#endif /* __OPPORTUNISTICCELLULARNETWORK_FRAME_H_ */
