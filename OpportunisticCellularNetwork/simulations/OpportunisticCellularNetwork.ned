package opportunisticcellularnetwork.simulations;

import opportunisticcellularnetwork.Antenna;
import opportunisticcellularnetwork.Cellular;
import opportunisticcellularnetwork.Source;


network OpportunisticCellularNetwork
{
    parameters:
        int population = default(1);
    submodules:
        antenna    			 : Antenna;
        cellulars[population]: Cellular;
        sources[population]  : Source;
}
