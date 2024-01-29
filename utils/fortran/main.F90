program main
        use WMP_mod, only: helloworld, wmp_read
        use neutron_mod, only: neutron_t
        use iso_c_binding
   
        implicit none
        !character(80) :: s
        !character(kind=c_char,len=72) :: s
!        character(*) :: s
        character(kind=c_char) :: s*90

        type(neutron_t) :: nn


        print *, "Fortran implementation of WMP" 
        s = "092238.h5"
        call wmp_read(s)

        call nn%create(s)



end program main

