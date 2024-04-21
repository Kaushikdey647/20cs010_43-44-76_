#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include "sdat.h"

class Connection {
private:
    int socket_fd;
    struct sockaddr_in server_address;
    int port_number;

public:
    Connection(int port);
    ~Connection();

    bool createSocket();
    bool bindSocket();
    bool listenForConnection();
    int acceptConnection();
    bool connectToServer(const std::string& server_ip);
    bool sendData(int socket, const SyncData& data);
    SyncData receiveData(int socket);
    void closeSocket(int socket);
};

#endif // CONNECTION_H