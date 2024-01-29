

#include "WMP.hpp"
#include "Neutron.hpp"



const std::string FILE_NAME("092238.h5");
const std::string BASE_GROUP("/");
const std::string ISO_GROUP("U238");



int main()
{

	wmp_read("compiles");
	H5::H5File file = openhdf(FILE_NAME);

	Neutron nn(FILE_NAME); 
	return 0; 
}
