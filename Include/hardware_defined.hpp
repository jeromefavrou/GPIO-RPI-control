#ifndef HARDWARE_DEFINED_HPP_INCLUDED
#define HARDWARE_DEFINED_HPP_INCLUDED

#include "Utility.hpp"

class  RPI_3BP
{
public:

    static std::uint8_t const
    PIN_3=2, GPIO_2=PIN_3, SDA1=PIN_3,
    PIN_5=3, GPIO3=PIN_5, SCL1=PIN_5,
    PIN_7=4, GPIO_4= PIN_7, GPIO_GCLK= PIN_7,
    PIN_8=14, GPIO_14= PIN_8, UART0_TX= PIN_8,
    PIN_10=15, GPIO_15= PIN_8, UART0_RX= PIN_8,
    PIN_11=17, GPIO_17= PIN_11, GPIO_GEN0= PIN_11,
    PIN_12=18, GPIO_18= PIN_12, GPIO_GEN1= PIN_12,
    PIN_13=27, GPIO_27= PIN_13, GPIO_GEN2= PIN_13,
    PIN_15=22, GPIO_22= PIN_15, GPIO_GEN3= PIN_15,
    PIN_16=23, GPIO_23= PIN_16, GPIO_GEN4= PIN_16,
    PIN_18=24, GPIO_24= PIN_18, GPIO_GEN5= PIN_18,
    PIN_19=10, GPIO_10= PIN_19, SPI0_MOSI= PIN_19,
    PIN_21=9, GPIO_9= PIN_21, SPI0_MISO= PIN_21,
    PIN_22=25, GPIO_25= PIN_22, GPIO_GEN6= PIN_22,
    PIN_23=11, GPIO_11= PIN_23, SPI0_CLK= PIN_23,
    PIN_24=8, GPIO_8= PIN_24, SPI0_CE0_N= PIN_24,
    PIN_26=7, GPIO_7= PIN_26, SPI0_CE1_N= PIN_26,
    PIN_29=5, GPIO_5= PIN_29,
    PIN_31=6, GPIO_6= PIN_31,
    PIN_32=12, GPIO_12= PIN_32,
    PIN_33=13, GPIO_13= PIN_33,
    PIN_35=19, GPIO_19= PIN_35,
    PIN_36=16, GPIO_16= PIN_36,
    PIN_37=26, GPIO_26= PIN_37,
    PIN_38=20, GPIO_20= PIN_38,
    PIN_40=21, GPIO_21= PIN_40;
};

class  RPI_3B : public RPI_3BP
{
};


typedef volatile uint32_t * Hardware_ptr;

typedef enum class HARDWARE_VERSION  : int const {BCM2835=0,BCM2836,BCM2837,BCM2837B0,BCM2711} Soc_hardware_version;

typedef enum class PO_GP : int const {GPFSEL=0,GPSET,GPCLR,GPLEV,GPEDS,GPREN,GPFEN,GPHEN,GPLEN,GPAREN,GPAFEN,GPPUD,GPPUDCLK,} SoC_gp_physical_offset;

typedef enum class PA : int const {GPIO=0,ST,PADS,CLOCK,SPI,BSC0,BSC1,PWM,} SoC_physical_addr;

typedef enum class FSEL : std::uint8_t const {INPUT = 0,OUTPUT ,ALT0, ALT1 ,ALT2,ALT3 ,ALT4 ,ALT5,MASK,}Function_Select;

typedef enum class LEV : std::uint8_t const {LOW=0x00,HIGH=0x01,}Level;

typedef enum class BAUD : unsigned long const{BR_9600=9600,BR_19200=19200,BR_57600=57600,BR_115200=115200,} Baud_Rate;

#endif // HARDWARE_DEFINED_HPP_INCLUDED
