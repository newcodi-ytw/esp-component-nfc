/*----------------------------------------------------------------------------*/
/* Copyright 2017-2022 NXP                                                    */
/*                                                                            */
/* NXP Confidential. This software is owned or controlled by NXP and may only */
/* be used strictly in accordance with the applicable license terms.          */
/* By expressly accepting such terms or by downloading, installing,           */
/* activating and/or otherwise using the software, you are agreeing that you  */
/* have read, and that you agree to comply with and are bound by, such        */
/* license terms. If you do not agree to be bound by the applicable license   */
/* terms, then you may not retain, install, activate or otherwise use the     */
/* software.                                                                  */
/*----------------------------------------------------------------------------*/

/** \file
* Generic phDriver Component of Reader Library Framework.
* $Author$
* $Revision$
* $Date$
*
*/

#ifndef BOARD_PIPN5180_H
#define BOARD_PIPN5180_H

/*******************************************************************
 *  Enable ESP SPI Configure
 ******************************************************************/

#include "driver/gpio.h"

#define BOARD_DEVKITC

#ifdef BOARD_DEVKITC
#define PIN_MOSI  GPIO_NUM_4
#define PIN_SCLK  GPIO_NUM_16
#define PIN_MISO  GPIO_NUM_17

#define PIN_BUSY  GPIO_NUM_35
#define PIN_BUSY_REG  IO_MUX_GPIO35_REG
#define PIN_RESET GPIO_NUM_32
#define PIN_RESET_REG IO_MUX_GPIO32_REG
#define PIN_SSEL  GPIO_NUM_25
#define PIN_SSEL_REG  IO_MUX_GPIO25_REG

#define PIN_IRQ   GPIO_NUM_34
#define PIN_DWL   GPIO_NUM_33

#define PIN_AUX1        GPIO_NUM_NC
#define PIN_GPO1_GREEN  GPIO_NUM_23
#define PIN_GPO1_BLUE   GPIO_NUM_13
#define PIN_GPO1_RED    GPIO_NUM_22
#endif

#define PIN_MASK(n) ((uint64_t)1 << (n))

#define PINS_OUT (PIN_MASK(PIN_MOSI)  | \
                  PIN_MASK(PIN_SSEL)  | \
                  PIN_MASK(PIN_SCLK)  | \
                  PIN_MASK(PIN_RESET) | \
                  PIN_MASK(PIN_DWL))

#define PINS_OD (0)

#define PINS_IN (PIN_MASK(PIN_MISO) | \
                 PIN_MASK(PIN_IRQ)  | \
                 PIN_MASK(PIN_AUX1) | \
                 PIN_MASK(PIN_GPO1) | \
                 PIN_MASK(PIN_BUSY))

/*******************************************************************
 *  Enable User/Kernel space SPI.
 ******************************************************************/
/*#define PHDRIVER_LINUX_USER_SPI*/
// #define PHDRIVER_LINUX_KERNEL_SPI

/******************************************************************
 * Board Pin/Gpio configurations
 ******************************************************************/
#define PHDRIVER_PIN_RESET         PIN_RESET   /**< "/sys/class/gpio/gpio16/" */
#define PHDRIVER_PIN_IRQ           PIN_IRQ  /**< "/sys/class/gpio/gpio23/" */
#define PHDRIVER_PIN_BUSY          PIN_BUSY  /**< "/sys/class/gpio/gpio25/" */
#define PHDRIVER_PIN_DWL           PIN_DWL  /**< "/sys/class/gpio/gpio18/" */

/******************************************************************
 * PIN Pull-Up/Pull-Down configurations.
 ******************************************************************/
#define PHDRIVER_PIN_RESET_PULL_CFG    PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_IRQ_PULL_CFG      PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_BUSY_PULL_CFG     PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_DWL_PULL_CFG      PH_DRIVER_PULL_UP

/******************************************************************
 * IRQ & BUSY PIN TRIGGER settings
 ******************************************************************/
#define PIN_IRQ_TRIGGER_TYPE         PH_DRIVER_INTERRUPT_RISINGEDGE
#define PIN_BUSY_TRIGGER_TYPE        PH_DRIVER_INTERRUPT_FALLINGEDGE

/*****************************************************************
 * Front End Reset logic level settings
 ****************************************************************/
#define PH_DRIVER_SET_HIGH            1          /**< Logic High. */
#define PH_DRIVER_SET_LOW             0          /**< Logic Low. */
#define RESET_POWERDOWN_LEVEL       PH_DRIVER_SET_LOW
#define RESET_POWERUP_LEVEL         PH_DRIVER_SET_HIGH

/*****************************************************************
 * SPI Configuration
 ****************************************************************/
#ifdef PHDRIVER_LINUX_USER_SPI
#    define PHDRIVER_USER_SPI_BUS                    0   /**< "/dev/spidev0.0" */
#    define PHDRIVER_USER_SPI_CS                     0   /**< "/dev/spidev0.0" */
#    define PHDRIVER_USER_SPI_FREQ                   5000000 /**< 5 MHz. */
#    define PHDRIVER_USER_SPI_CFG_DIR                "/dev/spidev"
#    define PHDRIVER_USER_SPI_CFG_MODE               SPI_MODE_0
#    define PHDRIVER_USER_SPI_CFG_BITS_PER_WORD      8
#endif

#ifdef PHDRIVER_LINUX_KERNEL_SPI
#    define PHDRIVER_KERNEL_SPI_ID                   0x11U       /**< ID for Linux Kernel Spi BAL component */
#    define PHDRIVER_KERNEL_SPI_CFG_DIR              "/dev/bal"
#endif

/*****************************************************************
 * Dummy entries
 * No functionality. To suppress build error in HAL. No pin functionality in SPI Linux BAL.
 *****************************************************************/
#define PHDRIVER_PIN_SSEL                            PIN_SSEL
#define PHDRIVER_PIN_NSS_PULL_CFG                    PH_DRIVER_PULL_UP

/*****************************************************************
 * STATUS LED Configuration
 ****************************************************************/
#define PHDRIVER_LED_SUCCESS_DELAY      2

#define PHDRIVER_LED_FAILURE_DELAY_MS   250
#define PHDRIVER_LED_FAILURE_FLICKER    4

#endif /* BOARD_PIPN5180_H */
