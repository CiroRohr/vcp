#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <ctype.h>
#include "hardware/gpio.h"
#include "vcp.h"
#include "pico/cyw43_arch.h"

const int mon_addr = 0x03;


void gpio_irq_handler(uint gpio, uint32_t events){

	printf("irq event detected \n");
	
	set_vcp(0x60, 0x35); // random inputs	
	busy_wait_ms(300);

}

int main(){
	
	stdio_init_all();

	cyw43_arch_init();

	printf("Hello, VCP!\n");

	i2c_init(i2c_default, 100*1000);
	gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
	gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
	
	gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
	gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

	uint8_t buf[1];
 	buf[0]	= 0xBE;

	gpio_set_irq_enabled_with_callback(2, GPIO_IRQ_EDGE_RISE, true, &gpio_irq_handler);
	
	scan_bus();	
	
	while(true){
		

		printf("Hello, VCP!\n");
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

		#ifndef i2c_default
		print("Not defined!!");
		#endif

		//i2c_write_blocking(i2c_default, mon_addr, buf, count_of(buf), false);

		sleep_ms(300);
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
		sleep_ms(100);
	}



}


