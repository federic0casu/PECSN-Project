#include "RB.h"

namespace opportunisticcellularnetwork {

RB::RB(int id, int dimension, int allocatedBytes, std::vector<std::pair<simtime_t, int>> chunks)
{
    this->id = id;
    this->dimension = dimension;
    this->allocatedBytes = allocatedBytes;

    for(std::pair<simtime_t, int> c : chunks)
        this->chunks.push_back(c);
}

int RB::getId() { return this->id; }

int RB::getDim() { return this->dimension; }

int RB::getAllocatedBytes() {
    return this->allocatedBytes;
}

std::vector<std::pair<simtime_t, int>> RB::getChunks()
{
    return this->chunks;
}

RB::~RB(){}

} /* namespace opportunisticcellularnetwork */
