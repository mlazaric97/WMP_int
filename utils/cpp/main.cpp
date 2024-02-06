

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

	int i{1}; 

	int* j; 
	j = nn.grab_broaden_poly(&i); 

	std::cout << "j = " << *j << std::endl; 
	std::cout << "order = " << nn.grab_order() << std::endl; 	
	return 0; 
}
