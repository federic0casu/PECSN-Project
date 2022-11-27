#include "Frame.h"

namespace opportunisticcellularnetwork {

Frame::Frame()
{
}

Frame::~Frame()
{
}

void Frame::addRB(int id, int dimension, int allocatedBytes)
{
    frame.push_back(new RB(id, dimension, allocatedBytes));
}

} /* namespace opportunisticcellularnetwork */
