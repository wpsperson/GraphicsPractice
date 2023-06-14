#include "Operation.h"


void Operation::setName(const std::string& name) noexcept
{
    m_name = name;
}

const std::string& Operation::name() const noexcept
{
    return m_name;
}

void Operation::initialize(Renderer* renderer) noexcept
{

}

void Operation::paint(Renderer* renderer) noexcept
{
}
