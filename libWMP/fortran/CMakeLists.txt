add_library(WMP_Fortran STATIC "")

target_sources(WMP_Fortran 
	PUBLIC
	"${CMAKE_CURRENT_LIST_DIR}/src/WMP_mod.F90"
	"${CMAKE_CURRENT_LIST_DIR}/src/Neutron_mod.F90"
	)

target_link_libraries(WMP_Fortran
	PRIVATE
	WMP_int
)

#target_include_directories(WMP_int
#	PUBLIC ${CMAKE_INCLUDE_DIRECTORIES}
#	)
