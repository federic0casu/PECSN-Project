package opportunisticcellularnetwork.simulations;

import opportunisticcellularnetwork.Antenna.*;
import opportunisticcellularnetwork.Cellular.*;
import opportunisticcellularnetwork.Source.*;


network OpportunisticCellularNetwork
{
    parameters:
        int population;
        @display("bgb=,,#DDDDDD,,1");
    submodules:
        antenna: Antenna {
            @display("i=device/antennatower,,0;is=vl;p=298,73");
        }
        cellular[population]: Cellular {
            id = index;
            @display("i=device/cellphone;is=s;p=468,125");
        }
        source[population]: Source {
            id = index;
            @display("i=device/server2;is=n;p=96,142");
        }
    connections:
        for i=0..(population-1) {
            cellular[i].out --> antenna.inCellular++;
            source[i].out --> antenna.inSource++;
            antenna.out++ --> cellular[i].in;
        }
}
