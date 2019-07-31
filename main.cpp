#include <iostream>
#include "gpiostream.hpp"
#include "uart.hpp"
#include <stdint.h>

int main()
{
    std::shared_ptr<Hardware_memory<HARDWARE_VERSION::BCM2837B0>> gpio_mem=std::make_shared<Hardware_memory<HARDWARE_VERSION::BCM2837B0>>(Hardware_memory<HARDWARE_VERSION::BCM2837B0>{});

    Serial serial0("/dev/AMA0",BAUD::BR_9600);

    try
    {
        std::cout << "opening physicals address: 0x"<< std::hex << gpio_mem->get_peripheral_addr()+gpio_mem->get_pa(PA::GPIO)<< " to 0x" <<gpio_mem->get_peripheral_addr()+gpio_mem->get_pa(PA::GPIO) + gpio_mem->get_block_size() <<std::endl<<std::endl;

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

    for(auto i=0;i<100;i++)
    {
        gp_test.set(i%2?LEV::HIGH:LEV::LOW);

        std::this_thread::sleep_for(std::chrono::duration<int,std::milli>(100));
    }

    gp_test.direction(FSEL::MASK);

    try
    {
       serial0.write("test_en");

       std::this_thread::sleep_for(std::chrono::duration<int,std::milli>(50));

       int len=serial0.availble();

       for(;len>0;len--)
       {
           std::cout << serial0.read_byte();
       }

       std::cout << std::endl;
    }
    catch(Error & e)
    {
        std::cout << e.what() << std::endl;

        if(e.get_level()!=Error::level::WARNING)
            return -1;
    }

    return 0;
}
