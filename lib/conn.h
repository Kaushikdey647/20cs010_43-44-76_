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
    bool is_server;
    int port_number;
public:
    Connection(bool serv, int port);
    ~Connection();

    bool createSocket();
    bool bindSocket();
    bool listenForConnection();
    bool acceptConnection();
    bool connectToServer(const std::string& server_ip);
    bool sendData(int socket, const std:string& data);
    //TODO: Replace string by a struct jisko randi jaisa pass karenge
    std::string recieveData(int socket);
    void closeSocket(int socket);
};

#endif