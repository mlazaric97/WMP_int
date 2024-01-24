module neutron_mod
        use, intrinsic :: iso_c_binding
        implicit none

        type,public :: neutron_t
                type(c_ptr) :: this = c_null_ptr
        contains
                procedure, public :: create => neutron_create 
                procedure, public :: broadpoly => neutron_bp
                procedure, public :: order => neutron_order
                procedure, public :: fissionable => neutron_fis
                procedure, public :: E_min => neutron_Emin
                procedure, public :: E_max => neutron_Emax
                procedure, public :: spacing => neutron_spacing
                procedure, public :: getcf => neutron_cf
                procedure, public :: window => neutron_wind
                procedure, public :: xs => neutron_xs  
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
                        use, intrinsic :: iso_c_binding, only : c_ptr, c_int, c_double_complex 
                        implicit none
                        type(c_ptr), intent(inout) :: this
                        integer(c_int), intent(in) :: wid
                        integer(c_int), intent(in) :: rxn
                        complex(kind=c_double_complex) :: datum
                end function 

                function neutron__get_windows(this,wid) bind(c, &
                                name="Neutron__grab_windows_c") result(pti) !Pointer To Int
                        use, intrinsic :: iso_c_binding, only : c_ptr, c_int
                        implicit none
                        type(c_ptr), intent(inout) :: this       
                        integer(c_int), intent(in) :: wid
                        integer(c_int), pointer :: pti
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

                subroutine neutron__delete(this) bind(c, &
                                name="Neutron__delete_c")
                        use, intrinsic :: iso_c_binding, only : c_ptr
                        implicit none 
                        type(c_ptr), intent(inout) :: this
                end subroutine 
        end interface
        contains
               
                subroutine neutron_create(self,filename)
                        class(neutron_t), intent(inout) :: self
                        character(len=*), intent(in) :: filename
                        call self%delete()
                        call neutron__new(self%this,filename//c_null_char)
                end subroutine neutron_create

                function neutron_bp(self) result(bpval)
                        class(neutron_t), intent(inout) :: self
                        integer(c_int) :: bpval

                        bpval = neutron__get_bp(self%this)
                
                end function neutron_bp 

                function neutron_order(self) result(ord)
                        class(neutron_t), intent(inout) :: self
                        integer(c_int) :: ord

                        ord = neutron__get_order(self%this)
                end function neutron_order

                function neutron_fis(self) result(fis)
                        class(neutron_t), intent(inout) :: self 
                        integer(c_int) :: fis

                        fis = neutron__get_fis(self%this)

                end function neutron_fis
        
                function neutron_Emax(self) result(emax)
                        class(neutron_t), intent(inout) :: self
                        real(c_double) :: emax 

                        emax = neutron__get_Emax(self%this)
                end function neutron_Emax

                function neutron_Emin(self) result(emin)
                        class(neutron_t), intent(inout) :: self 
                        real(c_double) :: emin

                        emin = neutron__get_Emin(self%this)

                end function neutron_Emin

                function neutron_spacing(self) result(spacing)
                       class(neutron_t), intent(inout) :: self 
                       real(c_double) :: spacing

                      spacing = neutron__get_spacing(self%this) 

                end function neutron_spacing 

                function neutron_cf(self,wid,j,rxn) result(a_j) ! j is index for the order of the curvefit 
                        class(neutron_t), intent(inout) :: self
                        integer(c_int), intent(in) :: wid
                        integer(c_int), intent(in) :: j
                        integer(c_int), intent(in) :: rxn 
                        real(c_double) :: a_j 

                        a_j = neutron__get_cf(self%this, wid, j, rxn)
                end function neutron_cf

                function neutron_xs(self,E,T) result(xs)
                        class(neutron_t), intent(inout) :: self 
                        real(c_double), intent(in) :: E
                        real(c_double), intent(in) :: T
                        real(c_double), pointer :: xs 

                        xs = neutron__get_xs(self%this, E,T)
                end function neutron_xs

                function neutron_wind(self,w_id) result(ind)
                        class(neutron_t), intent(inout) :: self
                        integer(c_int), intent(in) :: w_id
                        integer(c_int), pointer :: ind

                        ind = neutron__get_windows(self%this,w_id)
                end function neutron_wind

                subroutine neutron_delete(self)
                        class(neutron_t), intent(inout) :: self

                        if (c_associated(self%this)) then 
                                call neutron__delete(self%this)
                                self%this = C_NULL_PTR
                        end if 

                end subroutine neutron_delete
                        
end module neutron_mod


