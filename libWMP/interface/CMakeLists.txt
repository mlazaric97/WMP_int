add_library(WMP_int STATIC "")

target_sources(WMP_int
	PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/src/WMP_interface.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/Neutron_interface.cpp"
	)
target_link_libraries(WMP_int
	PRIVATE
	WMP_CXX
	)

#target_include_directories(WMP_int
#	PUBLIC	${CMAKE_INCLUDE_DIRECTORIES}
#	)
