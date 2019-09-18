#include "Object.hpp"

Object::Object(byte const & _addr,type const & _type):m_addr(_addr),m_type_decrp(_type),RW_right(Object::RIGHT::RW),m_size(0),m_data(VCHAR(0))
{

}

Object::~Object(void){}

Object Object::operator=(VCHAR const & _value)
{
    //control of the size??? -> assert?

    this->m_data=_value;

    return *this;
}

void Object::set_obj(VCHAR const & _value)
{
    //control of the size???

    this->m_data=_value;
}

VCHAR Object::get_obj(void) const
{
    return this->m_data;
}

VCHAR & Object::obj(void)
{
    return this->m_data;
}

Object::type Object::get_type(void) const
{
    return this->m_type_decrp;
}

byte Object::get_addr(void) const
{
    return this->m_addr;
}
