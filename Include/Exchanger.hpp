#ifndef EXCHANGER_HPP_INCLUDED
#define EXCHANGER_HPP_INCLUDED

#include "Object.hpp"
#include <memory>
#include <map>


class Exchanger
{
public:

    class Exchanger_Error : public Error
    {
    public:
        Exchanger_Error(int numero, std::string const& _str,level _level)throw():Error(numero,_str,_level){this->m_class="Exchanger::Error";};
        virtual ~Exchanger_Error(){};
    };

    struct Header
    {
        float trame_verbose;
        byte writer_id;
        byte reader_id;
        unsigned int trame_size;
    };

    typedef enum class DIR : byte const {INPUT, OUTPUT} direction;

    Exchanger(byte const & _Master_id,byte const & _Slave_id);
    ~Exchanger(void);

    byte get_master_id(void)const;
    byte get_slave_id(void)const;

    Object & get_obj_ptr(void);
    Object get_obj(void)const;

    template<byte const & _addr,typename T> void set_var(T const & _var);
    template<byte const & _addr,typename T> T get_var(void) const;
    template<byte const & _addr,typename T> T & var(void);

    void read(Tram const &);
    Tram write(void);

    void read_data(VCHAR const &);
    VCHAR write_data(void);

    void create(byte const & _addr,Object::type const & _type,direction const &_dir);
    void insert(std::shared_ptr<Object> const & _obj,direction const &_dir);
    void erase(byte const & _addr,direction const &_dir);
    void clear(direction const &_dir);

private:

    bool check_free_addr(byte addr,direction _dir);

    struct Var
    {
        std::map<byte,std::shared_ptr<Object>> in;
        std::map<byte,std::shared_ptr<Object>> out;
    };

    struct Var m_var;


    byte const m_master_id;
    byte const m_slave_id;
};


#endif // EXCHANGER_HPP_INCLUDED
