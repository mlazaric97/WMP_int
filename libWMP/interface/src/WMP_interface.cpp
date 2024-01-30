#include "WMP.hpp"

extern "C" {
	void helloworld_c() { 
		helloworld();
	}

	void wmp_read_c(std::string filename) { 
		wmp_read(filename);
	}	
}
