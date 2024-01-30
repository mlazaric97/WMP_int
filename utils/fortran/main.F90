program main
        use WMP_mod, only: helloworld, wmp_read
        use neutron_mod, only: neutron_t
        use iso_c_binding
   
        implicit none

        character(kind=c_char) :: filnam*9
        type(neutron_t) :: nn
        !real(kind=c_double) :: T


        filnam = "092238.h5"
        print *, "Fortran implementation of WMP" 
        call wmp_read(filnam)

        call nn%create(filnam)



end program main

