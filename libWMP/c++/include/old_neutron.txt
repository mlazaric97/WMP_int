#ifndef __Neutron_HPP__
#define __Neutron_HPP__

#include "WMP.hpp"

class Neutron {

	
	    
    	int foo, bar;
  public:	
	double *curvefit[1000][4][11]; 
	// holds the curvfit data curvefit[pole_id][rxn_id][pole_id]
	std::complex<double> *data[4][4]; 
	// holds the poles and residues data[pole_id][0: pole, 1-3: residues]
	int windows[Nlamb][2];
	// holds the indices for the Energy ranges of the windows

	Neutron (int,int);
    	int get_foo ();
    	int get_bar ();
    	void set_foo (int);
    	void set_bar (int);
    	~Neutron ();

};

Neutron::Neutron (int a, int b) {
  foo = a;
  bar = b;

  helloworld();
}

int Neutron::get_foo () {
  return foo;
}

int Neutron::get_bar () {
  return bar;
}

void Neutron::set_foo (int a) {
  foo = a;
}

void Neutron::set_bar (int a) {
  bar = a;
}

Neutron::~Neutron () {
  helloworld();
}

#endif //__Neutron_HPP__
