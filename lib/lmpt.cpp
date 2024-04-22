#include "lmpt.h"

Lamport::Lamport(int id, int lport){
    logicalClock = 0;
    processId = id;
    listenPort = lport;
}

Lamport::~Lamport(){

}

void Lamport::addNode(int id, std::string ip, int port){
    struct sockaddr_in node;
    node.sin_family = AF_INET;
    node.sin_port = htons(port);
    node.sin_addr.s_addr = inet_addr(ip.c_str());
    nodeList[id] = node;
}

int Lamport::unicast(Signal sig, int sysId){
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

    SyncData data;

    // Lock the clock
    std::unique_lock<std::mutex> lock(clockMutex);

    // Get the clock
    data.timestamp = logicalClock;

    // Set the senderId
    data.senderId = processId;

    // Set the message type
    data.msgType = sig;

    // Unlock the clock
    lock.unlock();

    // Send the data
    if(send(sock, &data, sizeof(data), 0) < 0){
        perror("Send failed");
        return -1;
    }

    // Close the socket
    close(sock);

    return 0;
}

int Lamport::broadcast(Signal sig){
    for(auto it = nodeList.begin(); it != nodeList.end(); it++){
        if(it->first != processId){
            unicast(sig, it->first);
        }
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

    switch(data.msgType){
        case REQUEST:
            std::cout << logicalClock << ": Request received from " << data.senderId << std::endl;
            // Add the request to the queue
            requestQueue.push({data.timestamp, data.senderId});

            // If top of requestQueue is not this process, reply
            if(requestQueue.top().second != processId){
                unicast(REPLY, data.senderId);
            }

            break;

        case REPLY:
            // Add reply to the replyMap
            std::cout << logicalClock << ": Reply received from " << data.senderId << std::endl;
            replyMap.insert(data.senderId);
            break;

        case RELEASE:
            // Do something
            std::cout << logicalClock << ": Release received from " << data.senderId << std::endl;
            if(data.senderId == requestQueue.top().second)
                requestQueue.pop();
            else{
                perror("Invalid release");
                exit(1);
            }
            break;
    }
}

void Lamport::handleQueue(){
    while(true){
        if(!requestQueue.empty()){

            // If the top request is from this process
            if(requestQueue.top().second == processId){

                // Check if all replies are received
                if(replyMap.size() == nodeList.size() - 1){

                    //increment clock
                    std::unique_lock<std::mutex> lock(clockMutex);
                    logicalClock++;
                    lock.unlock();

                    std::cout << logicalClock << ": Entering Critial Section" << std::endl;
                    // Enter the critical section
                    std::this_thread::sleep_for(std::chrono::seconds(10));

                    // Broadcast release signal
                    broadcast(RELEASE);

                    // Clear the replyMap
                    replyMap.clear();

                    //pop from the pqueue
                    requestQueue.pop();

                    std::cout << logicalClock << ": Exiting Critial Section" << std::endl;
                }
            }
        }
    }
}

void Lamport::printConfig(){


    std::cout << "ID: " << processId;
    std::cout << " Port: " << listenPort;
    std::cout << " Clock: " << logicalClock << std::endl;

    std::cout << "Node List: " << std::endl;

    for(auto it = nodeList.begin(); it != nodeList.end(); it++){
        std::cout << "ID: " << it->first << " PORT: " << ntohs(it->second.sin_port) << " HOST: " << inet_ntoa(it->second.sin_addr) << std::endl;
    }

    std::cout << "Request Queue: " << std::endl;

    //print the request queue
    std::priority_queue<std::pair<int, int>,std::vector<std::pair<int,int>>,std::greater<std::pair<int,int>>> temp = requestQueue;
    while(!temp.empty()){
        std::cout << "ID: " << temp.top().second << " TIMESTAMP: " << temp.top().first << std::endl;
        temp.pop();
    }

    std::cout << std::endl;

    std::cout << "Reply Map: " << std::endl;

    //Print reply map
    for(auto it = replyMap.begin(); it != replyMap.end(); it++){
        std::cout << "ID: " << *it << std::endl;
    }

    std::cout << std::endl;
}

void Lamport::request(){
    // Increment clock

    std::unique_lock<std::mutex> lock(clockMutex);
    logicalClock++;
    lock.unlock();

    broadcast(REQUEST);
    requestQueue.push({logicalClock, processId});
}