module neutron_mod

        use, intrinsic :: iso_c_binding

        implicit none

        type, public :: neutron_t
                type(c_ptr) :: this = c_null_ptr
        contains
                procedure, public :: create => neutron_create
                procedure, public :: getfoo => neutron_getfoo
                procedure, public :: getbar => neutron_getbar
                procedure, public :: setfoo => neutron_setfoo
                procedure, public :: setbar => neutron_setbar
                procedure, public :: delete => neutron_delete
        end type neutron_t

  interface
    subroutine neutron__new(this, a, b)  bind(c, name="Neutron__new_c")
      use, intrinsic :: iso_c_binding, only : c_ptr, c_int
      implicit none
      type(c_ptr), intent(inout) :: this
      integer(c_int), intent(in), value :: a
      integer(c_int), intent(in), value :: b
    end subroutine
   
    function neutron__get_foo(this) bind(c,name="Neutron__get_foo_c") result(reval)
      use, intrinsic :: iso_c_binding, only : c_ptr, c_int
      implicit none
      type(c_ptr), intent(inout) :: this
      integer(c_int) :: reval
    end function

    function neutron__get_bar(this) bind(c,name="Neutron__get_bar_c") result(reval)
      use, intrinsic :: iso_c_binding, only : c_ptr, c_int
      implicit none
      type(c_ptr), intent(inout) :: this
      integer(c_int) :: reval
    end function

    subroutine neutron__set_foo(this,a) bind(c,name="Neutron__set_foo_c")
      use, intrinsic :: iso_c_binding, only : c_ptr, c_int
      implicit none
      type(c_ptr), intent(inout) :: this
      integer(c_int), intent(in) :: a
    end subroutine

    subroutine neutron__set_bar(this,b) bind(c,name="Neutron__set_bar_c")
      use, intrinsic :: iso_c_binding, only : c_ptr, c_int
      implicit none
      type(c_ptr), intent(inout) :: this
      integer(c_int), intent(in) :: b
    end subroutine

    subroutine neutron__delete(this) bind(c,name="Neutron__delete_c")
      use, intrinsic :: iso_c_binding, only : c_ptr
      implicit none
      type(c_ptr), intent(inout) :: this
    end subroutine
  end interface


contains


  subroutine neutron_create(self,a,b)
    class(neutron_t), intent(inout) :: self
    integer(c_int), intent(in) :: a
    integer(c_int), intent(in) :: b

    call self%delete()

    call neutron__new(self%this,a,b)

  end subroutine neutron_create

  function neutron_getfoo(self) result(reval)
    class(neutron_t), intent(inout) :: self
    integer(c_int) :: reval
    reval = neutron__get_foo(self%this)

   end function neutron_getfoo

    function neutron_getbar(self) result(reval) 
      class(neutron_t), intent(inout) :: self
      integer(c_int) :: reval

      reval = neutron__get_bar(self%this)

  end function neutron_getbar


  subroutine neutron_setfoo(self,a)
    class(neutron_t), intent(inout) :: self
    integer(c_int), intent(in) :: a

    call neutron__set_foo(self%this, a)

  end subroutine neutron_setfoo

  subroutine neutron_setbar(self,b)
    class(neutron_t), intent(inout) :: self
    integer(c_int), intent(in) :: b

    call neutron__set_bar(self%this,b)

  end subroutine neutron_setbar

  subroutine neutron_delete(self)
    class(neutron_t), intent(inout) :: self

    if (c_associated(self%this)) then 
      call neutron__delete(self%this)
      self%this = C_NULL_PTR
    end if
  end subroutine neutron_delete

end module neutron_mod
















