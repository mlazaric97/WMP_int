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
                                name="Neutron__grab_order_c") result(order)
                        use, intrinsic :: iso_c_binding, only : c_ptr, c_int
                        implicit none
                        type(c_ptr), intent(inout) :: this
                        integer(c_int) :: order 
                 end function

                function neutron__get_fis(this) bind(c, &
                               name="Neutron__grab_fissionable_c") result(fis)
                      use, intrinsic :: iso_c_binding, only : c_ptr, c_int
                      implicit none
                      type(c_ptr), intent(inout) :: this
                      integer(c_int) :: fis
                end function 

                function neutron__get_Emax(this) bind(c, &
                                name="Neutron__grab_E_max_c") result(emx)
                        use, intrinsic :: iso_c_binding, only : c_ptr, c_double
                        implicit none
                        type(c_ptr), intent(inout) :: this
                        real(kind=c_double) :: emx
                end function

                function neutron__get_Emin(this) bind(c, &
                                name="Neutron__grab_E_min_c") result(emn)
                        use, intrinsic :: iso_c_binding, only : c_ptr, c_double
                        implicit none
                        type(c_ptr), intent(inout) :: this 
                        real(kind=c_double) :: emn
                end function

                function neutron__get_spacing(this) bind(c, &
                                name="Neutron__grab_spacing_c") result(spac)
                        use, intrinsic :: iso_c_binding, only : c_ptr, c_double
                        implicit none
                        type(c_ptr), intent(inout) :: this
                        real(kind=c_double) :: spac
                end function

                function neutron__get_cf(this,wid,o,rxn) bind(c, &
                                name="Neutron__grab_curvefit_c") result(cf)
                        use, intrinsic :: iso_c_binding, only : c_ptr, c_double, c_int
                        implicit none 
                        type(c_ptr), intent(inout) :: this
                        integer(c_int), intent(in) :: wid ! stands for window_id \eps [0,N_windows)
                        integer(c_int), intent(in) :: o ! order 
                        integer(c_int), intent(in) :: rxn ! reaction id
                        real(kind=c_double) :: cf
                end function 

                function neutron__get_data(this, wid, rxn) bind(c, &
                                name="Neutron__grab_data_c") result(datum) 
                        use, intrinsic :: iso_c_binding, only : c_ptr, c_int, c_double_complex, 
                        implicit none
                        type(c_ptr), intent(inout) :: this
                        integer(c_int), intent(in) :: wid
                        integer(c_int), intent(in) :: rxn
                        complex(type=c_double_complex) :: datum
                end function 

                function neutron__get_windows(this,wid) bind(c, &
                                name="Neutron__grab_windows_c") result(pti) !Pointer To Int
                        use, intrinsic :: iso_c_binding, only : c_ptr, c_int
                        implicit none
                        type(c_ptr), intent(inout) :: this       
                        integer(c_int), intent(in) wid
                        type(c_int), pointer :: pti
                end function 

                function neutron__get_xs(this,E,T) bind(c, &
                                name="Neutron__xs_c") result(xs)
                        use, intrinsic :: iso_c_binding, only : c_ptr, c_double
                        implicit none
                        type(c_ptr), intent(inout) :: this
                        real(kind=c_double), intent(in) :: E
                        real(kind=c_double), intent(in) :: T
                        real(kind=c_double), pointer :: xs
                 
                end function 


contains
                subroutine neutron_create(self,filename)
                        class(neutron_t), intent(inout) :: self
                        character(len=*), intent(in) :: filename
                        call self%delete()
                        call neutron__new(self%this,filename//c_null_char)
                end subroutine neutron_create

                function neutron_bp(self) result(bpval)
                        class(neutron_t), intent(inout) :: self
                        integer(c_int), intent(out) :: bpval

                        call neutron__get_bp(self%this)
                
                end subroutine neutron_bp 

                function neutron_order(self) result(ord)
                        class(neutron_t), intent(inout) :: self
                        integer(c_int), intent(out) :: ord

                        call neutron__get_order(self%this)
                end function neutron_order

                function neutron_fis(self) result(fis)
                        class(neutron_t), intent(inout) :: self 
                        integer(c_int), intent(out) :: fis

                        call neutron__get_fis(self%this)

                end function neutron_fis
        
                function neutron_Emax result(emax)
                        class(neutron_t), intent(inout) :: self
                        real(c_double), intent(out) :: emax 

                        call neutron__get_E_max(self%this)
                end function neutron_Emax

                function neutron_Emin result(emin)
                        class(neutron_t), intent(inout) :: self 
                        real(c_double), intent(out) :: emin

                        call neutron__get_E_min(self%this)

                end function neutron_Emin

                function neutron_spacing result(spacing)
                       class(neutron_t), intent(inout) :: self 
                       real(c_double), intent(out) :: spacing

                      call neutron__get_spacing(self%this) 











































