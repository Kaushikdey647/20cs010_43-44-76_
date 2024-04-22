#include "lib/lmpt.h"
#include <iostream>

int main(int argc, char* argv[]) {
    int port = 0;
    int sys_id = 0;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if ((arg == "-p") && i + 1 < argc) {
            port = std::stoi(argv[++i]); // Convert string to int
        } else if ((arg == "-i") && i + 1 < argc) {
            sys_id = std::stoi(argv[++i]); // Convert string to int
        } else {
            std::cerr << "Usage: " << argv[0] << " -p <PORT NO> -i <SYS ID>\n";
            return 1;
        }
    }

    //REST CODE GOES HERE LOL

    return 0;
}