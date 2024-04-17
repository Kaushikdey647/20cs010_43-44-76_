#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

class Connection {
private:
    int socket_fd;
    struct sockaddr_in server_addr;
    int port_number;
public:
    Connection();
    ~Connection();

    bool createSocket();
    bool bindSocket();
    bool listenForConnection();
    bool acceptConnection();
    bool connectToServer(const std::string& server_ip);
    bool sendData(int socket, const std:string& data);
    std::string recieveData(int socket);
    void closeSocket(int socket);
};

#endif