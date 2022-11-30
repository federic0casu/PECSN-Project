#include "RB.h"

namespace opportunisticcellularnetwork {

RB::RB(int id, int dimension, int allocatedBytes)
{
    this->id = id; // Cellular ID, recipient
    this->dimension = dimension;
    this->allocatedBytes = allocatedBytes;
}

// Get cellular informations from position in vector
int RB::getId()
{
    return this->id;
}

int RB::getDim()
{
    return this->dimension;
}

int RB::getBytes()
{
    return this->allocatedBytes;
}

RB::~RB()
{
}

} /* namespace opportunisticcellularnetwork */
