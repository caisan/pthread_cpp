#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <sys/types.h>

class Thread {
private:
    pthread_t thread_id;
    const char* thread_name;
    pid_t pid;
    
    void *entry_wrapper();
public:
    Thread(const Thread& other);
    const Thread& operator=(const Thread& other);


    Thread();
    virtual ~Thread();
protected:
    virtual void *entry() = 0;
private:
    static void *_entry_func(void *arg);
public:
    const pthread_t &get_thread_id();
    pid_t get_pid() const { return pid; }
    bool is_started() const;
    bool am_self();
    int kill(int signal);
    int try_create(size_t stacksize);
    void create(size_t stacksize = 0);
    void create(const char* name, size_t stacksize = 0);
    int join(void **prval = 0);
    int detach();
};
#endif
