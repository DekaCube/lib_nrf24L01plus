#include "spi_master.h"

unsigned char TX = 0;
unsigned char RXBUFFER = 0;


unsigned char flip(unsigned char var){
    bool bit0 = var & BIT0;
    bool bit1 = var & BIT1;
    bool bit2 = var & BIT2;
    bool bit3 = var & BIT3;
    bool bit4 = var & BIT4;
    bool bit5 = var & BIT5;
    bool bit6 = var & BIT6;
    bool bit7 = var & BIT7;
    var = 0;
    if(bit0){var |= BIT7;}
    if(bit1){var |= BIT6;}
    if(bit2){var |= BIT5;}
    if(bit3){var |= BIT4;}
    if(bit4){var |= BIT3;}
    if(bit5){var |= BIT2;}
    if(bit6){var |= BIT1;}
    if(bit7){var |= BIT0;}
    return var;

}

void spi_init(){
    P2DIR |= MOSI|SCLK|NSS;
    P2DIR &= ~MISO;
    P2OUT |= NSS; //Set NSS high for RF
    __delay_cycles(100); //Wait for device to be fully powered down

}

void spi_transmit(unsigned char *txbuff,
                  unsigned char numb,unsigned char* rxbuff){
    P2OUT &= ~NSS; //Set Slave Select
    char j = 0;
    for(j = 0;j < numb;j++){ //Loop for each byte
        char i = 0;
        rxbuff[j] = 0; //Clear the RX buffer
        TX = txbuff[j]; //Load buffer frame into TX
        TX = flip(TX); //Flip the byte, sending MSB first
        for(i = 0;i < 8;i++){ //Loop for software acting as shift registers to RX/TX
            if(TX & 0b00000001){ //Send 1 or 0
                P2OUT |= MOSI;}
            else{
                P2OUT &= ~MOSI;
            }
            //Change this to suit your needs
            __delay_cycles(50); //Wait a bit for slower SPI controls, especially a bit banged slave;
            P2OUT |= SCLK; //RAISE CLOCK TO START TRANSFER
            __delay_cycles(100); // wait again (change as needed)
            RXBUFFER  = (P2IN & MISO) ? 1 : 0;
            if(RXBUFFER){rxbuff[j] |= RXBUFFER;}
            if(!RXBUFFER){rxbuff[j] = rxbuff[j];}
            if(i < 7){ //Shift the buffer if appropriate
                rxbuff[j] = rxbuff[j] << 1;}
            P2OUT &= ~SCLK; //lower sclock to prepare for next transaction
            TX = TX >> 1; //Shift next bit for transmission
        }
    }
    P2OUT |= NSS; //Set Slave Select High to End Transaction
    return;



}
