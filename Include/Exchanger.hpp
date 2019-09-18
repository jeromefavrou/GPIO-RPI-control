#ifndef EXCHANGER_HPP_INCLUDED
#define EXCHANGER_HPP_INCLUDED

#include "tram.hpp"
#include <memory>
#include <map>

class Object
{
public:

    typedef enum class TYPE  : byte const {USHORT,SHORT,FLOAT,INT,UINT,DOUBLE,STRING,BYTE,CHAR,UCHAR,LONG,ULONG} type;

    Object(byte const & _addr,type const & _type);
    ~Object(void);

    Object operator=(VCHAR const & _value);

    void set_obj(VCHAR const & _value);
    VCHAR m_data get_obj(void) const ;

    VCHAR m_data & obj(void);
    type get_type(void);

    bool lock;

private:

    VCHAR m_data;
    byte const m_addr;
    unsigned int m_size;
    type m_type_decrp;
};

class Exchanger
{
public:

    class Exchanger_Error : public Error
    {
    public:
        Exchanger_Error(int numero, std::string const& _str,level _level)throw():Error(numero,_str,_level){this->m_class="Exchanger::Error";};
        virtual ~Erreur(){};
    };

    struct Header
    {
        float trame_verbose;
        byte writer_id;
        byte reader_id;
        unsigned int trame_size;
    };

    Exchanger(byte const & Master_id,byte & Slave_id);
    ~Exchanger(void);

    template<byte const & _addr,typename T> void set_var(T const & _var);
    template<byte const & _addr,typename T> T get_var(void) const;
    template<byte const & _addr,typename T> T & var(void);

    void read(Trame const &);
    Trame write(void);

    void read(VCHAR const &);
    VCHAR write(void);

    void proto_create(byte const & _addr,Object::type const & _type);
    void create(Object const & _obj);
    void insert(byte const & _addr);
    void erase(byte const & _addr);

    Object & get_obj_ptr(void);
    Object get_obj(void)const;

private:
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
