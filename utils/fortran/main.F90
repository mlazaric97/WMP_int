program main
        use WMP_mod, only: helloworld, wmp_read
        use neutron_mod, only: neutron_t

        character (80) :: s

        s = "abcdefg"


        call helloworld()
        call wmp_read(s)

end program main

