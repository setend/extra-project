#ifndef MSGQUE_H
#define MSGQUE_H
#include <list>
#include <queue>
#include <iostream>

class Msg{
public:
  enum{ TEXT,ORDER,NOTICE};
  Msg(int type, 
      const std::string &from,
      const std::string &to,
      const std::string &content):m_type(type),m_from(from),m_to(to),m_content(content){}
  int getType() const{ return m_type;}
  std::string getFrom() const { return m_from;}
  std::string getTo()const { return m_to;}
  std::string getContent()const { return m_content;}
  
private:
  int m_type;
  std::string m_from;
  std::string m_to;
  std::string m_content;
  
};// class Msg

class Msgque{
public:
  Msg get() { return msgque.front();}
  Msg pop() {Msg t = get(); msgque.pop(); return t;}
  bool add(const Msg& t) {msgque.push(t);}
  bool empty() { return msgque.empty();}
  std::size_t size() { return msgque.size();}
  
private:
  std::queue<Msg> msgque;
};// class Msgque

#endif //MSGQUE_H