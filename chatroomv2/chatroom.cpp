#include "chatroom.h"
int Chatroom::s_seq = 1000;

bool Chatroom::addone(Talker t)
{
  members.push_back(t);
  return true;
}

Chatroom::iterator Chatroom::getone(int ID)
{
  std::list<Talker>::iterator it= members.begin();
  while(it != members.end())
  {
    if(it->getID()== ID) break;
    ++it;
  }
  return it;
}

bool Chatroom::delone(int ID)
{

  auto it = getone(ID);
  if(it != end()){
    members.erase(it);
    return true;
  }
  return false;
}

void Chatroom::changeOwner(int ID)
{

    for(auto it = members.begin(); it != members.end();++it)
    {
        if(it->getID() == ID)
        {
            room_owner_id = ID;
            return;
        }
    }
}

