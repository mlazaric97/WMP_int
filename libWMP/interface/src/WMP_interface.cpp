#include "WMP.hpp"

extern "C" {
	void helloworld_c() { 
		helloworld();
	}

	void wmp_read_c(char *filename) { 
		wmp_read(filename);
	}	
}
