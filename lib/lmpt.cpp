#include "lmpt.h"
#include "conn.h"

Lamport::Lamport(){
    clock = 0;
}

Lamport::~Lamport(){
}

void Lamport::increment(){
    clock++;
}

void Lamport::decrement(){
    clock--;
}

void Lamport::setClock(Connection conn){
}

int Lamport::getClock(Connection conn){
}