package opportunisticcellularnetwork;


simple Cellular
{
    parameters:
        int id;
        bool typeCQI = default(false); 		//default: uniform
        int binomial_n = default(0); 		// param n for binomial distribution
        int binomial_p = default(0); 		// param p for binomial distribution
        int uniform_r = default(0); 		// range for uniform random generator 
        
    gates:
        input in;
        output out;
}
