#ifndef CHATROOM_H
#define CHATROOM_H
#include <vector>
#include <list>
#include "talker.h"
#include <iostream>

class Chatroom{
  
public:
  typedef std::list<Talker>::iterator iterator;
  Chatroom(const std::string n=""):room_name(n) { room_id = s_seq++;}
  iterator begin() { return members.begin();}
  iterator end() { return members.end();}
  int getID() { return room_id;}
  bool addone(Talker t);
 
  iterator getone(int ID);
  bool delone(int ID);
  void changeOwner(int ID);
  
  
private:
  int room_id;
  std::string room_name;
  int room_owner_id;
  std::list<Talker> members;
  static int s_seq;         
};

#endif // CHATROOM_H
