#ifndef __OPPORTUNISTICCELLULARNETWORK_CQIPACKET_H_
#define __OPPORTUNISTICCELLULARNETWORK_CQIPACKET_H_

namespace opportunisticcellularnetwork {

class CQIPacket {
private:
    int id;
    int CQI;
public:
    CQIPacket(int, int);
    virtual ~CQIPacket();
    virtual int getId();
    virtual int getCQI();
};

}

#endif /* __OPPORTUNISTICCELLULARNETWORK_CQIPACKET_H_ */
