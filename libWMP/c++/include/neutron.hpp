#ifndef __NEUTRON_HPP__
#define __NEUTRON_HPP__


// Class definition for WMP evaluation for incident neutron
// Used MIT's WMP_library as inpsiration for setup of problem
// Member functions will be able to : 
// 	- calculate cross section at specified temperature
// 	- Windowing optional to allow for calculation of sensitivities
// 	- calculate sensitivities to Real(pole_j) Imag(pole_j) |r_j|, 
// 	- Re(r_j), Im(r_j), a_c, 
#include "WMP.hpp"
#include "h5reader.hpp"

class Neutron { 
	private: 
	std::vector<std::vector<std::vector<double>>> curvefit;
	std::vector<std::vector<std::complex<double>>> data; 	
	std::vector<std::vector<int>> windows;
//	std::vector<float> ceoff; commented out as I think this is a mistake
       	std::vector<int> broaden_poly; 	
	std::string name;  
	int order,fissionable; 
	double E_min,E_max,spacing;



	// Description of Member variales;
	// Curvefit - holomorphic peice of the cross section
	// Curvefit[i] - curve fit polynomials for the i'th window  
	// Curvefit[i][j] - curve fit polynomials for the E^(-1 +j)'th order term for total, abs, fiss xs's 
	// 
	// data - Contains the poles and residues 
	// data[i][0] -  retrieves the i'th pole
	// data[i][1:]- retrieves the residues total, absorption, fission 
	//
	//
	// 
	// windows - list of integer pairs dictating the indicies of each window 
	// windows[i] = [j,k] where j and k represent the first and last index of the windows respectively


	public:
	
	// accessor functions
	double* grab_curvefit(int* window_id, int* order, int* rxn);
	std::complex<double>* grab_data(int* window_id, int* rxn);
	int*   grab_windows(int* window_id, int* sec_id); // goal for this one is to call a window and a size 2 array of ints
	int*   grab_broaden_poly(int* window_id); // implemented
	int    grab_order(); // implemented
	int    grab_fissionable(); //  
	double grab_E_min(); // 
	double grab_E_max(); //
	double grab_spacing(); 

	// Current ctor takes the filename as a string, 
	// To do: copy constructor, more options for constructor (i.e. H5File arg etc.)
	
	#include "ctor/ctor.cpp"
	// data setup as in WMP_library for convenience
	// data[i][0] is a pole data[i][1:] are the partial residues
	std::array<double,3> xs(double&, double&); // returns pointer to double array of xs at given E and T
						// might make it private then only access it through WMP 

	// not working right now due to compatibility issues 
	//double* WMP(double* Energies, double temp,const int E_len); // returns a pointer to a pointer to a double 
	
	// futher explanation of WMP for future ref
	// just as int* a can point to the start of an array of ints 
	// int** can point to the start of an array of pointers that each points to the start of an array of ints
	// int** a; 
	// *a[0] = address of the first pointer in the array of point
	// *(*a[0])[0] = int at the start of the first array in the array of arrays that a points to 

}; 

// grab_xxx defintions
// 

double* Neutron::grab_curvefit(int *window_id, int *order, int *rxn)
{
	return &(this->curvefit[*window_id][*order][*rxn]); 
}

std::complex<double>* Neutron::grab_data(int* window_id, int* rxn) 
{
	return &(this->data[*window_id][*rxn]);
}

int*   Neutron::grab_windows(int* window_id, int* sec_id)
{	
	return &(this->windows[*window_id][*sec_id]); 
}

int*    Neutron::grab_broaden_poly(int* window_id)
{
	//std::cout << "here 2" << std::endl; 
//	std::cout << window_id << std::endl; 
	
	return &(this->broaden_poly[*window_id]);
}

int    Neutron::grab_order()
{
	return this->order; 
}

int    Neutron::grab_fissionable()
{
	return this->fissionable;
}

double Neutron::grab_E_min()
{
	return this->E_min; 
}

double Neutron::grab_E_max() 
{
	return this->E_max; 
}

double Neutron::grab_spacing() 
{
	return this->spacing;
}

// end grab_xxx definitions; 


std::array<double,3> Neutron::xs(double &energy, double &temperature) 
{
	
	double conv_fact = 1E+6; // energy is MeV from mcnp 
	double E = energy*conv_fact; // E is in eV from here on  
	if (E > E_max) return {0,0,0}; 
	double temp = temperature;
	std::cout << "E = " << E << std::endl; 
//	std::cout << "Temp = " << temp << std::endl; 
	
	double ss{},sa{},sf{},st{}; // intializing cross sections to zero, sa = sigma absorption
	double sqrtE = sqrt(E); 
	int window_i = floor((sqrtE - sqrt(E_min))/spacing);
	int startw = windows[window_i][0] - 1; 
	int endw   = windows[window_i][1];
	double pwr; 
	if (temp == 0. || not(broaden_poly[window_i]))
	{
		pwr = 1./E; 
		for (int j{}; j <= order; ++j)
		{
			ss += curvefit[window_i][j][0]*pwr; 
			sa += curvefit[window_i][j][1]*pwr; 
			if (fissionable){ 
				sf += curvefit[window_i][j][2]*pwr; 
			}
			pwr *= sqrtE; 
		}
		for (int i = startw; i < endw; ++i)
		{
			std::complex<double> j(0,-1); 
			std::complex<double> temp,common; 
			temp = -(sqrtE - data[i][0]);
			common = j/temp/E; 
			ss += (data[i][1]*common).real(); 
			sa += (data[i][2]*common).real();
			if (fissionable) {
				sf += (data[i][3]*common).real();
			}

		}
		std::cout << "holomorphic piece = {" << ss << ", " << sa << ", " << sf << " }\n"; 
	}
	else
	{
		std::cout << "ERROR: DOPPLER BROADENED CROSS SECTIONS ARE A WORK IN PROGRESS\n SET TEMPERATURES TO 0K AND TRY AGAIN" << std::endl;
	}

	st = ss + sa + sf;
	//static std::array<double,3> sigmas{{st,sa,sf}};
	std::array<double,3> sigmas{{st,sa,sf}};

//	std::cout << "c++ returning the following array to fortan\n" << sigmas[0] << "\n" << sigmas[1] << "\n" << sigmas[2] << std::endl; 
	return sigmas; 
}


/*
double* Neutron::WMP(double* E, double T,const int length_E)
{
	
	static double* xss[3][length_E]; //
	
	for (int i{}; i<length_E; ++i)
	{
		double* xs_vec[3]{xs(*(E+i),T)}; 
		xss[i] = xs_vec; 

	}
	return xss; 
	/*
	int l = ceil(;
	std::vector<std::vector<double>> xs_vec(l,std::vector<double>(3,0.0)); // same length as the E vector, 3 xs's per energy total, absorption
	double* xs_vec[3]; 								       // fission 
	
	for (int i{}; i < l; ++i)
	{
		xs_vec[i] = xs(E[i],T); 
	}
	return xs_vec; 
}

*/



#endif //__NEUTRON_HPP__
