/*
 * bl_jump.c
 *
 *  Created on: Nov 1, 2026
 *      Author: Mazi
 */

#include "main.h"
#include "bl_jump.h"
#include "flash_layout.h"

typedef void (*pFunction)(void);

void JumpToApplication(void)
{
	uint32_t appStack;
	uint32_t appResetHandler;
	pFunction appEntry;

	/* read application stack pointer */
	appStack = *(volatile uint32_t*)APP_START_ADDR;

	/* Read reset handler address */
	appRestHandler = *(volatile uint32_t*)(APP_START_ADDR + 4); //***************************** what is this 4
	appEntry = (pFunction)appResetHandler;

	/* Disable interrupts */
	__disable_irq();

	/* Stop SysTick */
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL  = 0;

	/* Set main stack pointer */
	__set_MSP(appStack);

	/* Jump to application rest handler*/
	appEntry();

}
