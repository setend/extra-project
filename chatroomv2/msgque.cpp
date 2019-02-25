#include "msgque.h"

int Msg::getType() const
{ 
    return m_type;
}

std::string Msg::getFrom() const
{
    return m_from;
}

std::string Msg::getTo() const
{
    return m_to;
}

std::string Msg::getContent() const
{
    return m_content;
}


Msg Msgque::get() 
{
    std::unique_lock<std::mutex> lck(mtx);
    return msgque.front();
}

Msg Msgque::pop() 
{
    std::unique_lock<std::mutex> lck(mtx);
    Msg t = get();
    msgque.pop();
    return t;
}

void Msgque::add(const Msg& t)
{
    std::unique_lock<std::mutex> lck(mtx);
    msgque.push(t);

}

bool Msgque::empty() 
{
    std::unique_lock<std::mutex> lxk(mtx);
    return msgque.empty();
}

std::size_t Msgque::size() 
{
    std::unique_lock<std::mutex> lxk(mtx);
    return msgque.size();
}
