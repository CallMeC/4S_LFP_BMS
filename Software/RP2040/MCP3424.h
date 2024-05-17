#ifndef MCP3424_H
#define MCP3424_H

//Paramètres de l'ADC
#define BITS    18      //ADC Resolution
#define CH1_HEX 0x1C    //Ch1, AGC=1, 3.75 sps (18 bits)
#define CH2_HEX 0x3C    //Ch2, AGC=1, 3.75 sps (18 bits)
#define CH3_HEX 0x5C    //Ch3, AGC=1, 3.75 sps (18 bits)
#define CH4_HEX 0x7C    //Ch4, AGC=1, 3.75 sps (18 bits)

class c_MCP3424
{
    public:
        c_I2C I2C_ADC;
        uint8_t ADC_ADDRESS;
        void Init(uint8_t x,  c_I2C whichI2C);
        c_MCP3424();
        int readADC(uint8_t ADC_CHANNEL);  //Fait une lecture pour une voie
};

#endif