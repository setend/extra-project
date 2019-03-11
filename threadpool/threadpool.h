#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <mutex>
#include <thread>
#include <condition_variable>
#include <stack>
#include <queue>
#include <list>
#include <vector>
#include <iostream>
#include <utility>

class ThreadPool;

class Task_base{
public:
  Task_base(int id):_id(id){}
  int getId() const { return _id;}
  virtual void taskproc() = 0;
private:
  int _id;
}; //Task_base

class MyThread{
  
public:
  explicit MyThread(int id,ThreadPool*);
  int getId() const { return _id;} // thread id
  void run();  //just run thread function
  void wakeup(); // wakeup thread
  void stop(); //stop thread
  bool next_task(); // ask threadpool if there is a task
  void set_task(Task_base *t); // set task pointer
  ~MyThread();
private:
  int _id;
  bool _hastask;
  bool _running;
  Task_base *_task;
  std::thread _t;
  ThreadPool *_pool;
  
  std::mutex mtx;
  std::condition_variable cv;
  
  
};//MyThread

class ThreadPool{
  
public:
  explicit ThreadPool(int n); //init 
  ThreadPool(const ThreadPool&) =delete;
  void add_task(Task_base*); //add Task
  bool assign_task(int i); // just used by MyThread
  void destroy_threadpool();
  
  
private:
  int _size;
  std::vector<MyThread*> mythreads;
  std::queue<Task_base*> task_queue;
  std::stack<int> idle_thread;
  std::list<int> working_thread;
  
  
private:
  std::mutex mtx;
  
}; //ThreadPool

#endif // THREADPOOL_H