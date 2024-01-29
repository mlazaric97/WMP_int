program main
        use WMP_mod, only: helloworld, wmp_read
        use neutron_mod, only: neutron_t

        use iso_c_binding
        
        !character(80) :: s
        character(kind=c_char,len=*) :: s
        type(neutron_t) :: nn


        print *, "Fortran implementation of WMP" 
        s = "092238.h5"
        print *, "Opening file: " 
       ! call wmp_read(s)

        call nn%create(s)



end program main

