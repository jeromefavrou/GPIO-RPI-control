#include "Exchanger.hpp"

Exchanger::Exchanger(byte const & _Master_id,byte const & _Slave_id):m_master_id(_Master_id),m_slave_id(_Slave_id)
{
}

Exchanger::~Exchanger(void){}

byte Exchanger::get_master_id(void)const
{
    return this->m_master_id;
}

byte Exchanger::get_slave_id(void)const
{
    return this->m_slave_id;
}

void Exchanger::create(byte const & _addr,Object::type const & _type,direction const &_dir)
{
    if(this->check_free_addr(_addr,_dir))
        throw Exchanger_Error(0,"creation impossible la variable existe déja",Error::level::WARNING);

    if(_dir == DIR::INPUT)
        this->m_var.in[_addr]=std::make_shared<Object>(_addr,_type);
    else if(_dir == DIR::OUTPUT)
        this->m_var.out[_addr]=std::make_shared<Object>(_addr,_type);
}

void Exchanger::insert(std::shared_ptr<Object> const & _obj,direction const &_dir)
{
    if(this->check_free_addr(_obj->get_addr(),_dir))
        throw Exchanger_Error(1,"insertion impossible la variable existe déja",Error::level::WARNING);

    if(_dir == DIR::INPUT)
        this->m_var.in[_obj->get_addr()]=_obj;
    else if(_dir == DIR::OUTPUT)
        this->m_var.out[_obj->get_addr()]=_obj;
}
void Exchanger::erase(byte const & _addr,direction const &_dir)
{
    if(!this->check_free_addr(_addr,_dir))
        throw Exchanger_Error(2,"suppression impossible la variable n'existe pas",Error::level::WARNING);

    if(_dir == DIR::INPUT)
        this->m_var.in.erase(_addr);
    else if(_dir == DIR::OUTPUT)
        this->m_var.out.erase(_addr);
}

void Exchanger::clear(direction const &_dir)
{
    if(_dir == DIR::INPUT)
        this->m_var.in.clear();
    else if(_dir == DIR::OUTPUT)
        this->m_var.out.clear();
}

bool Exchanger::check_free_addr(byte addr,direction _dir)
{
    return false;
}
