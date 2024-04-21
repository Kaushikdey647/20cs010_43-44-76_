#ifndef SYNC_DATA_H
#define SYNC_DATA_H

#include <string>

struct SyncData {
    int process_id;
    int logical_clock;
    std::string data;

    // Constructor
    SyncData(int id, int clock, const std::string& d) : process_id(id), logical_clock(clock), data(d) {}
};

#endif // SYNC_DATA_H