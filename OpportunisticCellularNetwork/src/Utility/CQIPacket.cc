#include "CQIPacket.h"

namespace opportunisticcellularnetwork {

CQIPacket::CQIPacket(int id, int CQI)
{
    this->id = id;
    this->CQI = CQI;
}

CQIPacket::~CQIPacket()
{
}

int CQIPacket::getId() { return id; }

int CQIPacket::getCQI() { return CQI; }

} /* namespace opportunisticcellularnetwork */
