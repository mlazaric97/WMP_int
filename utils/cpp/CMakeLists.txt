add_executable(WMP_cxx.x main.cpp)

target_link_libraries(WMP_cxx.x 
	WMP_CXX
	${HDF5_LIBRARIES}
	${HDF5_CXX_LIBRARIES}
	)

#target_include_directories(WMP_cxx.x 
#	PRIVATE 
#	${HDF5_INCLUDE_DIRS}		
#	)
