set(compile_flags 
    "-DxPortIsInsideInterrupt=xPortInIsrContext"
    "-isystem"
    "$(IDF_PATH)/components/freertos/include/freertos"
    
    "-DPHDRIVER_PN5180_CUSTOM_DEV_BOARD"
    "-DPH_OSAL_FREERTOS"
    # "-DDEBUG"
    
    # "-DNXPBUILD__PH_LOG"
    "-DNXPBUILD__PHHAL_HW_PN5180"
    "-DNXPBUILD_CUSTOMER_HEADER_INCLUDED"

    "-Wno-error=memset-elt-size"
    "-Wno-error=duplicate-decl-specifier"
    "-Wno-error=implicit-function-declaration"
)

set(nfc_lib 
    "nxp/Platform/DAL/src/ESP/dal.c"
    "nxp/Platform/DAL/src/ESP/phDriver.c"
    "nxp/Platform/DAL/src/ESP/phDriver_timer.c"
    "nxp/Platform/DAL/src/ESP/phDriver_spi.c"
    "nxp/Platform/DAL/src/ESP/phDriver_gpio.c"

    "nxp/NxpNfcRdLib/comps/phhalHw/src/Pn5180/phhalHw_Pn5180.c"
    "nxp/NxpNfcRdLib/comps/phhalHw/src/Pn5180/phhalHw_Pn5180_Instr.c"
    "nxp/NxpNfcRdLib/comps/phhalHw/src/Pn5180/phhalHw_Pn5180_Wait.c"
    "nxp/NxpNfcRdLib/comps/phhalHw/src/Pn5180/phhalHw_Pn5180_Int.c"

    "nxp/RTOS/phOsal/src/Freertos/phOsal_Freertos.c"

    "nxp/NxpNfcRdLib/comps/phTools/src/phTools_Q.c"
    "nxp/NxpNfcRdLib/comps/phTools/src/phTools.c"
    "nxp/NxpNfcRdLib/comps/phLog/src/phLog.c"

    "nxp/NxpNfcRdLib/comps/phalT1T/src/phalT1T.c"
    "nxp/NxpNfcRdLib/comps/phalT1T/src/Sw/phalT1T_Sw.c"
    "nxp/NxpNfcRdLib/comps/phhalHw/src/phhalHw.c"

    "nxp/NxpNfcRdLib/comps/phNfcLib/src/phNfcLib_Initialization.c"
    "nxp/NxpNfcRdLib/comps/phalMfdf/src/Sw/phalMfdf_Sw_Int.c"
    "nxp/NxpNfcRdLib/comps/phalMfdf/src/Sw/phalMfdf_Sw.c"
    "nxp/NxpNfcRdLib/comps/phalMfdf/src/phalMfdf_Int.c"
    "nxp/NxpNfcRdLib/comps/phalMfdf/src/phalMfdf.c"

    "nxp/NxpNfcRdLib/comps/phalMful/src/Sw/phalMful_Sw.c"
    "nxp/NxpNfcRdLib/comps/phalMful/src/phalMful_Int.c"
    "nxp/NxpNfcRdLib/comps/phalMful/src/phalMful.c"

    "nxp/NxpNfcRdLib/comps/phpalMifare/src/Sw/phpalMifare_Sw.c"
    "nxp/NxpNfcRdLib/comps/phpalMifare/src/phpalMifare.c"

    "nxp/NxpNfcRdLib/comps/phalTop/src/Sw/phalTop_Sw_Int_T2T.c"
    "nxp/NxpNfcRdLib/comps/phalTop/src/Sw/phalTop_Sw_Int_T3T.c"
    "nxp/NxpNfcRdLib/comps/phalTop/src/Sw/phalTop_Sw.c"
    "nxp/NxpNfcRdLib/comps/phalTop/src/Sw/phalTop_Sw_Int_MfcTop.c"
    "nxp/NxpNfcRdLib/comps/phalTop/src/Sw/phalTop_Sw_Int_T4T.c"
    "nxp/NxpNfcRdLib/comps/phalTop/src/Sw/phalTop_Sw_Int_T1T.c"
    "nxp/NxpNfcRdLib/comps/phalTop/src/Sw/phalTop_Sw_Int_T5T.c"
    "nxp/NxpNfcRdLib/comps/phalTop/src/phalTop.c"

    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/phacDiscLoop.c"
    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw.c"
    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw_Int_A.c"
    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw_Int_I18000p3m3.c"
    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw_Int.c"
    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw_Int_B.c"
    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw_Int_F.c"
    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw_Int_ECP.c"
    "nxp/NxpNfcRdLib/comps/phacDiscLoop/src/Sw/phacDiscLoop_Sw_Int_V.c"

    "nxp/NxpNfcRdLib/comps/phpalI14443p4a/src/Sw/phpalI14443p4a_Sw.c"
    "nxp/NxpNfcRdLib/comps/phpalI14443p4a/src/phpalI14443p4a.c"
    "nxp/NxpNfcRdLib/comps/phpalI14443p3a/src/Sw/phpalI14443p3a_Sw.c"
    "nxp/NxpNfcRdLib/comps/phpalI14443p3a/src/phpalI14443p3a.c"
    "nxp/NxpNfcRdLib/comps/phpalI14443p3b/src/Sw/phpalI14443p3b_Sw.c"
    "nxp/NxpNfcRdLib/comps/phpalI14443p3b/src/phpalI14443p3b.c"
    "nxp/NxpNfcRdLib/comps/phpalI14443p4/src/Sw/phpalI14443p4_Sw.c"
    "nxp/NxpNfcRdLib/comps/phpalI14443p4/src/phpalI14443p4.c"

    "nxp/NxpNfcRdLib/comps/phalI18000p3m3/src/Sw/phalI18000p3m3_Sw_Int.c"
    "nxp/NxpNfcRdLib/comps/phalI18000p3m3/src/Sw/phalI18000p3m3_Sw.c"
    "nxp/NxpNfcRdLib/comps/phalI18000p3m3/src/phalI18000p3m3.c"

    "nxp/NxpNfcRdLib/comps/phpalI18000p3m3/src/Sw/phpalI18000p3m3_Sw.c"
    "nxp/NxpNfcRdLib/comps/phpalI18000p3m3/src/phpalI18000p3m3.c"

    "nxp/NxpNfcRdLib/comps/phpalSli15693/src/Sw/phpalSli15693_Sw.c"
    "nxp/NxpNfcRdLib/comps/phpalSli15693/src/phpalSli15693.c"

    "nxp/NxpNfcRdLib/comps/phpalI18092mPI/src/Sw/phpalI18092mPI_Sw_Int.c"
    "nxp/NxpNfcRdLib/comps/phpalI18092mPI/src/Sw/phpalI18092mPI_Sw.c"
    "nxp/NxpNfcRdLib/comps/phpalI18092mPI/src/phpalI18092mPI.c"

    "nxp/NxpNfcRdLib/comps/phpalFelica/src/Sw/phpalFelica_Sw.c"
    "nxp/NxpNfcRdLib/comps/phpalFelica/src/phpalFelica.c"

    "nxp/NxpNfcRdLib/comps/phalFelica/src/Sw/phalFelica_Sw.c"
    "nxp/NxpNfcRdLib/comps/phalFelica/src/phalFelica.c"

    "nxp/NxpNfcRdLib/comps/phalICode/src/Sw/phalICode_Sw.c"
    "nxp/NxpNfcRdLib/comps/phalICode/src/phalICode.c"
    "nxp/NxpNfcRdLib/comps/phalICode/src/phalICode_Int.c"

    "nxp/NxpNfcRdLib/comps/phalMfc/src/Sw/phalMfc_Sw.c"
    "nxp/NxpNfcRdLib/comps/phalMfc/src/phalMfc_Int.c"
    "nxp/NxpNfcRdLib/comps/phalMfc/src/phalMfc.c"

    "nxp/NxpNfcRdLib/comps/phCryptoRng/src/Sw/phCryptoRng_Sw.c"
    "nxp/NxpNfcRdLib/comps/phCryptoRng/src/phCryptoRng.c"

    "nxp/NxpNfcRdLib/comps/phCryptoSym/src/Sw/phCryptoSym_Sw_Int.c"
    "nxp/NxpNfcRdLib/comps/phCryptoSym/src/Sw/phCryptoSym_Sw_Des.c"
    "nxp/NxpNfcRdLib/comps/phCryptoSym/src/Sw/phCryptoSym_Sw_Aes.c"
    "nxp/NxpNfcRdLib/comps/phCryptoSym/src/Sw/phCryptoSym_Lrp.c"
    "nxp/NxpNfcRdLib/comps/phCryptoSym/src/Sw/phCryptoSym_Sw.c"
    "nxp/NxpNfcRdLib/comps/phCryptoSym/src/phCryptoSym.c"

    "nxp/NxpNfcRdLib/comps/phKeyStore/src/Sw/phKeyStore_Sw.c"
    "nxp/NxpNfcRdLib/comps/phKeyStore/src/phKeyStore.c"
    "nxp/NxpNfcRdLib/comps/phKeyStore/src/Rc663/phKeyStore_Rc663.c"
    
    "nxp/Examples/NfcrdlibEx1_DiscoveryLoop/NfcrdlibEx1_DiscoveryLoop.c"
    "nxp/Examples/NfcrdlibEx1_DiscoveryLoop/src/NfcrdlibEx1_EmvcoProfile.c"
    "nxp/Examples/NfcrdlibEx1_DiscoveryLoop/src/phApp_PN5180_Init.c"
    "nxp/Examples/NfcrdlibEx1_DiscoveryLoop/src/phApp_Init.c"
    "nxp/Examples/NfcrdlibEx1_DiscoveryLoop/src/phApp_Helper.c"
)

set(nfc_lib_inc
    "nxp/Examples/NfcrdlibEx1_DiscoveryLoop/intfs"
    
    "nxp/Platform/DAL/src/ESP/include"

    "nxp/NxpNfcRdLib/comps"
    "nxp/NxpNfcRdLib/comps/phhalHw/src/Pn5180/"
    "nxp/NxpNfcRdLib/intfs"
    "nxp/NxpNfcRdLib/types"

    "nxp/Platform/DAL/cfg"
    "nxp/Platform/DAL/inc"

    "nxp/RTOS/phOsal/src/Freertos"
    "nxp/RTOS/phOsal/inc"
    "nxp/RTOS/FreeRTOS/include"
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