#include "neutron.hpp"

extern "C" { 
	void Neutron__new_c(Neutron *&This, char* filename) { 
		This = new Neutron(filename); 
	}

	double Neutron__grab_curvefit_c(Neutron *&This, int w_id, int order, int rxn)
	{
		return This->grab_curvefit(w_id,order,rxn); 
	}
	
	std::complex<double> Neutron__grab_data_c(Neutron *&This, int w_id, int rxn) 
	{
		return This->grab_data(w_id,rxn);
	}
	
	int* Neutron__grab_windows_c(Neutron *&This, int w_id)
	{
		int a[2]{*(This->grab_windows(w_id))}; 
		return a; 
	}

	int Neutron__grab_broaden_poly_c(Neutron *&This, int w_id)
	{
		return This->grab_broaden_poly(w_id);
	}

	int Neutron__grab_order_c(Neutron *&This)
	{
		return This->grab_order(); 
	}

	int Neutron__grab_fissionable_c(Neutron *&This)
	{
		return This->grab_fissionable();
	}

	double Neutron__grab_E_max_c(Neutron *&This)
	{
		return This->grab_E_max();
	}

	double Neutron__grab_E_min_c(Neutron *&This)
	{
		return This->grab_E_min(); 
	}

	double Neutron__grab_spacing_c(Neutron *&This)
	{
		return This->grab_spacing();
	}

	double* Neutron__xs_c(Neutron *&This,double E, double Temp)
	{
		return This->xs(E,Temp); 
	}

/*
	double** Neutron__WMP_c(Neutron *&This, double* E, double Temp)
	{

		return This->WMP(E,Temp); 

	}
*/
}

/* reference example
extern "C" {

	void Neutron__new_c(Neutron *&This, int a, int b) {
		This = new Neutron(a,b);
	}

	int Neutron__get_foo_c(Neutron *&This) {
		return This->get_foo();
	}
	
	int Neutron__get_bar_c(Neutron *&This) {
		return This->get_bar();
	}

	void Neutron__set_foo_c(Neutron *&This, int a) {
		This->set_foo(a); 
	}

	void Neutron__set_bar_c(Neutron *&This, int b) {
		This->set_bar(b); 
	}

	void Neutron__delete_c(Neutron *&This) {
		delete This;
	}

}
*/
