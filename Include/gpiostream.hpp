#ifndef GPIOSTREAM_HPP_INCLUDED
#define GPIOSTREAM_HPP_INCLUDED

#include "Rpi_ctrl.hpp"
#include <string>
#include <vector>

typedef enum class FSEL : std::uint8_t const{INPUT = 0,OUTPUT ,ALT0, ALT1 ,ALT2,ALT3 ,ALT4 ,ALT5,MASK,}Function_Select;

template<Soc_hardware_version version,Rasp_version::Pin_gpio pin> class GPIO : public RPi_Controler<version>
{
public:

    GPIO(std::shared_ptr< Hardware_memory<version> > & paddr):RPi_Controler<version>(paddr){}
    virtual ~GPIO(){}

    void direction(Function_Select const & mode)
    {
        this->set_bits(this->Rpi_base(this->get_po_gp<0>(PO_GP::GPFSEL))+pin/10 ,this->get_fsel(mode) << ((pin % 10) * 3) ,this->get_fsel(FSEL::MASK) << ((pin % 10) * 3));
    }

    void set(Level const& lev)
    {
        this->write(this->Rpi_base(( lev==LEV::HIGH ? this->get_po_gp<0>(PO_GP::GPSET) : this->get_po_gp<0>(PO_GP::GPCLR)) ) + pin/32, 1 << (pin % 32) );
    }

    Level get(void)
    {
        return ( this->read(this->Rpi_base(this->get_po_gp<0>(PO_GP::GPLEV))+ pin/32) & (1 << (pin % 32 ) ) ) ? LEV::HIGH : LEV::LOW;
    }

    Level get_eds(void)
    {
        return (this->read(this->Rpi_base(this->get_po_gp<0>(PO_GP::GPEDS)) + pin/32) & (1 << (pin % 32) )) ? LEV::HIGH : LEV::LOW;
    }

    void set_eds(void)
    {
        this->write(this->Rpi_base(this->get_po_gp<0>(PO_GP::GPEDS)) + pin/32, 1 << (pin % 32));
    }

    std::uint8_t const get_fsel(Function_Select const & fs) const
    {
        switch(fs)
        {
            case FSEL::INPUT: return 0b000;  break;
            case FSEL::OUTPUT: return 0b001;  break;
            case FSEL::ALT0: return 0b100;  break;
            case FSEL::ALT1: return 0b101;  break;
            case FSEL::ALT2: return 0b110;  break;
            case FSEL::ALT3: return 0b111;  break;
            case FSEL::ALT4: return 0b011;  break;
            case FSEL::ALT5: return 0b010;  break;
            case FSEL::MASK: return 0b111;  break;

            default :return 0b111 ;
        }

        return 0b111;
    }

protected:

    template<uint8_t M> std::uint32_t const get_po_gp(SoC_gp_physical_offset const & po_gp)
    {
        switch(po_gp)
        {
            case PO_GP::GPFSEL: return 0x0000 + (M << 2); break;
            case PO_GP::GPSET: return 0x001c + (M << 2); break;
            case PO_GP::GPCLR: return 0x0028 + (M << 2); break;
            case PO_GP::GPLEV: return 0x0034 + (M << 2); break;
            case PO_GP::GPEDS: return 0x0040 + (M << 2); break;
            case PO_GP::GPREN: return 0x004c + (M << 2); break;
            case PO_GP::GPFEN: return 0x0058 + (M << 2); break;
            case PO_GP::GPHEN: return 0x0064 + (M << 2); break;
            case PO_GP::GPLEN: return 0x0070 + (M << 2); break;
            case PO_GP::GPAREN: return 0x007c + (M << 2); break;
            case PO_GP::GPAFEN: return 0x0088 + (M << 2); break;
            case PO_GP::GPPUD: return 0x0094 + (M << 2); break;
            case PO_GP::GPPUDCLK: return 0x0098 + (M << 2); break;

            default : return 0xFFFF;
        }

        return 0xFFFF;
    }
};

#endif // GPIOSTREAM_HPP_INCLUDED
