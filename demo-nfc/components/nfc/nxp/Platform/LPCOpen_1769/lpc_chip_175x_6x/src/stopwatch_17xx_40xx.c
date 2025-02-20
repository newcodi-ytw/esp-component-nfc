/*
 * @brief LPC17xx_40xx specific stopwatch implementation
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "chip.h"
#include "stopwatch.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Precompute these to optimize runtime */
static uint32_t ticksPerSecond;
static uint32_t ticksPerMs;
static uint32_t ticksPerUs;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Initialize stopwatch */
void StopWatch_Init(void)
{
	/* Use timer 1. Set prescaler to divide by 8, should give ticks at 3.75 MHz.
	   That gives a useable stopwatch measurement range of about 19 minutes
	   (if system clock is running at 120 MHz). */
	const uint32_t prescaleDivisor = 8;
	Chip_TIMER_Init(LPC_TIMER1);
	Chip_TIMER_PrescaleSet(LPC_TIMER1, prescaleDivisor - 1);
	Chip_TIMER_Enable(LPC_TIMER1);

	/* Pre-compute tick rate. Note that peripheral clock supplied to the
	   timer includes a fixed divide by 4. */
	ticksPerSecond = Chip_Clock_GetSystemClockRate() / prescaleDivisor / 4;
	ticksPerMs = ticksPerSecond / 1000;
	ticksPerUs = ticksPerSecond / 1000000;
}

/* Start a stopwatch */
uint32_t StopWatch_Start(void)
{
	/* Return the current timer count. */
	return Chip_TIMER_ReadCount(LPC_TIMER1);
}

/* Returns number of ticks per second of the stopwatch timer */
uint32_t StopWatch_TicksPerSecond(void)
{
	return ticksPerSecond;
}

/* Converts from stopwatch ticks to mS. */
uint32_t StopWatch_TicksToMs(uint32_t ticks)
{
	return ticks / ticksPerMs;
}

/* Converts from stopwatch ticks to uS. */
uint32_t StopWatch_TicksToUs(uint32_t ticks)
{
	return ticks / ticksPerUs;
}

/* Converts from mS to stopwatch ticks. */
uint32_t StopWatch_MsToTicks(uint32_t mS)
{
	return mS * ticksPerMs;
}

/* Converts from uS to stopwatch ticks. */
uint32_t StopWatch_UsToTicks(uint32_t uS)
{
	return uS * ticksPerUs;
}
