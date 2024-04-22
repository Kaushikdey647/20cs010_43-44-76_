#include "lmpt.h"

Lamport::Lamport(int id, int lport){
    logicalClock = 0;
    processId = id;
    listenPort = lport;
}

void Lamport::addNode(int id, std::string ip, int port){
    struct sockaddr_in node;
    node.sin_family = AF_INET;
    node.sin_port = htons(port);
    node.sin_addr.s_addr = inet_addr(ip.c_str());
    nodeList[id] = node;
}

int Lamport::unicast(SyncData data, int sysId){
    // Fetch sockAddr from nodeList
    struct sockaddr_in node = nodeList[sysId];

    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("Socket creation failed");
        return -1;
    }

    // Connect to the node
    if(connect(sock, (struct sockaddr*)&node, sizeof(node)) < 0){
        perror("Connection failed");
        return -1;
    }

    // Send the data
    if(send(sock, &data, sizeof(data), 0) < 0){
        perror("Send failed");
        return -1;
    }

    // Close the socket
    close(sock);

    return 0;
}

int Lamport::broadcast(SyncData data){
    for(auto it = nodeList.begin(); it != nodeList.end(); it++){
        unicast(data, it->first);
    }
    return 0;
}

void Lamport::receive(){
    // Setup server on listenport
    int servsock = socket(AF_INET, SOCK_STREAM, 0);
    if(servsock < 0){
        perror("Socket creation failed");
        return;
    }

    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(listenPort);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(servsock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0){
        perror("Bind failed");
        return;
    }

    SyncData data;

    while(true){
        // Listen for incoming connections
        listen(servsock, 5);

        // Accept the connection
        int sock = accept(servsock, NULL, NULL);
        if(sock < 0){
            perror("Accept failed");
            return;
        }

        // Receive the data
        if(recv(sock, &data, sizeof(data), 0) < 0){
            perror("Receive failed");
            return;
        }

        // Close the socket
        close(sock);

        // Handle the received data
        handleData(data);
    }
}

void Lamport::handleData(SyncData data){
    // Lock the clock
    std::unique_lock<std::mutex> lock(clockMutex);

    // Update the clock
    logicalClock = std::max(logicalClock, data.timestamp) + 1;

    // Unlock the clock
    lock.unlock();

    // Handle the data
    switch(data.msgType){
        case REQUEST:
            // Add the request to the queue
            requestQueue.push({data.timestamp, data.senderId});
            break;
        case REPLY:
            // Do something
            break;
        case RELEASE:
            // Do something
            break;
        case SYNC:
            // Do something
            break;
    }
}