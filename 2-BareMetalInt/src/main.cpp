/**
 * Experiments with Flash
 * Simple write to top of flash with no interrupt protection
 * Jon Durrant - 2023
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/flash.h"
#include "hardware/sync.h"
#include <cstring>

#define PAGE_SIZE 256
#define NVS_SIZE 4096
#define FLASH_WRITE_START (PICO_FLASH_SIZE_BYTES - NVS_SIZE)
#define FLASH_READ_START  (FLASH_WRITE_START + XIP_BASE)

bool timerCB (repeating_timer_t *rt){
	int r = 2 * 7;
	return true;
}



int main() {
	char str[PAGE_SIZE] = "This is written to flash.";
	char *flashStr = (char *)FLASH_READ_START;
	repeating_timer_t xTimer;

	//Initialise IO as we are using printf for debug
	stdio_init_all();

	sleep_ms(2000);
	printf("Starting\n");

	//Add Repeating interrupt
	add_repeating_timer_us	(
	    			1000,
					timerCB,
					NULL,
					&xTimer
	    	);

	//START Critical Section
	uint32_t status = save_and_disable_interrupts();

	 flash_range_erase(FLASH_WRITE_START, NVS_SIZE);
	 flash_range_program(FLASH_WRITE_START, (const uint8_t *)str,  PAGE_SIZE);

	 restore_interrupts(status);
	 //END Critical Section

	 printf("From Flash: %s\n", flashStr);


	 for (;;){
		 sleep_ms(2000);
	 }


}
