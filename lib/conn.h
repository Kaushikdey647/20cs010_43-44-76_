#ifndef CONN_H
#define CONN_H

class Connection {
    public:
        Connection();
        ~Connection();
        int connect();
        int disconnect();
        int send();
        int receive();
};

#endif