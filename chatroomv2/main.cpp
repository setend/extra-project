#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <string.h>

#include "chatroom.h"
#include "msgque.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace std;

const int PORT = 8999;
Chatroom chatroom;
Msgque msg_rcv_que;   // message recieve queue
Msgque msg_snd_que;     // message send queue
int myRead(int fd,char *buf,int n);   // read from socket
int myWrite(int fd, char *buf, int n); // write to socket
Msg msgJsonParse(char *);   // parse json to msg format
void msgJsonPack(char *,const Msg& m); // pack msg to json format
int make_tcp_socket();    // as its name
void* login_handler(int sockfd);  // deal with user login
int make_udp_socket();           //  make udp socket
void* rcv_msg_handler(int sockfd);  // deal with received message
void* mid_deal_handler();           // classify the message and add different queuq
void* snd_msg_handler(int sockfd); // deal with message need to send 

int main(int argc, char **argv) {
    int tcp_sockfd = make_tcp_socket();
    if(tcp_sockfd < 0)
    {
      cerr << " tcp socket error" << endl;
      return 1;
    }
    int udp_sockfd = make_udp_socket();
    if(udp_sockfd < 0)
    {
      cerr << " udp socket error" << endl;
      return 1;
    }
    std::thread t1(login_handler,tcp_sockfd);
    std::thread t2(rcv_msg_handler,udp_sockfd);
    std::thread t3(mid_deal_handler);
    std::thread t4(snd_msg_handler,udp_sockfd);
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    return 0;
}

int myRead(int fd, char* buf, int n)
{
  int k=0;
  while(k<n)
  {
    k = read(fd,buf,n);
    if( k <= 0) return -1;
    n -=k;
    buf+=k;
  }
  return 0;
}

int myWrite(int fd, char* buf, int n)
{
  int k = 0;
  while(k<n)
  {
    k = write(fd,buf,n);
    if( k<= 0) return -1;
    n -=k;
    buf+=k;
  }
  return 0;

}

Msg msgJsonParse(char* json)
{
  Document d;
  d.Parse(json);
  int type = d["type"].GetInt();
  string from = d["from"].GetString();
  string to = d["to"].GetString();
  string content = d["content"].GetString();
  return Msg(type,from,to,content);

}

void msgJsonPack(char* buf, const Msg& m){
  Document d;
  d.SetObject();
  int a = m.getType();
  d.AddMember("type",a,d.GetAllocator());
  d.AddMember("from",StringRef(m.getFrom().c_str()),d.GetAllocator());
  d.AddMember("to",StringRef(m.getTo().c_str()),d.GetAllocator());
  d.AddMember("content",StringRef(m.getContent().c_str()),d.GetAllocator());
  StringBuffer jsonbuf;
  Writer<StringBuffer> writer(jsonbuf);
  d.Accept(writer);
  memcpy(buf,jsonbuf.GetString(),jsonbuf.GetSize());

}


int make_tcp_socket()
{
  sockaddr_in host;
  
  int sockfd = socket(PF_INET,SOCK_STREAM,0);
  if(sockfd == -1) return -1;
  memset(&host,0,sizeof host);
  
  host.sin_family = AF_INET;
  host.sin_port = htons(PORT);
  host.sin_addr.s_addr = htonl(INADDR_ANY);
  
  if(bind(sockfd,(sockaddr*)&host,sizeof host) == -1)
    return -1;
  
  listen(sockfd,5);
  
  return sockfd;

}

void *login_handler(int sockfd)
{
  sockaddr_in clnt;
  char buf[64];
  socklen_t clnt_len = sizeof clnt;
  for(;;)
  {
    int lsfd = accept(sockfd,(sockaddr*)&clnt,&clnt_len);
    if(lsfd == -1)
    {
      cerr << "tcp accept error " << endl;
      continue;
    }
    memset(buf,0,64);
    if(myRead(lsfd,buf,4) == -1) { close(lsfd); continue;}
    int len = atoi(buf);
    memset(buf,0,64);
    if( len >= 64 || myRead(lsfd,buf,len) == -1) { close(lsfd); continue;}
    std::string one_name = buf;
    Talker v(one_name,clnt.sin_addr.s_addr);
    chatroom.addone(v);
    msg_rcv_que.add(Msg(Msg::NOTICE,"admin","all",one_name+" join the chatroom"));
  }
}

int make_udp_socket()
{
  sockaddr_in host;
  int sockfd = socket(PF_INET,SOCK_DGRAM,0);
  if( sockfd == -1) return -1;
  
  host.sin_family = AF_INET;
  host.sin_port = htons(PORT);
  host.sin_addr.s_addr = htonl(INADDR_ANY);
  if( bind(sockfd , (sockaddr*)&host, sizeof host) == -1)
    return -1;
  return sockfd;
}

void* rcv_msg_handler(int sockfd)
{
  sockaddr_in clnt;
  char buf[1024];
  socklen_t clnt_len = sizeof clnt;
  for(;;)
  {
    memset(buf,0,sizeof buf);
    int n = recvfrom(sockfd,buf,1024,0,(sockaddr*)&clnt,&clnt_len);
    if( n <=0 ) { cerr << "rcv_msg_handler error occur "<< "error :" << n << std::endl; continue;}
    Msg t = msgJsonParse(buf);
    msg_rcv_que.add(t);
  }  

}

void* mid_deal_handler()
{
  while(true)
  {
    while(msg_rcv_que.empty()) this_thread::yield();
    msg_snd_que.add(msg_rcv_que.pop());
  }

}

void* snd_msg_handler(int sockfd)
{
  char snd_buf[1024];
  for(;;)
  {
    while(msg_snd_que.empty()) { this_thread::yield();}
    memset(snd_buf,0,sizeof snd_buf);
    msgJsonPack(snd_buf,msg_snd_que.pop());
    for(auto it = chatroom.begin(); it != chatroom.end();++it)
    {
      sockaddr_in to_clnt;
      socklen_t to_clnt_len = sizeof to_clnt;
      memset(&to_clnt,0,sizeof to_clnt);
      to_clnt.sin_addr.s_addr = it->getAddr();
      to_clnt.sin_port = PORT;
      to_clnt.sin_family = AF_INET;
      sendto(sockfd,snd_buf,strlen(snd_buf),0,(sockaddr*)&to_clnt,to_clnt_len);
    }
  }

}
