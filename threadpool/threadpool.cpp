#include "threadpool.h"

MyThread::MyThread(int id, ThreadPool* p)
{
  _id=id;
  _pool=p;
  _task = NULL;
  _hastask = false;
  _running = true;
  _t = std::thread(&MyThread::run,this);

}

void MyThread::run()
{
  std::unique_lock<std::mutex> lck(mtx);
  while(_running)
  {
	cv.wait(lck,[this](){return _hastask;});
	_task->taskproc();
	std::string temp_str= std::string("task : " )+std::to_string(_task->getId()) + "has finished\n";
	std::cout << temp_str;
	delete _task;
	_task = NULL;
	_hastask = next_task();
  }
}

void MyThread::stop()
{
  _running = false;
  _t.detach();

}

void MyThread::wakeup()
{
  _hastask=true;
  cv.notify_one();
}

bool MyThread::next_task()
{
  if(_pool->assign_task(_id)) return true;
  return false;

}

void MyThread::set_task(Task_base* t)
{
  _task = t;
}


MyThread::~MyThread()
{
  if(_t.joinable()) _t.detach();
  if(_task) delete _task;

}



ThreadPool::ThreadPool(int n)
{
  _size=n;
  for(int i=0;i<n;++i){
	mythreads.push_back(new MyThread(i,this));
	idle_thread.push(i);
  }
}

void ThreadPool::add_task(Task_base* t)
{
  mtx.lock();
  if(idle_thread.empty())
  {
	task_queue.push(t);
	mtx.unlock();
	return;
  }
  int th = idle_thread.top();
  idle_thread.pop();
  working_thread.push_back(th);
  mythreads[th]->set_task(t);
  mythreads[th]->wakeup();
  mtx.unlock();

}

bool ThreadPool::assign_task(int i)
{
  mtx.lock();
  if(task_queue.empty()) {
	working_thread.remove(i);
	idle_thread.push(i);
	mtx.unlock();
	return false;
  }
  mythreads[i]->set_task(task_queue.front());
  task_queue.pop();
  mtx.unlock();
  return true;
}

void ThreadPool::destroy_threadpool()
{
  mtx.lock();
  for(int i=0; i < _size; ++i)
  {
	mythreads[i]->stop();
	delete mythreads[i];
  }
}


