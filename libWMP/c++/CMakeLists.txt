add_library(WMP_CXX STATIC "")

add_subdirectory(dependencies)



target_include_directories(WMP_CXX PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
	$<INSTALL_INTERFACE:include/WMP>
	)

target_sources(WMP_CXX 
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/src/WMP.cpp
)



