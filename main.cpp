#include <iostream>
#include "gpiostream.hpp"
#include <stdint.h>

int main()
{
    std::shared_ptr<Hardware_memory<HARDWARE_VERSION::BCM2837B0>> gpio_mem=std::make_shared<Hardware_memory<HARDWARE_VERSION::BCM2837B0>>(Hardware_memory<HARDWARE_VERSION::BCM2837B0>{});

    try
    {
        std::cout << "open physicals address: 0x"<< std::hex << gpio_mem->get_peripheral_addr()+gpio_mem->get_pa(PA::GPIO)<< " to 0x" <<gpio_mem->get_peripheral_addr()+gpio_mem->get_pa(PA::GPIO) + gpio_mem->get_block_size() <<std::endl<<std::endl;

        gpio_mem->init(PA::GPIO);
    }
    catch(Error & e)
    {
        std::cout << e.what() << std::endl;

        if(e.get_level()==Error::level::FATAL_ERROR)
            return -1;
    }

    GPIO<HARDWARE_VERSION::BCM2837B0,Rasp_version::PI_3BP::GPIO_4> gp_test{gpio_mem};

    gp_test.direction(FSEL::OUTPUT);

    for(auto i=0;i<10;i++)
        gp_test.set(i%2?LEV::HIGH:LEV::LOW);


    gp_test.direction(FSEL::MASK);

    return 0;
}
