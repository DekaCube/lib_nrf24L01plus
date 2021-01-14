#include "nRF24L01.h"

unsigned char RXB[9] = {0}; //TRANSMIT AND RECIEVE BUFFERS
unsigned char TXB[9] = {0};

T_Status status_register;

//Test Commit

void RF_init() //Initalized with a default address, use RF_set_adresss to change
{
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
    //SET RX PIPE0 to Recieve Packets of Size 8 bytes
    TXB[0] = 0x31;
    TXB[1] = 0x08;
    spi_transmit(TXB,2,RXB);
    status_register.byte = RXB[0];
    //Leaving RF Channel and Data Rates and Enabled pipes at default since reset settings work
}

//Put the RF_TRANSCIEVER IN TRASMIT MODE;
void RF_transmit_mode(){
    P2DIR |= CE;
    P2OUT &= ~CE;
    TXB[0] = SPI_WRITE_CONFIG;
    TXB[1] = 0b00110010;
    spi_transmit(TXB,2,RXB);
    status_register.byte = RXB[0];
    return;
}

void RF_set_address(unsigned char* addr){
    //Set Address for RX PIPE 0
    TXB[0] = SPI_WRITE_RXADDR_PIPE0;
    TXB[1] = addr[0];
    TXB[2] = addr[1];
    TXB[3] = addr[2];
    TXB[4] = addr[3];
    TXB[5] = addr[4];
    spi_transmit(TXB,6,RXB);
    //SET TX ADDRESS for PIPE 0
    TXB[0] = SPI_WRITE_TXADDR;
    TXB[1] = addr[0];
    TXB[2] = addr[1];
    TXB[3] = addr[2];
    TXB[4] = addr[3];
    TXB[5] = addr[4];
    spi_transmit(TXB,6,RXB);
    status_register.byte = RXB[0];

}

//Put the rf tranciever in recieve mode
void RF_recieve_mode(){
    P2DIR |= CE;
    P2OUT &= ~CE;
    TXB[0] = SPI_WRITE_CONFIG;
    TXB[1] = 0b00110011;
    spi_transmit(TXB,2,RXB);
    P2OUT |= CE;
    status_register.byte = RXB[0];
    return;
}

//Dont use this function
unsigned char RF_get_byte(){
    P2OUT &= ~CE;
    TXB[0] = READ_RX_PAYLOAD;
    TXB[1] = SPI_NOP;
    spi_transmit(TXB,2,RXB);
    status_register.byte = RXB[0];
    P2OUT |= CE;
    return RXB[1];
}

RF_packet RF_get_packet(){
    P2OUT &= ~CE;
    TXB[0] = READ_RX_PAYLOAD;
    TXB[1] = SPI_NOP;
    TXB[2] = SPI_NOP;
    TXB[3] = SPI_NOP;
    TXB[4] = SPI_NOP;
    TXB[5] = SPI_NOP;
    TXB[6] = SPI_NOP;
    TXB[7] = SPI_NOP;
    TXB[8] = SPI_NOP;
    spi_transmit(TXB,9,RXB);
    status_register.byte = RXB[0];
    P2OUT |= CE;
    RF_packet packet;
    packet.start = RXB[1];
    packet.pw0 = RXB[2];
    packet.pw1 = RXB[3];
    packet.pw2 = RXB[4];
    packet.pw3 = RXB[5];
    packet.command = RXB[6];
    packet.datapayload = RXB[7];
    packet.end = RXB[8];
    return packet;

}

void RF_send_packet(RF_packet* packet){
    TXB[0] = WRITE_TX_PAYLOAD;
    TXB[1] = packet->start;
    TXB[2] = packet->pw0;
    TXB[3] = packet->pw1;
    TXB[4] = packet->pw2;
    TXB[5] = packet->pw3;
    TXB[6] = packet->command;
    TXB[7] = packet->datapayload;
    TXB[8] = packet->end;
    spi_transmit(TXB,9,RXB);
    status_register.byte = RXB[0];
    P2OUT |= CE;
    __delay_cycles(5000);
    P2OUT &= ~CE;


}


//Dont use this
void RF_get_RXFIFO(unsigned char* buffer){
    TXB[0] = READ_RX_PAYLOAD;
    TXB[1] = SPI_NOP;
    TXB[2] = SPI_NOP;
    TXB[3] = SPI_NOP;
    TXB[4] = SPI_NOP;
    TXB[5] = SPI_NOP;
    spi_transmit(TXB,2,RXB);
    status_register.byte = RXB[0];
    buffer[0] = RXB[1];
    buffer[1] = RXB[2];
    buffer[3] = RXB[3];
    buffer[4] = RXB[4];
    buffer[5] = RXB[5];
    return;


}


//Old, dont use this
void RF_send_byte(unsigned char byte){
    TXB[0] = WRITE_TX_PAYLOAD;
    TXB[1] = byte;
    spi_transmit(TXB,2,RXB);
    status_register.byte = RXB[0];
    P2OUT |= CE;
    __delay_cycles(5000);
    P2OUT &= ~CE;

}

void RF_read_address(){
    TXB[0] = SPI_READ_RXADDR_PIPE0;
    TXB[1] = SPI_NOP;
    TXB[2] = SPI_NOP;
    TXB[3] = SPI_NOP;
    TXB[4] = SPI_NOP;
    TXB[5] = SPI_NOP;
    spi_transmit(TXB,6,RXB);
    status_register.byte = RXB[0];
    return;
}

unsigned char RF_read_config(){
    TXB[0] = SPI_READ_CONFIG;
    TXB[1] = SPI_NOP;
    spi_transmit(TXB,2,RXB);
    status_register.byte = RXB[0];
    return RXB[1];

}

T_Status RF_get_status(){
    return status_register;
}

void RF_clear_IFG(){
    TXB[0] = SPI_WRITESTATUS;
    TXB[1] = 0b01110000;
    spi_transmit(TXB,2,RXB);
    status_register.byte = RXB[0];
}

void RF_flush_RX(){
    P2OUT &= ~CE;
    TXB[0] = FLUSH_RX;
    spi_transmit(TXB,1,RXB);
    status_register.byte = RXB[0];
    P2OUT |= CE;
    return;
}






