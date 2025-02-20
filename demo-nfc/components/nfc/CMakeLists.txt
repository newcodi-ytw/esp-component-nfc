set(compile_flags 
    "-DxPortIsInsideInterrupt=xPortInIsrContext"
    "-isystem"
    "${IDF_PATH}/components/freertos/include/freertos"

    "-DNXPBUILD__PHHAL_HW_PN5180"
    "-DPHDRIVER_PN5180_CUSTOM_DEV_BOARD"

    "-DDEBUG"
    "-DPH_OSAL_FREERTOS"

    "-DNXPBUILD_CUSTOMER_HEADER_INCLUDED"

    "-Wno-error=implicit-function-declaration"
    "-Wno-error=memset-elt-size"
    "-Wno-error=maybe-uninitialized"
    "-Wno-error=unused-but-set-parameter"
)

set(nfc_lib 
    "custom/dal.c"
    "custom/phDriver.c"
    "custom/phDriver_timer.c"
    "custom/phDriver_spi.c"
    "custom/phDriver_gpio.c"
    
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
)

set(nfc_lib_inc
    "custom/include"

    "nxp/NxpNfcRdLib/types"
    "nxp/NxpNfcRdLib/intfs"
    "nxp/NxpNfcRdLib/comps"
    
    "nxp/RTOS/phOsal/inc"
    "nxp/RTOS/phOsal/src/Freertos"

    "nxp/Platform/DAL/inc"

    "${IDF_PATH}/components/freertos/port/xtensa/include/freertos"
    "${IDF_PATH}/components/freertos/include/freertos"
)


#example source
set(EXAMPLE_SIMPLE_ISO FALSE)
set(EXAMPLE_DISC_LOOP FALSE)
set(EXAMPLE_NFCFORUM TRUE)
set(EXAMPLE_MIFARE_C FALSE)

#supported protocol - XXXX disable
set(NXPBUILD__PHHAL_HW_PN7640 FALSE)
set(NXPBUILD__PHHAL_HW_PN76XX FALSE)

#supported protocol - VVVV enable
set(NXPBUILD_CRYPTO_KEY TRUE)
set(NXPBUILD__PHPAL_I14443P3A_SW TRUE)
set(NXPBUILD__PHPAL_I14443P3B_SW TRUE)
set(NXPBUILD__PHPAL_I14443P4A_SW TRUE)
set(NXPBUILD__PHPAL_I14443P4_SW TRUE)
set(NXPBUILD__PHPAL_MIFARE_SW TRUE)
set(NXPBUILD__PHPAL_FELICA_SW TRUE)
set(NXPBUILD__PHPAL_SLI15693_SW TRUE)

if(EXAMPLE_SIMPLE_ISO)
    message(WARNING "**** EXAMPLE_SIMPLE_ISO ****")
    list(APPEND compile_flags 
        "-DEXAMPLE_SIMPLE_ISO"
    )
    list(APPEND nfc_lib 
        "examples/simplifiedAPI/Nfcrdlib_SimplifiedApi_ISO_MFC.c"
        "examples/simplifiedAPI/Nfcrdlib_SimplifiedApi_ISO_18000.c"
        "examples/simplifiedAPI/Nfcrdlib_SimplifiedApi_ISO_Layer4TypeA.c"
        "examples/simplifiedAPI/Nfcrdlib_SimplifiedApi_ISO_TypeB.c"
        "examples/simplifiedAPI/Nfcrdlib_SimplifiedApi_ISO_15693.c"
        "examples/simplifiedAPI/Nfcrdlib_SimplifiedApi_ISO_MFUL.c"
        "examples/simplifiedAPI/phApp_PN5180_Init.c"
        "examples/simplifiedAPI/phApp_Init.c"
        "examples/simplifiedAPI/phApp_Helper.c"

        "examples/simplifiedAPI/Nfcrdlib_SimplifiedApi_ISO.c"

        #nxp lib
        "nxp/NxpNfcRdLib/comps/phNfcLib/src/phNfcLib.c"
        "nxp/NxpNfcRdLib/comps/phNfcLib/src/phNfcLib_Initialization.c"
        "nxp/NxpNfcRdLib/comps/phNfcLib/src/phNfcLib_18000p3m3.c"
        "nxp/NxpNfcRdLib/comps/phNfcLib/src/phNfcLib_MFC.c"
        "nxp/NxpNfcRdLib/comps/phNfcLib/src/phNfcLib_15693.c"
        "nxp/NxpNfcRdLib/comps/phNfcLib/src/phNfcLib_MFDF.c"
        "nxp/NxpNfcRdLib/comps/phNfcLib/src/phNfcLib_Utility.c"
        "nxp/NxpNfcRdLib/comps/phNfcLib/src/phNfcLib_MFUL.c"
    )    
    list(APPEND nfc_lib_inc
        "examples/simplifiedAPI/inc"
    )
endif()

if(EXAMPLE_DISC_LOOP)
    set(EXAMPLE_TEST_LOOP FALSE)

    message(WARNING "**** EXAMPLE_DISC_LOOP ****")
    list(APPEND compile_flags 
        "-DEXAMPLE_DISC_LOOP"
    )
    list(APPEND nfc_lib 
        "examples/discLoop/NfcrdlibEx1_EmvcoProfile.c"
        "examples/discLoop/phApp_PN5180_Init.c"
        "examples/discLoop/phApp_Init.c"
        "examples/discLoop/phApp_Helper.c"
        
        "examples/discLoop/NfcrdlibEx1_DiscoveryLoop.c"
    )
    if(EXAMPLE_TEST_LOOP)
        list(APPEND compile_flags 
            "-DEXAMPLE_TEST_LOOP"
        )
        list(APPEND nfc_lib
            "examples/discLoop/phNfc_Example.c"
        )
    endif()
    list(APPEND nfc_lib_inc
        "examples/discLoop/inc"
    )
endif()

if(EXAMPLE_MIFARE_C)
    message(WARNING "**** EXAMPLE_MIFARE_C ****")
    list(APPEND compile_flags 
        "-DEXAMPLE_MIFARE_C"
    )
    list(APPEND nfc_lib 
        "examples/mifareClassic/inc"
        "examples/mifareClassic/phApp_PN5180_Init.c"
        "examples/mifareClassic/phApp_Init.c"
        "examples/mifareClassic/phApp_Helper.c"
        "examples/mifareClassic/NfcrdlibEx4_MIFAREClassic.c"

        #nxp lib
    )    
    list(APPEND nfc_lib_inc
        "examples/mifareClassic/inc"
    )
endif()

if(EXAMPLE_NFCFORUM)
    message(WARNING "**** EXAMPLE_NFCFORUM ****")
    list(APPEND compile_flags 
        "-DEXAMPLE_NFCFORUM"
    )
    list(APPEND nfc_lib 
        "examples/nfcForum/inc"
        "examples/nfcForum/cards.c"
        "examples/nfcForum/phApp_PN5180_Init.c"
        "examples/nfcForum/phApp_Init.c"
        "examples/nfcForum/phApp_Helper.c"
        "examples/nfcForum/NfcrdlibEx3_NFCForum.c"

        #nxp lib
        "nxp/NxpNfcRdLib/comps/phpalI18092mT/src/Sw/phpalI18092mT_Sw.c"
        "nxp/NxpNfcRdLib/comps/phpalI18092mT/src/Sw/phpalI18092mT_Sw_Int.c"
        "nxp/NxpNfcRdLib/comps/phpalI18092mT/src/phpalI18092mT.c"

        "nxp/NxpNfcRdLib/comps/phlnLlcp/src/Sw/phlnLlcp_Sw_Int.c"
        "nxp/NxpNfcRdLib/comps/phlnLlcp/src/Sw/phlnLlcp_Sw_Transport_Connection.c"
        "nxp/NxpNfcRdLib/comps/phlnLlcp/src/Sw/phlnLlcp_Sw.c"
        "nxp/NxpNfcRdLib/comps/phlnLlcp/src/Sw/phlnLlcp_Sw_Transport_Connectionless.c"
        "nxp/NxpNfcRdLib/comps/phlnLlcp/src/Sw/phlnLlcp_Timers.c"
        "nxp/NxpNfcRdLib/comps/phlnLlcp/src/Sw/phlnLlcp_Sw_Mac.c"
        "nxp/NxpNfcRdLib/comps/phlnLlcp/src/Sw/phlnLlcp_Sw_Transport.c"
        "nxp/NxpNfcRdLib/comps/phlnLlcp/src/phlnLlcp.c"

        "nxp/NxpNfcRdLib/comps/phnpSnep/src/Sw/phnpSnep_Sw.c"
        "nxp/NxpNfcRdLib/comps/phnpSnep/src/phnpSnep.c"
    )    
    list(APPEND nfc_lib_inc
        "examples/nfcForum/inc"
    )
endif()

if(NXPBUILD_CRYPTO_KEY)
    message(WARNING "**** NXPBUILD_CRYPTO_KEY ****")
    list(APPEND compile_flags 
        # "-DNXPBUILD__PH_KEYSTORE_SW"
        # "-DNXPBUILD__PH_CRYPTOSYM_SW"
        # "-DNXPBUILD__PH_CRYPTORNG_SW"
    )
    list(APPEND nfc_lib 
        "nxp/NxpNfcRdLib/comps/phKeyStore/src/Sw/phKeyStore_Sw.c"
        "nxp/NxpNfcRdLib/comps/phKeyStore/src/phKeyStore.c"
        "nxp/NxpNfcRdLib/comps/phKeyStore/src/Rc663/phKeyStore_Rc663.c"

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
    )
endif()

if(NXPBUILD__PHPAL_I14443P3A_SW)
    message(WARNING "**** NXPBUILD__PHPAL_I14443P3A_SW ****")
    list(APPEND compile_flags 
        # "-DNXPBUILD__PHPAL_I14443P3A_SW"
    )
    list(APPEND nfc_lib 
        "nxp/NxpNfcRdLib/comps/phpalI14443p3a/src/Sw/phpalI14443p3a_Sw.c"
        "nxp/NxpNfcRdLib/comps/phpalI14443p3a/src/phpalI14443p3a.c"
    )
endif()

if(NXPBUILD__PHPAL_I14443P3B_SW)
    message(WARNING "**** NXPBUILD__PHPAL_I14443P3B_SW ****")
    list(APPEND compile_flags 
        # "-DNXPBUILD__PHPAL_I14443P3B_SW"
    )
    list(APPEND nfc_lib 
        "nxp/NxpNfcRdLib/comps/phpalI14443p3b/src/Sw/phpalI14443p3b_Sw.c"
        "nxp/NxpNfcRdLib/comps/phpalI14443p3b/src/phpalI14443p3b.c"
    )
endif()

if(NXPBUILD__PHPAL_I14443P4A_SW)
    message(WARNING "**** NXPBUILD__PHPAL_I14443P4A_SW ****")
    list(APPEND compile_flags 
        # "-DNXPBUILD__PHPAL_I14443P4A_SW"
    )
    list(APPEND nfc_lib 
        "nxp/NxpNfcRdLib/comps/phpalI14443p4a/src/Sw/phpalI14443p4a_Sw.c"
        "nxp/NxpNfcRdLib/comps/phpalI14443p4a/src/phpalI14443p4a.c"
    )
endif()

if(NXPBUILD__PHPAL_I14443P4_SW)
    message(WARNING "**** NXPBUILD__PHPAL_I14443P4_SW ****")
    list(APPEND compile_flags 
        # "-DNXPBUILD__PHPAL_I14443P4_SW"
    )
    list(APPEND nfc_lib 
        "nxp/NxpNfcRdLib/comps/phpalI14443p4/src/Sw/phpalI14443p4_Sw.c"
        "nxp/NxpNfcRdLib/comps/phpalI14443p4/src/phpalI14443p4.c"
    )
endif()

if(NXPBUILD__PHPAL_MIFARE_SW)
    message(WARNING "**** DNXPBUILD__PHPAL_MIFARE_SW ****")
    list(APPEND compile_flags 
        # "-DNXPBUILD__PHPAL_MIFARE_SW"
    )
    list(APPEND nfc_lib 
        "nxp/NxpNfcRdLib/comps/phpalMifare/src/Sw/phpalMifare_Sw.c"
        "nxp/NxpNfcRdLib/comps/phpalMifare/src/phpalMifare.c"
        "nxp/NxpNfcRdLib/comps/phalMfdf/src/Sw/phalMfdf_Sw_Int.c"
        "nxp/NxpNfcRdLib/comps/phalMfdf/src/Sw/phalMfdf_Sw.c"
        "nxp/NxpNfcRdLib/comps/phalMfdf/src/phalMfdf_Int.c"
        "nxp/NxpNfcRdLib/comps/phalMfdf/src/phalMfdf.c"
        "nxp/NxpNfcRdLib/comps/phalMful/src/Sw/phalMful_Sw.c"
        "nxp/NxpNfcRdLib/comps/phalMful/src/phalMful_Int.c"
        "nxp/NxpNfcRdLib/comps/phalMful/src/phalMful.c"
    )
endif()

if(NXPBUILD__PHPAL_FELICA_SW)
    message(WARNING "**** NXPBUILD__PHPAL_FELICA_SW ****")
    list(APPEND compile_flags 
        # "-DNXPBUILD__PHPAL_FELICA_SW"
    )
    list(APPEND nfc_lib 
        "nxp/NxpNfcRdLib/comps/phalFelica/src/Sw/phalFelica_Sw.c"
        "nxp/NxpNfcRdLib/comps/phalFelica/src/phalFelica.c"

        "nxp/NxpNfcRdLib/comps/phpalFelica/src/Sw/phpalFelica_Sw.c"
        "nxp/NxpNfcRdLib/comps/phpalFelica/src/phpalFelica.c"
    )
endif()

if(NXPBUILD__PHPAL_SLI15693_SW)
    message(WARNING "**** NXPBUILD__PHPAL_SLI15693_SW ****")
    list(APPEND compile_flags 
        # "-DNXPBUILD__PHPAL_SLI15693_SW"
    )
    list(APPEND nfc_lib 
        "nxp/NxpNfcRdLib/comps/phpalSli15693/src/Sw/phpalSli15693_Sw.c"
        "nxp/NxpNfcRdLib/comps/phpalSli15693/src/phpalSli15693.c"
        "nxp/NxpNfcRdLib/comps/phalICode/src/Sw/phalICode_Sw.c"
        "nxp/NxpNfcRdLib/comps/phalICode/src/phalICode.c"
        "nxp/NxpNfcRdLib/comps/phalICode/src/phalICode_Int.c"
    )
endif()

if(NOT NXPBUILD__PHHAL_HW_PN7640)
    message(WARNING "**** NOT NXPBUILD__PHHAL_HW_PN7640 ****")
    list(APPEND compile_flags 
        # "-DNXPBUILD__PHPAL_I18000P3M3_SW"
    )
    list(APPEND nfc_lib 
        "nxp/NxpNfcRdLib/comps/phalI18000p3m3/src/Sw/phalI18000p3m3_Sw_Int.c"
        "nxp/NxpNfcRdLib/comps/phalI18000p3m3/src/Sw/phalI18000p3m3_Sw.c"
        "nxp/NxpNfcRdLib/comps/phalI18000p3m3/src/phalI18000p3m3.c"

        "nxp/NxpNfcRdLib/comps/phpalI18000p3m3/src/Sw/phpalI18000p3m3_Sw.c"
        "nxp/NxpNfcRdLib/comps/phpalI18000p3m3/src/phpalI18000p3m3.c"
    )
endif()

if(NOT NXPBUILD__PHHAL_HW_PN76XX)
    message(WARNING "**** NOT NXPBUILD__PHHAL_HW_PN76XX ****")
    list(APPEND compile_flags 
        # "-DNXPBUILD__PHPAL_I18092MPI_SW"
    )
    list(APPEND nfc_lib 
        "nxp/NxpNfcRdLib/comps/phpalI18092mPI/src/Sw/phpalI18092mPI_Sw_Int.c"
        "nxp/NxpNfcRdLib/comps/phpalI18092mPI/src/Sw/phpalI18092mPI_Sw.c"
        "nxp/NxpNfcRdLib/comps/phpalI18092mPI/src/phpalI18092mPI.c"
    )
endif()


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