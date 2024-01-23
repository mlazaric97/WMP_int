#ifndef __H5READER_WMP__
#define __H5READER_WMP__

#include "H5Cpp.h"

typedef std::shared_ptr<H5::Group>  h5fileptr; 
typedef std::shared_ptr<H5::Group>  h5grpptr; 
//typedef std::shared_ptr<H5::DataSet> h5dsptr; 
// defining the following to read "data" 
typedef struct comp_type {
	double r;
	double i; 
	} comp_type; 

// A lot of this code is taken from the hdf5 C++ example "readdata.cpp"
// opens and returns a pointer to and hdf5 file denoted by FILENAME; 
H5::H5File openhdf(std::string FILENAME)
{	
	std::cout << "Opening data file: " << FILENAME << std::endl; 
	H5::Exception::dontPrint();
	
	H5::H5File f; 
	try { 
		f = H5::H5File(FILENAME.c_str(),H5F_ACC_RDONLY);
	} catch( const H5::FileIException& error) {	
		std::cout << "ERROR OPENING DATAFILE: " << FILENAME << std::endl;
		error.printErrorStack(); 
		std::abort(); 
	}

	std::cout << "Successfully opened: " << FILENAME << std::endl; 
	return f; 
}

std::string get_symb(H5::H5File &file)
{
	std::string sym; 
	ssize_t symsize = file.getObjnameByIdx(0,sym,5);
	std::cout << "Name = " << sym << std::endl; 
	return sym; 
}

// Opens the "/" group at root of all WMP_library files
// then opens the isotope group denoted by GROUPNAME and returns a pointer to it 
H5::Group open_isogroup(const std::string GROUPNAME,H5::H5File &file)
{
	std::cout << "Opening Group: '/'" << std::endl; 
	H5::Exception::dontPrint();
	H5::Group g, isog;
	try {
		g = file.openGroup("/"); 
	} catch( const H5::GroupIException& error) {
		std::cout << "ERROR OPENING BASE GROUP" << std::endl; 
		error.printErrorStack(); 
		std::abort(); 
	}
	std::cout << "Opened Group: '/' " << std::endl << "Opening Isotope Group\n";	
	try {
		 isog = g.openGroup(GROUPNAME.c_str());
	} catch( const H5::GroupIException& error) {
		std::cout << "ERROR OPENING ISOTOPE GROUP" << std::endl; 
		error.printErrorStack();
		std::abort();  
	}	
	std::cout << "Successfully opened Isotope Group: "<< GROUPNAME << std::endl; 
	
	return isog; 
}

double get_E_bounds(H5::Group &isogroup, std::string maxormin)
{
	std::cout << "Loading Dataset: '" << maxormin << "' \n"; 
	H5::Exception::dontPrint(); 
	H5::DataSet emin; 

	try {
		emin = isogroup.openDataSet(maxormin.c_str()); 
	} catch(const H5::DataSetIException& error) {
		std::cout << "ERROR OPENING DATASET: '" << maxormin << "' \n";
		error.printErrorStack();
		std::abort(); 
	}
	std::cout << "Successfully opened Dataset: '" << maxormin << "' \n";
	
	// E_min is a scalar double, don't have to worry about data dimensions
	double buf[1];
       	emin.read(buf,H5::PredType::NATIVE_DOUBLE);	
	std::cout << "Succesfully loaded data from Dataset: '" << maxormin << "'" << std::endl; 

	return buf[0]; 
}

double get_spacing(H5::Group &isogroup)
{
	std::cout << "Loading Dataset: 'spacing' \n";
	H5::Exception::dontPrint();
	H5::DataSet spacing;
	
	try { 
		spacing = isogroup.openDataSet("spacing");
	} catch(const H5::DataSetIException& error) {
		std::cout << "ERROR OPENING DATASET: 'spacing' \n";
		error.printErrorStack();
		std::abort(); 
	}
	double buf[1]; 
	spacing.read(buf,H5::PredType::NATIVE_DOUBLE);
	std::cout << "Succesfully loaded data from Dataset: 'spacing'" << std::endl; 

	return buf[0]; 
}

void get_E_bounds2(H5::Group &isogroup)
{
	std::cout << "Loading Dataset: 'E_max' \n";
	H5::Exception::dontPrint();

	H5::DataSet emax;
	try {	
		emax = isogroup.openDataSet("E_max"); 
	} catch(const H5::FileIException& error) {
		std::cout << "ERROR OPENING DATASET: 'E_max'\n";
		error.printErrorStack();
		std::abort();
	}	
	std::cout << "Successfully opened DataSet 'E_max' " << std::endl;
	std::cout << "Loading Dataset: 'E_min' \n"; 

	H5::DataSet emin; 

	try{ 
		emin = isogroup.openDataSet("E_min"); 
	} catch( const H5::FileIException& error) {
		std::cout << "ERROR OPENING DATASET: 'E_min'\n";
		error.printErrorStack();
		std::abort(); 
	}
	std::cout << "Successfully opened DataSet 'E_min'\n";


	// We know that E_max contains a single value of H5T_IEEE_F64LE
	double ebuf1[1],ebuf2[1]; 
	emax.read(ebuf1,H5::PredType::NATIVE_DOUBLE);	
	emin.read(ebuf2,H5::PredType::NATIVE_DOUBLE);
	std::cout << "Energy range: [" << *ebuf2 << ", " << *ebuf1 << "]" << std::endl; 
}

std::vector<int> get_bp(H5::Group &iso_group)
{
	std::cout << "Loading Dataset: 'broaden_poly'\n";
	H5::Exception::dontPrint(); 
	H5::DataSet bp; 

	std::string name = "broaden_poly";
	try{ 
		bp = iso_group.openDataSet(name.c_str()); 
	} catch( const H5::FileIException& error) {
		std::cout << "ERROR OPENING DATASET: 'broaden_poly'\n";
		error.printErrorStack();
		std::abort(); 
	}
	std::cout << "Succesfully opened DataSet 'broaden_poly'\n";
	// Size of broaden_poly dataset is based on number of windows and is not constant
	// To get data we need to grab dimensions of the dataset
	
	H5T_class_t type = bp.getTypeClass(); 
	if (type != H5T_INTEGER)
	{
		std::cout << "ERROR: INVALID DATA TYPE IN 'broaden_poly' DATASET\n";
		std::abort();  
	}
	
	H5::IntType inttype = bp.getIntType(); 
	// opening dataspace, contains dimensions of dataset
	
	H5::DataSpace dataspace = bp.getSpace();
	int rank = dataspace.getSimpleExtentNdims();
	hsize_t dims_out[2]; 

	int ndims = dataspace.getSimpleExtentDims(dims_out,NULL); 
	//std::cout << "'broaden_poly' rank: "<< rank << "\n";
	//std::cout << "'broaden_poly  length: " <<  (unsigned long)(dims_out[0]) << std::endl;

	// buffer initialization
	int buf[(unsigned long)(dims_out[0])]; 

	//define memory space definitions
	hsize_t dimsm[2];
	dimsm[0] = dims_out[0]; 
	dimsm[1] = rank;
	bp.read(buf,H5::PredType::NATIVE_INT);
 	std::vector<int> bpvec(dimsm[0],0);	
	for (long long unsigned int i{}; i < dims_out[0]; ++i) // i is this type to avoid error, could have also cast dims_out[0] to int
	{
		bpvec[i] = buf[i];
	}
		
	return bpvec; 

}

std::vector<std::vector<std::vector<double>>> get_curvefit(H5::Group &isogroup)
{
	int Nx,Ny,Nz; // data dimensions;
	std::string name = "curvefit";
	H5::DataSet crvft; 
	std::cout << "Opening DataSet: " << name << std::endl; 
	try {
		crvft = isogroup.openDataSet(name.c_str());
	} catch(const H5::DataSetIException& error)
	{
		std::cout << "ERROR OPENING DATASET: " << name << std::endl; 
		error.printErrorStack();
		std::abort(); 
	}
	std::cout << "Successfully opened DataSet: " << name << std::endl; 


	//opening dataspace
	H5::DataSpace ds = crvft.getSpace();
	//Checking that type is correct
	H5T_class_t type = crvft.getTypeClass();
	//if (type != H5T_FLOAT) { std::cout << "ERROR: WRONG DATA TYPE IN DATASET: '" << name << "'\n"; abort;}
	// ABOVE LINE WAS REMOVED AS I AM PLAYING WITH DATA TYPES
	// IDEAL WOULD BE FLOAT VS DOUBLE AGNOSTIC JUST SO WE CAN HAVE OPTIONS 
	
	// finding dimensions of the dataspace
	int rank = ds.getSimpleExtentNdims();	
	hsize_t dims_out[3]; 
	int ndims = ds.getSimpleExtentDims(dims_out,NULL); 
	/*
	   std::cout << "curvefit rank = " << rank << "\ncurvefit dimensions = " << (unsigned long)(dims_out[0]) 
		<< " x " << (unsigned long)(dims_out[1]) <<" x " <<  (unsigned long)(dims_out[2]) << 
		std::endl; 
	*/
	Nx = dims_out[0];
	Ny = dims_out[1]; 
	Nz = dims_out[2]; 

	hsize_t dimsm[3]; 
	dimsm[0] = Nx;
	dimsm[1] = Ny; 
	dimsm[2] = Nz; 
	//std::cout << "[" << Nx << ", " << Ny << ", " << Nz << "]\n";
	std::vector<std::vector<std::vector<double>>> curvefit; 

	double buf[Nx][Ny][Nz];// buffer for data in curvefit dataset  
	try{
		crvft.read(buf,H5::PredType::NATIVE_DOUBLE); 
	} catch(H5::DataSetIException& error){
		std::cout << "ERROR READING 'curvefit' DATA\n";
		error.printErrorStack();
		std::abort(); 
	}
	
	std::vector<std::vector<double>> temp3; 
	for (int i{}; i < Nx; ++i)
	{
		std::vector<std::vector<double>> temp2; 
		for (int j{}; j < Ny; ++j)
		{
			std::vector<double> temp; 
			for (int k{}; k < Nz; ++k)
			{
				
				temp.push_back(buf[i][j][k]);	
		//	std::cout << "(" << i << "," << j << "," << "0" << "):"; 
		//	std::cout << buf[i][j][0] << ", " << buf[i][j][1] << ",\n";
			}
			temp2.push_back(temp); 
		}
		curvefit.push_back(temp2);
	}
	std::cout << "Succesfully read DataSet: 'curvefit' " << std::endl; 
	return curvefit; 
}

std::vector<std::vector<std::complex<double>>> get_data(const H5::Group &isogroup)
{
	std::cout << "Loading DataSet: 'data'\n";
	std::string name = "data";
	int Nx,Ny; // dimensions of dataset
	H5::DataSet ds;
	try{ 
		ds = isogroup.openDataSet(name.c_str());
	} catch(const H5::DataSetIException& error){
		std::cout << "ERROR OPENING DATASET: 'data'\n";
		error.printErrorStack();
		std::abort();
	}
	std::cout << "Successfully opened DataSet: 'data'\n";

	// getting dataspace
	H5::DataSpace dataspace = ds.getSpace(); 
	
	H5::CompType  ct(sizeof(comp_type));
	
	ct.insertMember("r",HOFFSET(comp_type,r), H5::PredType::NATIVE_DOUBLE);
	ct.insertMember("i",HOFFSET(comp_type,i), H5::PredType::NATIVE_DOUBLE);
			
	H5std_string  r = ct.getMemberName(0); 
	H5std_string  i = ct.getMemberName(1);

	// getting dimensions
	int rank = dataspace.getSimpleExtentNdims();
	hsize_t dims[2];
	int ndims = dataspace.getSimpleExtentDims(dims,NULL);
	Nx = dims[0];
	Ny = dims[1]; 
	//std::cout << "[" << Nx << "," << Ny << "]\n";

	comp_type data_out[Nx][Ny];
	
	// writes data onto 'data_out'
	try{ 
		ds.read(&data_out,ct);
	} catch(const H5::DataSetIException& error) {
		std::cout << "ERROR READING DATASET 'data'\n";
		error.printErrorStack(); 
		std::abort(); 
	}
	std::vector<std::vector<std::complex<double>>> data(Nx, (std::vector<std::complex<double>>(Ny))) ;
	for (int i{}; i < Nx; ++i)
	{
		for (int j{}; j < Ny; ++j)
		{
			std::complex<double> a(data_out[i][j].r,data_out[i][j].i);
			data[i][j] = a; 
		}
	}
	//std::cout << "data[5][2] = " << data[5][2] << std::endl; 
	std::cout << "Successfully read and loaded DataSet: " << name << std::endl << std::endl;
	return data; 
}

std::vector<std::vector<int>> get_windows(H5::Group &iso_group)
{
	int Nx,Ny; 
	std::string name = "windows"; 
	std::cout << "Loading Dataset: 'windows' \n"; 
	H5::DataSet wind;
	try{
		wind = iso_group.openDataSet(name.c_str());
	} catch(const H5::DataSetIException& error) {
		std::cout << "ERROR LOADING DATASET: 'windows' \n";
		error.printErrorStack();
		std::abort(); 
	}

	// get dataspace 
	H5::DataSpace ds = wind.getSpace();
	// finding rank and dimensions
	int rank = ds.getSimpleExtentNdims();
	hsize_t dims[2]; 
	int ndims = ds.getSimpleExtentDims(dims,NULL);
	Nx = dims[0];
	Ny = dims[1]; 

	// dataset 'windows' is a set of integers
	int buf[Nx][Ny]; 

	try{
		wind.read(buf,H5::PredType::NATIVE_INT);
	} catch( H5::DataSetIException& error) {	
		std::cout << "ERROR READING DATASET: 'windows' \n";
		error.printErrorStack();
		std::abort();
	}
	std::vector<std::vector<int>> windows(Nx, (std::vector<int>(2))); 
	for (int i{}; i < Nx; ++i)
	{	
		windows[i][0] = buf[i][0]; 
		windows[i][1] = buf[i][1]; 
		

	}	
//	std::cout << "windows[22] = [" << windows[22][0] << ", " << windows[22][1] << "]" << std::endl;
	return windows; 
}

#endif // __H5READER_WMP__
