/**
 * Experiments with Flash
 * Flashing in a multicore example, using bare metal
 * Jon Durrant - 2023
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "pico/multicore.h"
#include <cstring>
#include <math.h>

#define LED_PIN 15
#define PAGE_SIZE 256
#define NVS_SIZE 4096
#define FLASH_WRITE_START (PICO_FLASH_SIZE_BYTES - NVS_SIZE)
#define FLASH_READ_START  (FLASH_WRITE_START + XIP_BASE)

/***
 * Timer Callback
 */
bool timerCB (repeating_timer_t *rt){
	int r = 2 * 7;
	return true;
}

/***
 * Core 1 Role to flash LED and do some maths
 */
void core1Entry(){
	bool on = true;
	multicore_lockout_victim_init();
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);
	gpio_put(LED_PIN, on);
	for(;;){
		for (int i=1; i < 0xFFFF; i++ ){
			double sq = (double)i;
			double r = sqrt(sq);

		}
			on = !on;
			gpio_put(LED_PIN, on);
	}
}

int main() {
	char str[PAGE_SIZE] = "This is written to flash.";
	char *flashStr = (char *)FLASH_READ_START;
	repeating_timer_t xTimer;

	//Initialise IO as we are using printf for debug
	stdio_init_all();

	sleep_ms(2000);
	printf("Starting\n");

	// Start core 1
	multicore_launch_core1(core1Entry);

	//Add Repeating interrupt
	add_repeating_timer_us	(
	    			1000,
					timerCB,
					NULL,
					&xTimer
	    	);

	//START Critical Section
	uint32_t status = save_and_disable_interrupts();
	multicore_lockout_start_blocking();

	 flash_range_erase(FLASH_WRITE_START, NVS_SIZE);
	 flash_range_program(FLASH_WRITE_START, (const uint8_t *)str,  PAGE_SIZE);

	 multicore_lockout_end_blocking();
	 restore_interrupts(status);
	 //END Critical Section

	 printf("From Flash: %s\n", flashStr);


	 for (;;){
		 sleep_ms(2000);
	 }


}
