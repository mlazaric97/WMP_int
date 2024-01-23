module neutron_mod
        use, intrinsic :: iso_c_binding
        implicit none

        type,public :: neutron_t
                type(c_ptr) :: this = c_null_ptr
        contains
                procedure, public :: create => neutron_create 
                procedure, public :: broadpoly => neutron_broaden_poly 
                procedure, public :: order => neutron_order
                procedure, public :: fissionable => neutron_fissionable
                procedure, public :: E_min => neutron_E_min
                procedure, public :: E_max => neutron_E_max
                procedure, public :: spacing => neutron_spacing 
                procedure, public :: delete => neutron_delete
        end type neutron_t


        interface 
                subroutine neutron__new(this,a) bind(c,name="Neutron__new_c")
                        use, intrinsic :: iso_c_binding, only : c_ptr, c_char
                        implicit none
                        type(c_ptr), intent(inout) :: this
                        character(kind=c_char), dimension(*) :: a
                end subroutine 
        
                function neutron__get_bp(this) bind(c, &
                                name="Neutron__grab_broaden_poly_c") result(bpval)
                        use, intrinsic :: iso_c_binding, only : c_ptr, c_int
                        implicit none 
                        type(c_ptr), intent(inout) :: this 
                        integer(c_int) :: bpval
                end function 

                function neutron__get_order(this) bind(c, &
                                name=("Neutron__grab_order_c") result(order)
                        use, intrinsic :: iso_c_binding, only : c_ptr, c_int
                        implicit none
                        type(c_ptr), intent(inout) :: this
                        integer(c_int) :: order 
                 end function

                function neutron__get_fis(this) bind(c, &
                               name=("Neutron__grab_fissionable_c") result(fis)
                      use, intrinsic :: iso_c_binding, only : c_ptr, c_int
                      implicit none
                      type(c_ptr), intent(inout) :: this
                      integer(c_int) :: fis
                end function 

                function neutron__get_Emax(this) bind(c, &
                                name=("Neutron__grab_E_max_c") result(emx)
                        use, intrinsic :: iso_c_binding, only c_ptr, c_double
                        implicit none
                        type(c_ptr), intent(inout) :: this
                        real(kind=c_double) :: emx
                end function

                function neutron__get_Emin(this) bind(c, &
                                name=("Neutron__grab_E_min_c") result(emn)
                        use, intrinsic :: iso_c_binding, only c_ptr, c_double
                        implicit none
                        type(c_ptr), intent(inout) :: this 
                        real(kind=c_double) :: emn
                end function

                function neutron__get_spacing(this) bind(c, &
                                name=("Neutron__grab_spacing_c") result(spac)
                        use, intrinsic :: iso_c_binding, only c_ptr, c_double
                        implicit none
                        type(c_ptr), intent(inout) :: this
                        real(kind=c_double) :: spac
                end function


! functions left to define
! double Neutron__grab_curvefit_c(Neutron *&This, int w_id, int order, int rxn)
! std::complex<double> Neutron__grab_data_c(Neutron *&This, int w_id, int rxn)
! int* Neutron__grab_windows_c(Neutron *&This, int w_id)
! Neutron__xs_c














































