/*----------------------------------------------------*\
 * This is a library to interface the MCP3204/MCP3208  *
 * ADC chip with a Raspberry Pi using BCM2835 library  *
 *                                                     *
 * Author: Marco Duarte                                *
 \----------------------------------------------------*/
 
#ifndef MCP3204_BCM2835_h
#define MCP3204_BCM2835_h

#include <bcm2835.h>
#include <stdio.h>
#include <time.h>

#define SPI_0                 0
#define SPI_AUX               1 //aka SPI1 

//single ended
#define CH0    0x8
#define CH1    0x9
#define CH2    0xA
#define CH3    0xB
#define CH4    0xC
#define CH5    0xD
#define CH6    0xE
#define CH7    0xF

//differential CHA_B = (A-B)
#define CH0_1  0x0
#define CH1_0  0x1
#define CH2_3  0x2
#define CH3_2  0x3
#define CH4_5  0x4
#define CH5_4  0x5
#define CH6_7  0x6
#define CH7_6  0x7

class MCP3204
{
  public:
    bool begin(uint8_t spiModule=SPI_AUX, uint8_t cs=0);
    uint16_t readADC(uint8_t channel);
    
  private:
    uint8_t spiModule;
    uint8_t cs;
};

#endif
