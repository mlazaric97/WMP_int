include(FetchContent)



find_package(HDF5 COMPONENTS C CXX)
find_package(cerf REQUIRED)
	

if (NOT ${HDF5_FOUND})
	FetchContent_Declare(
        hdf5
        GIT_REPOSITORY https://github.com/HDFGroup/hdf5
	GIT_TAG origin/develop
	)

	FetchContent_MakeAvailable(hdf5)
endif()



target_link_libraries(WMP_CXX
	${HDF5_LIBRARIES}
	cerf::cerfcpp
	)

target_include_directories(WMP_CXX 
	PUBLIC ${HDF5_INCLUDE_DIRS}	
	)
