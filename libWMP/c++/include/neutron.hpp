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
	double E_min,E_max,spacing,sqrtawr; 



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


	std::complex<double> cd_zero{0,0};
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
	double grab_sqrtawr(); 
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
	if (this->data.size() == 0) { return &cd_zero ;}// returning 0 for both will not add anything to pole sum when called
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

double Neutron::grab_sqrtawr()
{
	return this->sqrtawr;
}

// end grab_xxx definitions; 


std::array<double,3> Neutron::xs(double &energy, double &temperature) 
{
	
	double conv_fact = 1E+6; // energy is MeV from mcnp 
	double E = energy*conv_fact; // E is in eV from here on  
	if (E > E_max) return {0,0,0}; 
	double temp = temperature;
	double beta{}; 
	double kb = 8.6173303E-5; // boltzmann constant in eV/K, beta has units of sqrt(eV); 
//	std::cout << "E = " << E << std::endl; 
//	std::cout << "Temp = " << temp << std::endl; 
	
	double ss{},sa{},sf{},st{}; // intializing cross sections to zero, sa = sigma absorption
	double sqrtE = sqrt(E); 
	int window_i = floor((sqrtE - sqrt(E_min))/spacing);
	int startw = windows[window_i][0] - 1; 
	int endw   = windows[window_i][1];
	double pwr; 
	if (temp == 0. || not(broaden_poly[window_i]))
	{
		
		if ( not broaden_poly[window_i]) { std::cout << "BROADEN POLY ==== FALSE\n";}
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
	
//		std::cout << "holomorphic piece = {" << ss << ", " << sa << ", " << sf << " }\n"; 
		for (int i = startw; i < endw; ++i)
		{
			if (data.size() == 0) {break;} 
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
	}
	else
	{
		beta = sqrt(kb*temp)/sqrtawr;
		std::cout << "BETA ===== " << beta << std::endl; 
		std::complex<double> Dbm2, Dbm1, Db0, Db1, temp, holo; 
		Dbm2 = erf(sqrtE/beta)/E; 
		Dbm1 = 1./sqrtE;
		

		double sqrtpi = sqrt(M_PI); 
		Db0  = (pow(beta,2)/2 + E)*Dbm2 + beta/sqrtE/sqrtpi*exp(-(pow(sqrtE/beta,2))); // M_PI is defined by math
		
		Db1  = Dbm1*(pow(beta,2)/2.*(3.0)); // added this here because otherwise loop is more complicated
		std::vector<std::complex<double>> rcrsvDb{Dbm2,Dbm1,Db0,Db1};
		std::complex<double> imag(0,1); 
	

		std::cout << "db[2] = " << "\n" << rcrsvDb[2] << std::endl; 
		std::cout << "db[3] = " << "\n" << rcrsvDb[3] << std::endl; 
		for ( double n = 0.0; n < order - 4; ++n) // don't need this loop if order <= 4; 
		{
			temp = ( pow(beta,2)/2.0 * (2.0*n + 1) + E) * rcrsvDb[-2] - (pow(pow(beta,2),2)*n*(n-1)*rcrsvDb[-4] );
			rcrsvDb.push_back(temp);	
		}
		for (int i{}; i < order+1; ++i) // holomorphic part
		{
			ss += (curvefit[window_i][i][0]*rcrsvDb[i]).real();
			sa += (curvefit[window_i][i][1]*rcrsvDb[i]).real();
			if (fissionable) { sf += (curvefit[window_i][i][2]*rcrsvDb[i]).real(); }
		}
		temp = 0.0;
	
		for (int i = startw; i < endw; ++i )
		{
			temp = 1.0/E*sqrtpi*w_of_z((sqrtE - data[window_i][0])/beta)/imag/beta;
			ss += (data[i][1]*temp).real();
			sa += (data[i][2]*temp).real();
			if (fissionable) { sf += (data[i][3]*temp).real(); }
				
		}	

	}

	st = ss + sa ;
	//static std::array<double,3> sigmas{{st,sa,sf}};
	std::array<double,3> sigmas{{st,sa-sf,sf}};

	std::cout << "c++ returning the following array to fortan\n" << sigmas[0] << "\n" << sigmas[1] << "\n" << sigmas[2] << std::endl; 
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
