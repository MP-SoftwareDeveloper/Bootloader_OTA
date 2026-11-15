/*
 * bl_jump.c
 *
 *  Created on: Nov 1, 2026
 *      Author: Mazi
 */

#include "main.h"
#include "bl_jump.h"
#include "flash_layout.h"
#include "app_header.h"

#define APP_MAGIC 0xABCDEFAB

typedef void (*pFunction)(void);

void JumpToApplication(void)
{
	uint32_t appStack;
	uint32_t appResetHandler;
	pFunction appEntry;

	/* read application stack pointer */
	appStack = *(volatile uint32_t*)APP_START_ADDR;

	/* Read reset handler address */
	/*The first 4 bytes of the application flash contains the initial MSB value.for CortexM processors,
	 *  the MSB must be set before the application code can start executing*/
	appResetHandler = *(volatile uint32_t*)(APP_START_ADDR + 4); // application main function start address
	appEntry = (pFunction)appResetHandler;

	/* Disable interrupts */
	__disable_irq();

	/* Stop SysTick */
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL  = 0;

	/* Set main stack pointer */
	__set_MSP(appStack);

	/* Jump to application rest handler, What function is located there? Reset_Handler()*/
	/*
	 Reset_Handler()
	{
		SystemInit();

		__libc_init_array();

		main();
	}
	 */

	appEntry();

}

int bootloader_is_app_valid(void)
{
	uint32_t HDR_ADDR = APP_HEADER_START;
	const app_header_t *app_hdr = (const app_header_t *)HDR_ADDR;

	/* 1.Magic */
	if(app_hdr->magic != APP_MAGIC)
		return 1;

	/* 2.Reset handler sanity */
	uint32_t reset_handler = *(uint32_t *)(APP_START_ADDR + 4);
	if ((reset_handler & 0xFF000000) != 0x08000000)
		return 2;

	return 0;  // VALID
}
