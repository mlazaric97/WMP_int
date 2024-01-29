

#include "WMP.hpp"
#include "Neutron.hpp"



const std::string FILE_NAME("092238.h5");
const std::string BASE_GROUP("/");
const std::string ISO_GROUP("U238");


int main()
{

	
	char* filename = "092238.h5";
	wmp_read("compiles");
	H5::H5File file = openhdf(filename);

	Neutron nn(filename); 
	return 0; 
}
