#ifndef MSGQUE_H
#define MSGQUE_H
#include <list>
#include <queue>
#include <iostream>
#include <mutex>


class Msg{
public:
  enum{ TEXT,ORDER,NOTICE};
  Msg(int type, 
      const std::string &from,
      const std::string &to,
      const std::string &content):m_type(type),m_from(from),m_to(to),m_content(content){}
  int getType() const;
  std::string getFrom() const;
  std::string getTo()const ;
  std::string getContent()const;
  
private:
  int m_type;
  std::string m_from;
  std::string m_to;
  std::string m_content;
  
};// class Msg

class Msgque{
public:
  Msg get(); 
  Msg pop();
  void add(const Msg& t);
  bool empty();
  std::size_t size(); 
  
private:
  std::queue<Msg> msgque;
  std::mutex mtx;

};// class Msgque

#endif //MSGQUE_H
