#ifndef RASPBERRY_HARDWARE_HPP_INCLUDED
#define RASPBERRY_HARDWARE_HPP_INCLUDED

#include "Error.hpp"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "hardware_defined.hpp"

template<Soc_hardware_version version> class Hardware_memory
{
public:

    class Hardware_memory_Error :public Error
    {
        public:
        Hardware_memory_Error(int numero, std::string const& _str,level _level)throw():Error(numero,_str,_level){this->m_class="Hardware_memory_Error";};
        virtual ~Hardware_memory_Error(){};
    };

    Hardware_memory();
    ~Hardware_memory();

    void init(SoC_physical_addr const & base);
    void free(void);

    Hardware_ptr get_ptr(void);

    std::uint32_t const get_peripheral_addr(void) const;
    std::uint32_t const get_page_size(void) const;
    std::uint32_t const get_block_size(void) const;
    std::uint32_t const get_pa(SoC_physical_addr const & pa);

    constexpr Soc_hardware_version get_version(void) const;

private:

    Hardware_ptr ptr_map;

    std::uint32_t BLOCK_SIZE;
    std::uint32_t PAGE_SIZE;
    std::uint32_t PERIPHERAL_ADDR;
};

template<Soc_hardware_version version> Hardware_memory<version>::Hardware_memory():ptr_map(nullptr)
{
    if( version == HARDWARE_VERSION::BCM2835 )
        this->PERIPHERAL_ADDR= 0x20000000;

    else if(version > HARDWARE_VERSION::BCM2835)
        this->PERIPHERAL_ADDR= 0x3F000000;

    this->BLOCK_SIZE=4096;
    this->PAGE_SIZE=4096;
}

template<Soc_hardware_version version> Hardware_memory<version>::~Hardware_memory()
{
    this->free();
}

template<Soc_hardware_version version> void Hardware_memory<version>::free(void)
{
    if(this->ptr_map==nullptr)
        return;

    munmap(&this->ptr_map,this->BLOCK_SIZE);

    this->ptr_map=nullptr;
}

template<Soc_hardware_version version> void Hardware_memory<version>::init(SoC_physical_addr const & base)
{
    if(this->ptr_map != nullptr)
        throw Hardware_memory_Error(1,"/dev/mem isn't working",Hardware_memory_Error::level::WARNING);

    int memfd {-1};

    memfd = open("/dev/mem", O_RDWR | O_SYNC);

    if ( memfd < 0)
    {
        close(memfd);
        throw Hardware_memory_Error(2,"/dev/mem isn't working",Hardware_memory_Error::level::FATAL_ERROR);
    }

    this->ptr_map=static_cast<Hardware_ptr>(MAP_FAILED);

    this->ptr_map= static_cast<Hardware_ptr>(mmap(NULL, this->BLOCK_SIZE , PROT_READ|PROT_WRITE, MAP_SHARED,memfd,this->PERIPHERAL_ADDR+this->get_pa(base)));

    close(memfd);

    if(this->ptr_map == static_cast<Hardware_ptr>(MAP_FAILED))
    {
        this->ptr_map=nullptr;
        throw Hardware_memory_Error(3,"Memory mapping has failed",Hardware_memory_Error::level::FATAL_ERROR);
    }
}

template<Soc_hardware_version version> Hardware_ptr Hardware_memory<version>::get_ptr(void)
{
    return this->ptr_map;
}

template<Soc_hardware_version version> constexpr Soc_hardware_version Hardware_memory<version>::get_version(void) const
{
    return version;
}

template<Soc_hardware_version version> std::uint32_t const Hardware_memory<version>::get_peripheral_addr(void) const
{
    return this->PERIPHERAL_ADDR;
}

template<Soc_hardware_version version> std::uint32_t const Hardware_memory<version>::get_page_size(void) const
{
    return this->PAGE_SIZE;
}

template<Soc_hardware_version version> std::uint32_t const Hardware_memory<version>::get_block_size(void) const
{
    return this->BLOCK_SIZE;
}

template<Soc_hardware_version version> std::uint32_t const Hardware_memory<version>::get_pa(SoC_physical_addr const & pa)
{
    switch(pa)
    {
        case PA::GPIO: return 0x00200000; break;
        case PA::ST: return 0x00003000; break;
        case PA::PADS: return 0x00100000; break;
        case PA::CLOCK: return 0x00101000; break;
        case PA::SPI: return 0x00204000; break;
        case PA::BSC0: return 0x00205000; break;
        case PA::BSC1: return 0x00804000; break;
        case PA::PWM: return 0x0020C000; break;

        default :throw Hardware_memory_Error(5,"error for getting physical address",Hardware_memory_Error::level::FATAL_ERROR);
    }

    return 0xFFFFFFFF;
}

#endif // RASPBERRY_HARDWARE_HPP_INCLUDED
