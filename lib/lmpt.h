#ifndef LAMPORT_H
#define LAMPORT_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <map>
#include <string>
#include <queue>
#include <set>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

enum Signal {
    REQUEST, //SIGNALS MEANT TO REQUEST THE CRITICAL SECTION
    REPLY, //SIGNALS MEANT TO REPLY TO A REQUEST
    RELEASE, //SIGNALS MEANT TO RELEASE THE CRITICAL SECTION
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
    std::priority_queue<std::pair<int, int>> requestQueue; // Queue for storing requests
    std::set<int> replyMap; // Map for storing reply status
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

    // SEND REQUEST SIGNAL
    void request();

    // MAINTAIN THE REQUEST QUEUE, ISKO BHI THREAD PE CHADHAYENGE
    void handleQueue();

    // PRINT CONFIG
    void printConfig();
};

#endif