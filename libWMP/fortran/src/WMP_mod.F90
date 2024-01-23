module WMP_mod

        use, intrinsic :: iso_c_binding

        implicit none

        public :: helloworld, wmp_read

        interface
                subroutine helloworld_f() bind(c,name="helloworld_c")
                end subroutine helloworld_f

                subroutine wmp_read_f(str) bind(c,name="wmp_read_c")
                        use, intrinsic :: iso_c_binding, only : c_char
                        implicit none
!                        character(kind=c_char,len=80), intent(in), value :: str
                        character(kind=c_char), dimension(*) :: str
                end subroutine wmp_read_f





        end interface


        contains
   
                subroutine helloworld()
                        call helloworld_f()
                end subroutine helloworld
                
                subroutine wmp_read(str)
                        use iso_c_binding, only: c_char, c_null_char
                        character(len=*), intent(in) :: str

                        call wmp_read_f(str//c_null_char)   
                end subroutine wmp_read



end module WMP_mod


