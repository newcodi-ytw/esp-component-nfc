set(main_lib "nfc.c")
set(main_lib_inc "include/.")

set(src "")
set(inc "")

list(APPEND src 
    ${main_lib}
    )

list(APPEND inc 
    ${main_lib_inc}
    )

idf_component_register(
        SRCS ${src}
        INCLUDE_DIRS ${inc}
        )
