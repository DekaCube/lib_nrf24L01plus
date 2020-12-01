//Bit Banged 8-bit SPI Master
//Author : Daniel Bennett
//Date: 11/22/19

//Note : THIS CAN ONLY BE USED AS A *MASTER* FOR 8-BIT SPI
//This code will not work as a slave device, it is not listening for edge high on SCLK



#ifndef BITBANG_SPI_MASTER
#define BITBANG_SPI_MASTER

#include <msp430.h>
#include <stdbool.h>

//PIN SETUP
//Only Use Port 1 Please on MSP430G2553

#define NSS BIT5

#define MOSI BIT4
#define MISO BIT2
#define SCLK BIT1
//END PIN SETUP

//Stick this somewhere in board init
void spi_init();

//Internal helper function
//@param var : input to be flipped LSB->MSB
unsigned char flip(unsigned char var);


//Peforms both transmit and recieve
//@Param txbuffer : The bytes to be transmitted
//@Param rxbuffer : The bytes to be recieved
//@Param numbyes : The size of the buffers in bytes
void spi_transmit(unsigned char *txbuffer,
                  unsigned char numbyes,unsigned char *rxbuffer);



#endif
