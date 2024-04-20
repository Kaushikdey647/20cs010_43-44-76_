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
    Connection* sender;
    Connection* reciever;
    std::mutex clock_mutex; // Mutex for protecting logical clock updates

public:
    Lamport(int id, int sport, int rport);
    ~Lamport();

    void senderThread();
    void receiverThread();

    void requestCriticalSection();
    void releaseCriticalSection();
};

#endif // LAMPORT_H