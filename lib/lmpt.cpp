#include "lmpt.h"
#include "conn.h"

Lamport::Lamport(int id, int sport, int rport){
    process_id = id;
    sender = Connection(false, sport);
    reciever = Connection(true, rport);
    logical_clock = std::atomic<int>(0);
}