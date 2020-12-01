//Library for nRF24L01 Radio Transmitter
//Author : Daniel Bennett
//Date : 11/22/2019

#ifndef NRF24L01
#define NRF24L01

//Pin for Chip Enable
#define CE BIT0

//This is the address to use for communication
#define ADDRESS1 0xA3
#define ADDRESS2 0xB3
#define ADDRESS3 0xC3
#define ADDRESS4 0xD3
#define ADDRESS5 0xE3

#include "spi_master.h"
#include <msp430.h>

//READ COMMANDS
#define SPI_READSTATUS 0x07
#define SPI_WRITESTATUS 0x27

#define SPI_READ_ENAA 0x01
#define SPI_WRITE_ENAA 0x21

#define SPI_READ_CONFIG 0x00
#define SPI_WRITE_CONFIG 0x20

#define SPI_READ_EN_RXADDR 0x02

#define SPI_READ_RXADDR_PIPE0 0x0A
#define SPI_WRITE_RXADDR_PIPE0 0x2A

#define SPI_WRITE_TXADDR 0x30
#define SPI_READ_TXADDR 0x10

#define SPI_READ_RF_CH 0x05 //Which RF Channel
#define SPI_NOP 0xFF //Used for filler when need to read more than one byte

#define CONFIG_POWER_UP_TX 0b00001000
#define CONFIG_POWER_UP_RX 0b00001001

#define READ_RX_PAYLOAD 0b01100001
#define WRITE_TX_PAYLOAD 0b10100000

#define FLUSH_RX 0b11100010
#define FLUSH_TX 0b11100001

typedef struct T_status{
    unsigned char txfull :1;
    unsigned char rxpno :3;
    unsigned char maxrt :1;
    unsigned char txdatas :1;
    unsigned char rxdatar :1;
    unsigned char reserved :1;
}T_status;

typedef union T_Status{
    T_status bits;
    unsigned char byte;
}T_Status;

typedef struct RF_packet{
    unsigned char start;
    unsigned char pw0;
    unsigned char pw1;
    unsigned char pw2;
    unsigned char pw3;
    unsigned char command;
    unsigned char datapayload;
    unsigned char end;
}RF_packet;


void RF_init();
void RF_recieve_mode();
unsigned char RF_get_byte();
void RF_send_byte(unsigned char byte);
void RF_transmit_mode();
void RF_read_address();
unsigned char RF_read_config();
T_Status RF_get_status();
void RF_get_RXFIFO(unsigned char* buffer);
void RF_clear_IFG();
void RF_flush_RX();
RF_packet RF_get_packet();
void RF_send_packet(RF_packet* t);
void RF_set_address(unsigned char* addr);








#endif
