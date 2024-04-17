#ifndef LAMPORT_H
#define LAMPORT_H

#include "conn.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class Lamport {
private:
    int process_id; // Unique ID for each process
    std::atomic<int> logical_clock; // Atomic variable for logical clock
    Connection* connection; // Pointer to Connection object
    std::mutex clock_mutex; // Mutex for protecting logical clock updates

public:
    Lamport(int id, int port);
    ~Lamport();

    void senderThread();
    void receiverThread();

    void requestCriticalSection();
    void releaseCriticalSection();
};

#endif // LAMPORT_H