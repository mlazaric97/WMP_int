program main
        use WMP_mod, only: helloworld, wmp_read
        use neutron_mod, only: neutron_t
        use iso_c_binding
   
        implicit none

        character(kind=c_char) :: filnam*9
        type(neutron_t) :: nn
        real(kind=c_double) :: energy, temperature
        real(kind=c_double) :: emax, emin, spac
        integer(c_int) :: window_id, window_value, ordr, &
                fiss, secid


        real(c_double) :: cf
        integer(c_int) :: wid, ord, rxn

        complex(c_double_complex) :: dat
        integer(c_int) :: win

        real(c_double), dimension(3) :: xsp
        real(c_double), pointer :: xss
        
        
        print *, "Fortran implementation of WMP" 
        
        print *, "FORTRAN CREATE NEUTRON CALL" 
        
        filnam = "092238.h5"
        call nn%create(filnam)
        print *, "FORTRAN BROADENPOLY CALL"
        
        window_id = 1

        window_value = nn%broadpoly(window_id)

        print *, window_value
        print *, "FORTRAN ORDER CALL" 
        
        ordr = nn%order()

        print *, ordr

        print *, "Fortran Fissionable call"

        fiss = nn%fissionable()

        print *, fiss

        print *, "FORTRAN E_MIN CALL" 
        
        emin = nn%E_min()

        print *, emin

        print *, "FORTRAN E_MAX CALL" 

        

        emax = nn%E_max()
        print *, emax
        
        print *, "FORTRAN SPACING CALL" 
        spac = nn%spacing()

        print *, spac

        print *, "FORTRAN GETCF CALL"
        wid = 1
        ord = 0
        rxn = 0

        cf = nn%getcf(wid,ord,rxn)
        print *, cf

        print *, "FORTRAN WINDOW CALL" 
        
        win = nn%window(wid,wid)
        print *, win
        
        win = nn%window(wid,ord)
        print *, win
        
        print *, "FORTRAN DATA CALL"

        dat = nn%pdata(wid,rxn)
        print *, dat


        print *, "FORTRAN XS CALL"
        energy = 25.
        temperature = 0
        xsp = nn%xs(energy,temperature)
        !call c_f_pointer(xsp,xss)
        print *, xsp        

end program main

