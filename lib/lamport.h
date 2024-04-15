#ifndef LAMPORT_H
#define LAMPORT_H

#include "conn.h"
//LAMPORT CLASS DECLARATION GOES HERE

class Lamport {
    public:
        Lamport();
        ~Lamport();
        void increment();
        void decrement();
        void setClock(Connection);
        int getClock(Connection);
    private:
        int clock;
};

#endif