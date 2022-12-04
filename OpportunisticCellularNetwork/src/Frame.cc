#include "Frame.h"

namespace opportunisticcellularnetwork {

Frame::Frame()
{
}

Frame::~Frame()
{
}

int Frame::getUsedRB() {
    return usedRB;
}

void Frame::addRB(int id, int dimension, int allocatedBytes)
{
    // push rb into frame
    frame.push_back(new RB(id, dimension, allocatedBytes));
    // update used RBs
    usedRB++;
}

//Erica:
//Get cellular informations from position in vector
int Frame::getIdRB(int pos)
{
    return frame[pos]->getId();
}

int Frame::getDimRB(int pos)
{
    return frame[pos]->getDim();
}

int Frame::getBytesRB(int pos)
{
    return frame[pos]->getBytes();
}


} /* namespace opportunisticcellularnetwork */
