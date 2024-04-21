#include "conn.h"

Connection::Connection(int port) : port_number(port) {
    // Initialize socket file descriptor
    socket_fd = 0;

    // Initialize server address structure
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);
}