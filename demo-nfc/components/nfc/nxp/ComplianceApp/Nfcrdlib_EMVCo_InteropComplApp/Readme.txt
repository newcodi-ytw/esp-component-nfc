	   __________________________________________________

			 NXPNFCRDLIB EXAMPLE :
		     NFCRDLIB_EMVCO_INTEROPCOMPLAPP
			      (V07.10.00)
	   __________________________________________________


Table of Contents
_________________

1 Document Purpose
2 Description of the Nfcrdlib_EMVCo_InteropComplApp
3 Restrictions on Nfcrdlib_EMVCo_InteropComplApp
4 Configurations of Nfcrdlib_EMVCo_InteropComplApp
5 Package Contents
6 Mandatory materials (not included)
7 Hardware Configuration
8 Software Configuration
9 Steps to build Nfcrdlib_EMVCo_InteropComplApp for LPC1769 with PN5190 using MCUXpresso
10 Steps to build Nfcrdlib_EMVCo_InteropComplApp for LPC1769 with PN5180 using MCUXpresso
11 Steps to build Nfcrdlib_EMVCo_InteropComplApp for LPC1769 with RC663 using MCUXpresso
12 Steps to build Nfcrdlib_EMVCo_InteropComplApp for PN7462AU using MCUXpresso
13 Steps to build Nfcrdlib_EMVCo_InteropComplApp for FRDM-K82F using MCUXpresso
14 Selection of Board / OSAL / Frontend (MCUXpresso)
15 Steps to follow for PI / Linux / CMake
16 Running Nfcrdlib_EMVCo_InteropComplApp
17 List of supported NFC Reader Boards/ICs
18 Reference Documents





1 Document Purpose
==================

  This document describes the steps to be followed to execute
  Nfcrdlib_EMVCo_InteropComplApp example as well as it's known problems
  and restrictions.


2 Description of the Nfcrdlib_EMVCo_InteropComplApp
===================================================

  - Nfcrdlib_EMVCo_InteropComplApp can be run with CLEV663B, PN5180,
    PN5190 and PN7462AU.  (See Section-17 for supported versions)
  This example is a Interoperability LoopBack Application which is used
  to perform EMVCo IOP(L1) with add-on(TTA Bulletin No.195) compliance
  validation. The CPU frequency configured for the Host controller
  platforms should be sufficient enough to meet EMVCo timing
  requirements.


3 Restrictions on Nfcrdlib_EMVCo_InteropComplApp
================================================

  - Nfcrdlib_EMVCo_InteropComplApp is restricted to be run on NFC Reader
    Boards/ICs mentioned in Section-17.
  - The APIs are intended for NXP contact-less reader ICs only, as such
    the APIs are not to be ported to any technology from any other
    vendor.
  - NXP will not support porting to any other vendor platform.
  - This software project requires NxpNfcRdLib v07.10.00 or later.
  - **IMPORTANT** The default used MCU Type is K82. When executing
    example on LPC1769 or PN7462AU, the MCU Type has to be changed in
    the MCUXpresso IDE.
  - For switching to MCU Type to PN7462AU, see Section-12
  - For switching to MCU Type to LPC1769, see Section-9, Section-10 and
    Section-11.


4 Configurations of Nfcrdlib_EMVCo_InteropComplApp
==================================================

  - This example can be used for running EMVCo IOP(L1) with add-on(TTA
    Bulletin No.195) compliance validation.
  - This application is NOT verified with Test tool, since the update
    required for Interop testing is not yet available.


5 Package Contents
==================

  - Readme.txt
    + This readme file
  - Nfcrdlib_EMVCo_InteropComplApp.c
    + Main example file.
  - intfs/Nfcrdlib_EMVCo_InteropComplApp.h
    + Interfaces/Defines specific to the example
  - intfs/Nfcrdlib_EMVCo_InteropComplApp_status.h
    + Interfaces/Defines specific to this example to enable and control
      LED's as well as GPIO's.
  - src/Nfcrdlib_EMVCo_InteropComplApp_status.c
    + Function definitions specific to the example to enable and control
      LED's as well as GPIO's.
  - intfs/ph_NxpBuild_App.h
    + Reader library build configuration file
  - src/phApp_Helper.c
  - src/phApp_PN5180_Init.c
  - src/phApp_PN5190_Init.c
  - src/phApp_PN7462AU_Init.c
  - src/phApp_RC663_Init.c
  - src/phApp_Init.c and intfs/phApp_Init.h
    + Common utility functions (common across all examples)
  - mcux/.cproject and mcux/.project
    + MCUXpresso project configuration file
  - cr_startup_lpc175x_6x.c
    + Startup required to compile example for LPC1769 Micro Controller.


6 Mandatory materials (not included)
====================================

  - MCUXpresso IDE. It can be downloaded from
    [http://www.nxp.com/products/:MCUXpresso-IDE]
  - SDK for Freedom K82 Board
    ([http://www.nxp.com/products/:FRDM-K82F]).  The pre-build SDK can
    be downloaded from
    [https://mcuxpresso.nxp.com/en/license?hash=9897a8c19a6bc569c3fade7141f0f405&hash_download=true&to_vault=true]
    See MCUXpresso User Manual for steps needed to install an SDK.
  - Plugin to extend MCUXpresso to support PN7462AU. (Required for
    MCUXpresso versions before MCUXpressoIDE_10.0.2)

  - LPCXpresso LPC1769 / Freedom K82 development boards For NFC ICs
    listed in Section-17 (Note: PN7462AU (Section-17 Bullet-3) does not
    need any other other microcontroller).


7 Hardware Configuration
========================

  Before starting this application, HW Changes may be required for the
  used board.  Refer to the following User Manuals / Application notes
  before starting with this example.

  - AN11211: Quick Start Up Guide RC663 Blueboard
  - AN11744: PN5180 Evaluation board quick start guide
  - AN11802: NFC Reader Library for Linux Installation Guidelines
  - AN12550: PNEV5190B Evaluation board quick start guide


8 Software Configuration
========================

  - The Software can be compiled for Cortex M3 LPC1769 micro-controller,
    Cortex M4 Freedom K82 and Cortex M0 based PN7462AU from NXP.
  - Since this example can be configured to run on various MCU ICs and
    various NFC ICs, appropriate changes are required as mentioned in
    Section-9, Section-10, Section-11, Section-12 and Section-13.


9 Steps to build Nfcrdlib_EMVCo_InteropComplApp for LPC1769 with PN5190 using MCUXpresso
========================================================================================

  See "PNEV5190B Evaluation board quick start guide" in AN12550 (See
  Section-18, Bullet-6 below)


10 Steps to build Nfcrdlib_EMVCo_InteropComplApp for LPC1769 with PN5180 using MCUXpresso
=========================================================================================

  See "Importing provided SW example projects" in AN11908 (See
  Section-18, Bullet-2 below)


11 Steps to build Nfcrdlib_EMVCo_InteropComplApp for LPC1769 with RC663 using MCUXpresso
========================================================================================

  See "Importing provided SW example projects" in AN11022 (See
  Section-18, Bullet-3 below)


12 Steps to build Nfcrdlib_EMVCo_InteropComplApp for PN7462AU using MCUXpresso
==============================================================================

  - For MCUXpresso versions before MCUXpressoIDE_10.0.2, See "Adding
    PN7462AU Plugin" in UM10883. (See Section-18, Bullet-4 below)
  - See "Importing provided SW example projects" in UM10883.


13 Steps to build Nfcrdlib_EMVCo_InteropComplApp for FRDM-K82F using MCUXpresso
===============================================================================

  See "Import projects to the MCUXpresso" in AN11908. (See Section-18,
  Bullet-2 below)


14 Selection of Board / OSAL / Frontend (MCUXpresso)
====================================================

  For MCUXpresso, the selection of Board / OSAL / Frontend has to be
  done via -D (Preprocessor defines).

  1) To select the board go to "Project Properties" --> "C/C++ Build"
     --> "Settings" --> "Preprocessor" --> "Defined symbols (-D)", and
     define the relevant PHDRIVER_<BoardNFCCombination>_BOARD macro.

     e.g. For using LPC1769 with Pn5180 use
     PHDRIVER_LPC1769PN5180_BOARD.  For list of supported boards refer
     to Platform\DAL\cfg\BoardSelection.h.

  2) To select the osal/os type, go to "Project Properties" --> "C/C++
     Build" --> "Settings" --> "Preprocessor" --> "Defined symbols
     (-D)".

     e.g. For using FreeRTOS use PH_OSAL_FREERTOS. For other options
     refer to RTOS\phOsal\inc\phOsal_Config.h.


15 Steps to follow for PI / Linux / CMake
=========================================

  The steps are described in AN11802. (See Section-18, Bullet-5 below)


16 Running Nfcrdlib_EMVCo_InteropComplApp
=========================================

  The running application can now be used for EMVCo IOP with add-on(TTA
  Bulletin No.195) Test suite validation.  Test success indicated by
  Green LED and GPIO and fail indicated by Red LED and GPIO (except in
  the case of Raspberry Pi).
  1) CLEV6630B 2.0 or PNEV5180B v2.0 Customer Evaluation Board LED's and
     GPIO's indication:
     - Red LED - LD201
     - Green LED - LD203
     - GPIO(indicates Success) - TXD
     - GPIO(indicates Fail) - RXD Success: Green LED On, GPIO High 500
       milli sec delay GPIO Low and Green LED Off Fail : Red LED On,
       GPIO High, 500 milli sec delay, GPIO Low and RED LED Off
  2) PNEV7462C Board LED's and GPIO's indication:
     - Red LED - LED12
     - Green LED - LED10
     - GPIO(indicates Success) - GPIO1
     - GPIO(indicates Fail) - GPIO2 Success: Green LED On, GPIO High,
       500 milli sec delay, GPIO Low and Green LED Off Fail : Red LED
       On, GPIO High, 500 milli sec delay, GPIO Low and Red LED Off
  3) FRDM-K82F Board LED's and GPIO's indication:
     - Red LED - D3 RGB
     - Green LED - D3 RGB
     - GPIO(indicates Success) - Con J4 - Pin6
     - GPIO(indicates Fail) - Con J4 - Pin8 Success: Green LED On, GPIO
       High, 500 milli sec delay, GPIO Low and Green LED Off Fail : Red
       LED On, GPIO High, 500 milli sec delay, GPIO Low and Red LED Off
  4) PNEV5190B V1.0 Board LED's and GPIO's indication:
     - Red LED - LEDRR
     - Green LED - LEDGR
     - GPIO(indicates Success) - TP8
     - GPIO(indicates Fail) - TP6 Success: Green LED On, GPIO High, 500
       milli sec delay, GPIO Low and Green LED Off Fail : Red LED On,
       GPIO High, 500 milli sec delay, GPIO Low and Red LED Off
  5) Raspberry Pi: Test success and fail indicated by Green LED
     - Green LED - ACT Success: Green LED On, 2 sec delay and Green LED
       Off Fail : Green LED Toggles every 250 milli sec for 2 sec and
       Green LED Off
  6) X-PNEV76FAMA (Rev D) Board LED and GPIO's indications:
     - Red LED(D6) - GPIO3 and Green LED(D7)- GPIO2
     Note:Jumper at J63 should not be connected
     - GPIO(indicates Success) - GPIO1
     - GPIO(indicates Fail) - GPIO0 Success: Green LED On, GPIO High,
       500 milli
     sec delay, GPIO Low and Green LED Off Fail : Red LED On, GPIO High,
     500 milli sec delay, GPIO Low and Red LED Off Note:Jumper at J64
     should be connected Note:J60 pin 2 should be probed for Success
     indication and J60 pin 4 should be probed for Fail indication
  7) PNEV7642A (Rev A) Board LED and GPIO's indications:
     - Red LED(D6) - GPIO3 and Green LED(D7)- GPIO2
     Note:Jumper at J63 and J66 should not be connected
     - GPIO(indicates Success) - GPIO1
     - GPIO(indicates Fail) - GPIO0 Success: Green LED On, GPIO High,
       500 milli
     sec delay, GPIO Low and Green LED Off Fail : Red LED On, GPIO High,
     500 milli sec delay, GPIO Low and Red LED Off Note:Jumper at J64
     and J67 should be connected Note:J60 pin 2 should be probed for
     Success indication and J60 pin 4 should be probed for Fail
     indication


17 List of supported NFC Reader Boards/ICs
==========================================

  1) CLEV6630B 2.0 Customer Evaluation Board
  2) PNEV5180B v2.0 Customer Evaluation Board
  3) PN7462AU v2.1 Customer Evaluation Board
  4) PNEV5190B v1.0 Customer Evaluation Board


18 Reference Documents
======================

  1) UM10954 : PN5180 SW Quick start guide
     [http://www.nxp.com/docs/en/user-guide/UM10954.pdf]

  2) AN11908 : NFC Reader Library for FRDM-K82F Board Installation
     guidelines
     [http://www.nxp.com/docs/en/application-note/AN11908.pdf]

  3) AN11022 : CLRC663 Evaluation board quick start guide
     [http://www.nxp.com/docs/en/application-note/AN11022.pdf]

  4) UM10883 : PN7462AU Quick Start Guide - Development Kit
     [http://www.nxp.com/docs/en/user-guide/UM10883.pdf]

  5) AN11802 : NFC Reader Library for Linux Installation Guidelines
     [http://www.nxp.com/docs/en/application-note/AN11802.pdf]

  6) AN12550 : PNEV5190B Evaluation board quick start guide


  ----------------------------------------------------------------------

  For updates of this example, see
  [http://www.nxp.com/pages/:NFC-READER-LIBRARY]
