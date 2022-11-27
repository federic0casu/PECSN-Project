#include "RB.h"

namespace opportunisticcellularnetwork {

RB::RB(int id, int dimension, int allocatedBytes)
{
    this->id = id;
    this->dimension = dimension;
    this->allocatedBytes = allocatedBytes;
}

RB::~RB()
{
}

} /* namespace opportunisticcellularnetwork */
