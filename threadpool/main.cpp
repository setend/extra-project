#include <iostream>
#include "threadpool.h"

using namespace std;

class MyTask:public Task_base{
  public:
	MyTask(int i):Task_base(i){}
	void taskproc() { for(int i=0;i<10000;++i);}
};
  

int main(int argc, char **argv) {
    ThreadPool pool(5);
	for(int i = 0; i < 100; ++i)
	  pool.add_task(new MyTask(i));
	char ch;
	cin >> ch;
	pool.destroy_threadpool();
	return 0;
}
