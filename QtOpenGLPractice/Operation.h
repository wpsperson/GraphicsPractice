#pragma once

#include <string>

class Renderer;

class Operation
{
public:
    virtual ~Operation() = default;

    void setName(const std::string& name) noexcept;

    const std::string& name() const noexcept;

    virtual void initialize(Renderer *renderer) noexcept;

    virtual void paint(Renderer* renderer) noexcept;

private:
    std::string m_name;
};
