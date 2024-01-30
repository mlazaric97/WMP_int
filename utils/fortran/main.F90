program main
        use WMP_mod, only: helloworld, wmp_read
        use neutron_mod, only: neutron_t
        use iso_c_binding
   
        implicit none

        character(80) :: filename
       
        filename = "092238.h5"
        print *, "Fortran implementation of WMP" 
        call wmp_read(filename)

!        call nn%create(filename)



end program main

