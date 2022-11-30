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
    int getIdRB(int);   //E
    int getDimRB(int);  //E
    int getBytesRB(int);//E
    virtual ~Frame();
};

} /* namespace opportunisticcellularnetwork */

#endif /* __OPPORTUNISTICCELLULARNETWORK_FRAME_H_ */
