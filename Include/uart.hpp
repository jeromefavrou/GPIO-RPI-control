#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

#include "Utility.hpp"
#include "Error.hpp"
#include "hardware_defined.hpp"

#include <inttypes.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <asm/termbits.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

extern "C" int tcdrain (int __fd);

class Serial : public Nocopyable
{
public:

    class Serial_Error :public Error
    {
        public:
        Serial_Error(int numero, std::string const& _str,level _level)throw():Error(numero,_str,_level){this->m_class="Serial_Error";}
        virtual ~Serial_Error(){}
    };

    typedef std::vector<char> Data;

    Serial(std::string  const & pth_driver, Baud_Rate const & br):m_driver_path(pth_driver),m_baud_rate(br),m_init(false),m_fd(-1)
    {
    }
    ~Serial(void)
    {
        this->end();
    }

    void init(void)
    {
        struct termios2 options;

        if (this->m_fd >= 0)
            this->end();

        if(!this->check_file(this->m_driver_path))
            throw Serial_Error(0,"driver path unfound",Error::level::FATAL_ERROR);

        if(system(nullptr));
        else
            throw  Serial_Error(1,"system() not ready",Error::level::FATAL_ERROR);

        this->m_fd = open(this->m_driver_path.c_str(), O_RDWR|O_NOCTTY);

        if (this->m_fd < 0)
            return ;


        fcntl(this->m_fd, F_SETFL, 0);

        ioctl(this->m_fd, TCGETS2, &options);
        ioctl(this->m_fd, TCGETS2, &this->m_savedOptions);
        options.c_cflag &= ~CBAUD;
        options.c_cflag |= BOTHER;
        options.c_ispeed = static_cast<unsigned long>(this->m_baud_rate);
        options.c_ospeed = static_cast<unsigned long>(this->m_baud_rate);
        this->m_savedOptions.c_cflag |= BOTHER;
        this->m_savedOptions.c_ispeed = static_cast<unsigned long>(this->m_baud_rate);
        this->m_savedOptions.c_ospeed = static_cast<unsigned long>(this->m_baud_rate);
        options.c_cflag |= (CLOCAL | CREAD);
        options.c_cflag &= ~CRTSCTS;
        options.c_cflag &= ~HUPCL;
        this->m_savedOptions.c_cflag &= ~HUPCL;

        options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                    | INLCR | IGNCR | ICRNL | IXON);
        options.c_oflag &= ~OPOST;
        options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
        options.c_cflag &= ~(CSIZE | PARENB);
        options.c_cflag |= CS8;

        if (strcmp(this->m_driver_path.c_str(), "/dev/tty") == 0)
            options.c_lflag |= ISIG;

        ioctl(this->m_fd, TCSETS2, &options);

        this->m_init=true;
    }

    Data read(void)
    {
        if (!this->available())
            return Data(0);

        char *c;

        if (::read(this->m_fd, c, 1) <= 0)
            return Data(0);

        Data d;
        d.push_back(*c);

        return d;
    }

    std::string read_str(void)
    {
        Data buff=this->read();

        return std::string(buff.begin(),buff.end());
    }
    void write(Data const & msg)
    {
        if (this->m_fd < 0 || !this->m_init)
            return;

        ::write(this->m_fd, msg.data() , msg.size());
    }
    void write_str(std::string const & msg)
    {
        this->write(Data(msg.begin(),msg.end()));
    }

    inline int getFD() { return this->m_fd; }

private:

    bool check_file(std::string const & str)
    {
        std::fstream If(str,std::ios::in|std::ios::out);

        return (If && !If.bad() && !If.fail())?true:false;
    }

    void end()
    {
        if (this->m_fd > -1)
        {
            tcdrain(this->m_fd);
            ioctl(this->m_fd, TCSETS2, &this->m_savedOptions);
            ::close(this->m_fd);
        }
        this->m_fd = -1;
    }

    bool available(void)
    {
        if (this->m_fd < 0 || !m_init)
            return false;

        fd_set rfds;
        struct timeval tv;

        FD_ZERO(&rfds);
        FD_SET(this->m_fd, &rfds);
        tv.tv_sec = 0;
        tv.tv_usec = 1;
        int retval = select(this->m_fd+1, &rfds, NULL, NULL, &tv);
        if (retval)
            return true;

        return false;
    }

    bool m_init;
    std::string m_driver_path;
    Baud_Rate m_baud_rate;

    int m_fd;
    struct termios2 m_savedOptions;
};
