#include <iostream>
#include "Thread.h"
using namespace std;

class Worker : public Thread {
public:
    Worker() {}
    void *entry();
};

void *Worker::entry()
{
    /*Write your work here*/
    std::cout<<"Start work"<<std::endl;
    return NULL;
}




int main()
{
    Worker *worker = new Worker();
    worker->create("test worker");

    return 0;
}
