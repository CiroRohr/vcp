#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <ctype.h>
#include "hardware/gpio.h"
#include "vcp.h"

#define ONE_BYTE	1


void i2c_write_byte(uint8_t byte){

	i2c_write_blocking(i2c_default, ADDR, &byte, ONE_BYTE , false);
}

void scan_bus(){

	printf("Scanning the bus for available addresses...\n");
	
	for(uint8_t addr = 0; addr < (1 << 7); ++addr){
		
		int ret;
		uint8_t rdata;

		if((addr & 0x78) == 0 || (addr & 0x78) == 0x78){
			printf("Reserved address\n");
		}else{
		
			ret = i2c_read_blocking(i2c_default, &addr, &rdata, 1, false);

			if(ret < 0) printf("addr %d is not available. \n", addr);
			if(ret >= 0) printf("addr %d acknowledged \n", addr);
		}
	}

	printf("Done \n");
}

int set_vcp( uint8_t op, uint16_t value){

	uint8_t buf[8];
	int err = 0;

	buf[0] = 0x6E;	// destination address
	buf[1] = 0x51;	// source address
	buf[2] = 0x84;	// length 
	buf[3] = 0x03;	// set feature command
	buf[4] = op;	// vcp code
 	buf[5] = (value & 0xF0) >> 8;	// high byte 
	buf[6] = value & 0x0F;	// low byte 
 	buf[7] = (buf[0] << 1) ^ buf[1] ^ buf[2] ^ buf[3] ^ buf[4] ^ buf[5] ^ buf[6]; // checksum

	err = i2c_write_blocking(i2c_default, ADDR, buf, count_of(buf), false);
	
	return err;
}


uint16_t get_vcp(uint8_t op){

	uint8_t wbuf[6];
	uint8_t rbuf[12];
	uint16_t ret;

	wbuf[0] = 0x6E;	// destination address
	wbuf[1] = 0x51;	// source address
	wbuf[2] = 0x82;	// length 
	wbuf[3] = 0x01;	// get feature command
	wbuf[4] = op;	// vcp code
	wbuf[5] = (wbuf[0] << 1) ^ wbuf[1] ^ wbuf[2]^ wbuf[3] ^ wbuf[4]; // checksum

	i2c_write_blocking(i2c_default, ADDR, wbuf, count_of(wbuf), true);

	busy_wait_ms(40);

	i2c_read_blocking(i2c_default, ADDR, rbuf, count_of(rbuf), false);

	return 0;
}
