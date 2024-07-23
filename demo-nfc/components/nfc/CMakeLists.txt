set(compile_flags 
    "-DxPortIsInsideInterrupt=xPortInIsrContext"
    "-isystem"
    "${IDF_PATH}/components/freertos/include/freertos"
    # "${IDF_PATH}/components/freertos/port/xtensa/include/freertos"

    "-DNXPBUILD__PHHAL_HW_PN5180"
    "-DPHDRIVER_PN5180_CUSTOM_DEV_BOARD"

    "-DPH_OSAL_FREERTOS"

    "-DNXPBUILD_CUSTOMER_HEADER_INCLUDED"

    "-Wno-error=implicit-function-declaration"
    "-Wno-error=memset-elt-size"
)

set(nfc_lib 
    "custom/dal.c"
    "custom/phDriver.c"
    "custom/phDriver_timer.c"
    "custom/phDriver_spi.c"
    "custom/phDriver_gpio.c"

    "src/phApp_PN5180_Init.c"
    "src/phApp_Init.c"
    "src/phNfc_Example.c"

    "nxp/RTOS/phOsal/src/Freertos/phOsal_Freertos.c"

    "nxp/NxpNfcRdLib/comps/phNfcLib/src/phNfcLib_Initialization.c"

    "nxp/NxpNfcRdLib/comps/phhalHw/src/phhalHw.c"
    "nxp/NxpNfcRdLib/comps/phhalHw/src/Pn5180/phhalHw_Pn5180.c"

    "nxp/NxpNfcRdLib/comps/phhalHw/src/Pn5180/phhalHw_Pn5180.c"
    "nxp/NxpNfcRdLib/comps/phhalHw/src/Pn5180/phhalHw_Pn5180_Instr.c"
    "nxp/NxpNfcRdLib/comps/phhalHw/src/Pn5180/phhalHw_Pn5180_Wait.c"
    "nxp/NxpNfcRdLib/comps/phhalHw/src/Pn5180/phhalHw_Pn5180_Int.c"

    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw.c"
    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw_Int_A.c"
    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw_Int_I18000p3m3.c"
    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw_Int.c"
    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw_Int_B.c"
    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw_Int_F.c"
    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw_Int_ECP.c"
    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw_Int_V.c"
    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/phacDiscLoop.c"

    "nxp/NxpNfcRdLib/comps/phalTop/src/Sw/phalTop_Sw_Int_T2T.c"
    "nxp/NxpNfcRdLib/comps/phalTop/src/Sw/phalTop_Sw_Int_T3T.c"
    "nxp/NxpNfcRdLib/comps/phalTop/src/Sw/phalTop_Sw.c"
    "nxp/NxpNfcRdLib/comps/phalTop/src/Sw/phalTop_Sw_Int_MfcTop.c"
    "nxp/NxpNfcRdLib/comps/phalTop/src/Sw/phalTop_Sw_Int_T4T.c"
    "nxp/NxpNfcRdLib/comps/phalTop/src/Sw/phalTop_Sw_Int_T1T.c"
    "nxp/NxpNfcRdLib/comps/phalTop/src/Sw/phalTop_Sw_Int_T5T.c"
    "nxp/NxpNfcRdLib/comps/phalTop/src/phalTop.c"

    "nxp/NxpNfcRdLib/comps/phalT1T/src/Sw/phalT1T_Sw.c"
    "nxp/NxpNfcRdLib/comps/phalT1T/src/phalT1T.c"

    #tools
    "nxp/NxpNfcRdLib/comps/phTools/src/phTools.c"
    "nxp/NxpNfcRdLib/comps/phTools/src/phTools_Q.c"

    #supported protocol
    "nxp/NxpNfcRdLib/comps/phpalI14443p3a/src/Sw/phpalI14443p3a_Sw.c"
    "nxp/NxpNfcRdLib/comps/phpalI14443p3a/src/phpalI14443p3a.c"
)

set(nfc_lib_inc
    "custom/include"

    "src/inc"

    "nxp/NxpNfcRdLib/types"
    "nxp/NxpNfcRdLib/intfs"
    "nxp/NxpNfcRdLib/comps"
    
    "nxp/RTOS/phOsal/inc"
    "nxp/RTOS/phOsal/src/Freertos"

    "nxp/Platform/DAL/inc"

    "${IDF_PATH}/components/freertos/port/xtensa/include/freertos"
    "${IDF_PATH}/components/freertos/include/freertos"
)


set(main_lib "nfc.c")
set(main_lib_inc "include")

set(src "")
set(inc "")

list(APPEND src 
    ${main_lib}
    ${nfc_lib}
    )

list(APPEND inc 
    ${main_lib_inc}
    ${nfc_lib_inc}
    )

idf_component_register(
        SRCS ${src}
        INCLUDE_DIRS ${inc}
        )

target_compile_options(${COMPONENT_LIB} PRIVATE ${compile_flags})