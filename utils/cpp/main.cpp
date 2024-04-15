

#include "WMP.hpp"
#include "Neutron.hpp"



const std::string FILE_NAME("092238.h5");
const std::string BASE_GROUP("/");
const std::string ISO_GROUP("U238");


int main()
{

	
	char* filename = "092235.h5";
	wmp_read("compiles");
	H5::H5File file = openhdf(filename);

	Neutron nn(filename); 

	int i{1}; 

	int* j; 
	j = nn.grab_broaden_poly(&i); 

	std::cout << "j = " << *j << std::endl; 
	std::cout << "order = " << nn.grab_order() << std::endl; 	
	double E{25.0E-6};
	double T{0.};

	std::cout << nn.xs(E,T)[0] << " " << nn.xs(E,T)[1] << " " << nn.xs(E,T)[2] << std::endl;  

	std::cout << "testing libcerf" << std::endl; 
	std::complex<double> cmp{1,1};
	std::cout << w_of_z(cmp) << std::endl; 



	return 0; 
}
