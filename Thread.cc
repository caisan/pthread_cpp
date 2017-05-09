#include "Thread.h"
#include <errno.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <assert.h>
#include <signal.h>
Thread::~Thread()
{
}

Thread::Thread():thread_id(0),pid(0)
{
}

void *Thread::_entry_func(void *arg) {
    void *r = ((Thread*)arg)->entry_wrapper();
    return r;
}

void *Thread::entry_wrapper()
{
    return entry();
}
const pthread_t &Thread::get_thread_id()
{
    return thread_id;
}
bool Thread::is_started() const
{
    return thread_id != 0;
}
bool Thread::am_self()
{
    return (pthread_self() == thread_id);
}
int Thread::kill(int signal)
{
    if (thread_id)
        return pthread_kill(thread_id, signal);
    else
        return -EINVAL;
}
#define PAGE_MASK 100
int Thread::try_create(size_t stacksize)
{
    pthread_attr_t *thread_attr = NULL;
    pthread_attr_t thread_attr_loc;

    stacksize &= PAGE_MASK;

    if (stacksize) {
        thread_attr = &thread_attr_loc;
        pthread_attr_init(thread_attr);
        pthread_attr_setstacksize(thread_attr, stacksize);
    }
    int r;
    r = pthread_create(&thread_id, thread_attr, _entry_func, (void*)this);
    if (thread_attr) {
        pthread_attr_destroy(thread_attr); 
    }
    return r;
}
void Thread::create(size_t stacksize)
{
    int ret = try_create(stacksize);
    if (ret != 0) {
      char buf[256];
      snprintf(buf, sizeof(buf), "Thread::try_create(): pthread_create "
                                              "failed with error %d", ret);
      std::cout<<buf<<std::endl;
      assert(ret == 0);
   }
}
void Thread::create(const char* name, size_t stacksize)
{
    assert(strlen(name) < 16);
    thread_name = name;
    int ret = try_create(stacksize);
    if(ret!=0)
    {
        char buf[256];
        snprintf(buf, sizeof(buf), "Thread::try_create(): pthread_create failed wi  th %d", ret);
        assert(ret==0);
    }
}
int Thread::join(void **prval)
{
    if (thread_id == 0) {
        assert("join on thread that was never started" == 0);
        return -EINVAL;
    }
    int status = pthread_join(thread_id, prval);
    assert(status == 0);
    thread_id = 0;
    return status;
}
int Thread::detach()
{
    return pthread_detach(thread_id);
}


