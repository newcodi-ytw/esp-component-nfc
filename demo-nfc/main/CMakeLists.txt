set(console_lib 
    "console/console_helper.cpp"
    "console/console_cmd.cpp"
    )

set(console_inc "console/include/.")

set(main_lib "demo-nfc.c")
set(main_lib_inc ".")

set(src "")
set(inc "")

list(APPEND src 
    ${main_lib}
    ${console_lib} 
    )

list(APPEND inc 
    ${main_lib_inc}
    ${console_inc} 
    )

idf_component_register(
        SRCS ${src}
        INCLUDE_DIRS ${inc}
        )