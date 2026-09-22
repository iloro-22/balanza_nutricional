#include "ssp1.h"

/*Seteo del puerto 1 para comunicación SPI*/
void SSP1_Init(void)
{
    Chip_SCU_PinMuxSet(0x1, 19, MD_PLN | FUNC1);
    Chip_SCU_PinMuxSet(0x1, 4, MD_PLN | FUNC5);
    Chip_SCU_PinMuxSet(0x1, 3, MD_PLN | FUNC5);
    Chip_SSP_Init(LPC_SSP1);
    Chip_SSP_SetFormat(LPC_SSP1, SSP_BITS_8, SSP_FRAMEFORMAT_SPI, SSP_CLOCK_CPHA0_CPOL0);
    Chip_SSP_SetMaster(LPC_SSP1, 1);
    Chip_SSP_SetBitRate(LPC_SSP1, 10000000);
    Chip_SSP_Enable(LPC_SSP1);
}
uint8_t SSP1_transfer_byte(uint8_t data)
{
    /*Esperamos al que el buffer del transmisor este vacío*/
    while (!(LPC_SSP1->SR & SSP_STAT_TNF)) {};
    /*Enviamos el byte*/
    LPC_SSP1->DR = data;
    /*Esperamos a que el buffer del receptor este vacío*/
    while(!(LPC_SSP1->SR & SSP_STAT_RNE)) {};
    return (uint8_t)LPC_SSP1->DR;
}
void SSP1_change_format(uint8_t size){
    Chip_SSP_Disable(LPC_SSP1);
    if (size == 8)
        Chip_SSP_SetFormat(LPC_SSP1, SSP_BITS_8, SSP_FRAMEFORMAT_SPI, SSP_CLOCK_CPHA0_CPOL0);
    else if (size == 16)
        Chip_SSP_SetFormat(LPC_SSP1, SSP_BITS_16, SSP_FRAMEFORMAT_SPI, SSP_CLOCK_CPHA0_CPOL0);
    Chip_SSP_Enable(LPC_SSP1);
}