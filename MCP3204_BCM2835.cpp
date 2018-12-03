/*----------------------------------------------------*\
 * This is a library to interface the MCP3204/MCP3208  *
 * ADC chip with a Raspberry Pi using BCM2835 library  *
 *                                                     *
 * Author: Marco Duarte                                *
 \----------------------------------------------------*/
 
#include "MCP3204_BCM2835.h"

bool MCP3204::begin(uint8_t spiModule, uint8_t cs)
{
  this->cs = cs;
  this->spiModule = spiModule;
  
  struct timespec delay;
  delay.tv_sec = 1;
  delay.tv_nsec = 10005;
  
  nanosleep(&delay, NULL);
  
  delay.tv_sec = 0;
  
  if(this->spiModule == SPI_AUX)
  {
    uint16_t clockDivider = bcm2835_aux_spi_CalcClockDivider(3125000);
    bcm2835_aux_spi_setClockDivider(clockDivider);
  }
  else if(this->spiModule == SPI_0)
  {
    if(this->cs != BCM2835_SPI_CS0 && this->cs != BCM2835_SPI_CS1)
    {
      printf("Choose BCM2835_SPI_CS0 or BCM2835_SPI_CS1\n");
      return false;
    }
    
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128);
    bcm2835_spi_chipSelect(this->cs);
    bcm2835_spi_setChipSelectPolarity(this->cs, LOW);
  }
  else
  {
    printf("Choose SPI_AUX or SPI_0\n");
    return false;
  }
  
  return true;
}

uint16_t MCP3204::readADC(uint8_t channel)
{
  //see page 18 of datasheet: https://www.mouser.com/datasheet/2/268/21298c-68472.pdf
  char buff[3] = {0};
  
  buff[0] = (0x1 << 2) | channel >> 2;
  buff[1] = channel << 6;
  buff[2] = 0x00;
  
  //printf("Sending: %.2x %.2x %.2x\n", buff[0], buff[1], buff[2]);
  
  if(spiModule == SPI_AUX)
    bcm2835_aux_spi_transfern(buff, sizeof(buff));
  else
  {
    bcm2835_spi_chipSelect(this->cs);
    bcm2835_spi_transfern(buff, sizeof(buff));
  }
         
  /*output should be buff[0] = XXXXXXXX
                     buff[1] = XXXXDDDD
                     buff[2] = DDDDDDDD*/
                                  
  //printf("Got: %.2x %.2x %.2x\n", buff[0], buff[1], buff[2]);
  
  return ((buff[1] << 8) | buff[2]); 
}
