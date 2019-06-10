#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

#include "Utility.hpp"
#include "Error.hpp"
#include "hardware_defined.hpp"

#define LOCK_PATH "/var/lock"

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

    Serial(std::string  const & pth_driver, Baud_Rate const & br):m_driver_path(pth_driver),m_baud_rate(br),m_init(false)
    {
    }
    ~Serial(void)
    {
        this->m_serial.close();
    }

    void init(void)
    {
        if(!this->check_file(this->m_driver_path))
            throw Serial_Error(0,"driver path unfound",Error::level::FATAL_ERROR);

        if(system(nullptr));
        else
            throw  Serial_Error(1,"system() not ready",Error::level::FATAL_ERROR);

        system(std::string("stty -F "+this->m_driver_path+" "+ss_cast<int,std::string>(static_cast<int>(this->m_baud_rate))).c_str());

        this->m_serial.open(m_driver_path,std::ios::in | std::ios::out | std::ios::binary);

        this->m_init=true;
    }

    void lock()
    {

    }

    Data read(void)
    {
        if(!this->m_init)
            return Data(0);

        this->m_serial.seekg (0, this->m_serial.end);
        long int length = this->m_serial.tellg();
        this->m_serial.seekg (0, this->m_serial.beg);

        Data buffer=Data(length,0);

        this->m_serial.read(buffer.data(),length);

        this->m_serial.seekg (0, this->m_serial.beg);
    }

    std::string read_str(void)
    {
        Data buff=this->read();

        return std::string(buff.begin(),buff.end());
    }
    void write(Data const & msg)
    {
        if(!this->m_init)
            return ;

        this->m_serial.seekp(0, this->m_serial.beg);

        this->m_serial.write(msg.data(),msg.size());

        this->m_serial.seekp(0, this->m_serial.beg);
    }
    void write_str(std::string const & msg)
    {
        this->write(Data(msg.begin(),msg.end()));
    }

private:

    bool check_file(std::string const & str)
    {
        std::fstream If(str,std::ios::in|std::ios::out);

        return (If && !If.bad() && !If.fail())?true:false;
    }

    bool m_init;
    std::string m_driver_path;
    Baud_Rate m_baud_rate;

    std::fstream m_serial;
};
