cmake_minimum_required(VERSION 3.27)
project(CSEII_Gr20 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(CSEII_Gr20
        Base.c
        Base.h
        DISP.c
        DISP.h
        Light.c
        Light.h
        LinkedList.c
        LinkedList.h
        main.c
        Temp.c
        Temp.h
        Time.c
        Time.h)
