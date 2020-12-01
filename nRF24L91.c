#include "nRF24L01.h"


unsigned char RXB[6] = {0}; //TRANSMIT AND RECIEVE BUFFERS
unsigned char TXB[6] = {0};



void RF_init()
{
    P1DIR |= CE;
    //Set Address for RX PIPE 0
    TXB[0] = SPI_WRITE_RXADDR_PIPE0;
    TXB[1] = ADDRESS1;
    TXB[2] = ADDRESS2;
    TXB[3] = ADDRESS3;
    TXB[4] = ADDRESS4;
    TXB[5] = ADDRESS5;
    spi_transmit(TXB,6,RXB);
    //SET TX ADDRESS
    TXB[0] = SPI_WRITE_TXADDR;
    TXB[1] = ADDRESS1;
    TXB[2] = ADDRESS2;
    TXB[3] = ADDRESS3;
    TXB[4] = ADDRESS4;
    TXB[5] = ADDRESS5;
    spi_transmit(TXB,6,RXB);
    //DISABLE SHOCKBURST FOR COMPATABILITY
    TXB[0] = SPI_WRITE_ENAA;
    TXB[1] = 0x00;
    spi_transmit(TXB,2,RXB);
    //SET RX PIPE0 to Recieve Packets of Size 1 byte
    TXB[0] = 0x31;
    TXB[1] = 0x01;
    spi_transmit(TXB,2,RXB);
    //Leaving RF Channel and Data Rates and Enabled pipes at default since reset settings work
}

//Put the RF_TRANSCIEVER IN TRASMIT MODE;
//Must be in transmit mode before using RF_send_byte();
void RF_transmit_mode(){
    P1OUT &= ~CE;
    TXB[0] = SPI_WRITE_CONFIG;
    TXB[1] = CONFIG_POWER_UP_RX;
    spi_transmit(TXB,2,RXB);
}

//Put the rf tranciever in recieve mode
void RF_recieve_mode(){
    P1OUT &= ~CE;
    TXB[0] = SPI_WRITE_CONFIG;
    TXB[1] = CONFIG_POWER_UP_TX;
    spi_transmit(TXB,2,RXB);
    P1OUT |= CE;
}

unsigned char RF_get_byte(){
    TXB[0] = READ_RX_PAYLOAD;
    TXB[1] = SPI_NOP;
    spi_transmit(TXB,2,RXB);
    return RXB[1];
}

void RF_send_byte(unsigned char byte){
    TXB[0] = WRITE_TX_PAYLOAD;
    TXB[1] = byte;
    spi_transmit(TXB,2,RXB);
    P1OUT |= CE;
    __delay_cycles(100); //Pulse CE high to start packet transfer
    P1OUT &= ~CE;

}






