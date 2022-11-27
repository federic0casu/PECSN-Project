#ifndef __OPPORTUNISTICCELLULARNETWORK_RB_H_
#define __OPPORTUNISTICCELLULARNETWORK_RB_H_

namespace opportunisticcellularnetwork {

class RB {
private:
    int id;
    int dimension;
    int allocatedBytes;
public:
    RB(int, int, int);
    virtual ~RB();
};

} /* namespace opportunisticcellularnetwork */

#endif /* __OPPORTUNISTICCELLULARNETWORK_RB_H_ */
// <3
