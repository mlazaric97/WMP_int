
Neutron(std::string filename, int verbose = 0)
{
	// open file
	H5::H5File file = openhdf(filename);	
	// find atomic symbol and number 
	this->name = get_symb(file); 

	// open Isotope Group
	H5::Group isogroup = open_isogroup(name,file); 
	// populate class from Isotope Group
	
	// Could combine these all into one function "get_h5" not neccesary as of yet	
	this->broaden_poly = get_bp(isogroup);
	this->curvefit = get_curvefit(isogroup); 
	this->order = curvefit[0].size();
	
	

	std::cout << "Order of curvefit = " << this->order << std::endl; 
	
	this->E_min = get_E_bounds(isogroup,"E_min");
       	this->E_max = get_E_bounds(isogroup,"E_max"); 	
	this->data = get_data(isogroup); 
	if (data[0].size() == 4) this->fissionable = 1; else this->fissionable = 0; 

	this->windows = get_windows(isogroup); 
	this->spacing = get_spacing(isogroup); 
	std::cout << "Finished loading in data for: '" << this->name << "'\n";	
	// close file
	file.close();
	// I think this is all we need to do to make sure there are no memory leaks from reading in data
	// Any suggestions would be welcome
	if (verbose) 
	{
		std::cout << "Name = " << name << std::endl << "Order = " << order << std::endl;
		std::cout << "E_min = " << E_min << std::endl << "E_max = " << E_max << std::endl; 
		std::cout << "spacing = " << spacing << std::endl;
	}

}
