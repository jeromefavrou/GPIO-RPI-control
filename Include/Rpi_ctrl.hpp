#ifndef RPI_CTRL_HPP_INCLUDED
#define RPI_CTRL_HPP_INCLUDED

#include "Raspberry_Hardware.hpp"
#include <memory>

typedef enum class LEV : std::uint8_t const
{
    LOW=0x00,
    HIGH=0x01,

}Level;

template<Soc_hardware_version version>class RPi_Controler : public Nocopyable
{
public:

    RPi_Controler(std::shared_ptr< Hardware_memory<version>> & paddr):m_ptr_map(paddr){}

    virtual ~RPi_Controler(){}

protected:

    void write(Hardware_ptr paddr,uint32_t value)
    {
        *paddr=value;
        *paddr=value;
    }

    uint32_t read(Hardware_ptr paddr)
    {
        uint32_t buff { *paddr };
        *paddr;
        return buff;
    }

    Hardware_ptr Rpi_base (uint32_t const offset)
    {
        return this->m_ptr_map->get_ptr() + (offset >> 2);
    }

    void set_bits(Hardware_ptr paddr, uint32_t value, uint32_t mask)
    {
        uint32_t v { read(paddr) };

        v = (v & ~mask) | (value & mask);

        write(paddr, v);
    }

    std::shared_ptr< Hardware_memory<version> > m_ptr_map;
};

#endif // RPI_CTRL_HPP_INCLUDED
