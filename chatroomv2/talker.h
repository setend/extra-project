#ifndef TALKER_H
#define TALKER_H
#include <arpa/inet.h>
#include <iostream>

class Talker{
public:
  Talker(const std::string &n, in_addr_t a):name(n),addr(a){ ID = s_seq++;}
  bool match(int i) { return i == ID;}
  bool match(std::string t) { return t== name;}
  int getID() { return ID;}
  in_addr_t getAddr() { return addr;}
private:
  int ID;
  std::string name;
  in_addr_t addr;
  static int s_seq;
  
}; //class Talker

#endif //TALKER_H
