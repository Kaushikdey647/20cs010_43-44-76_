#ifndef LAMPORT_H
#define LAMPORT_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <map>
#include <string>
#include <queue>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

enum Signal {
    REQUEST, //SIGNALS MEANT TO REQUEST THE CRITICAL SECTION
    REPLY, //SIGNALS MEANT TO REPLY TO A REQUEST
    RELEASE, //SIGNALS MEANT TO RELEASE THE CRITICAL SECTION
    SYNC //SIGNALS MEANT TO SYNC THE CLOCKS
};

class SyncData {
public:
    int timestamp;
    int senderId;
    Signal msgType;
};

class Lamport {
private:
    int processId; // Unique ID for each process
    int logicalClock; // Atomic variable for logical clock
    int listenPort; // Port for listening to incoming messages
    std::mutex clockMutex; // Mutex for protecting logical clock updates
    std::map<int, struct sockaddr_in> nodeList; // Map for storing node information
    std::queue<std::pair<int, int>> requestQueue; // Queue for storing requests
    std::thread listener; // Thread for listening to incoming messages
    std::thread queueHandler; // Thread for handling request queue

public:
    Lamport(int id, int lport);
    ~Lamport();

    // BASE FUNCTIONALITIES

    // ADD SYSTEM TO THE NODE LIST
    void addNode(int id, std::string ip, int port);

    // FIND THE PORT AND SEND THE DATA
    int unicast(Signal sig, int sysId);

    // SEND DATA TO ALL NODES
    int broadcast(Signal sig);

    // RECIEVE DATA FROM THE PORT, ISKO THREAD PE CHADHAYENGE
    void receive();

    // HANDLE RECIEVED DATA
    void handleData(SyncData data);
};

#endif