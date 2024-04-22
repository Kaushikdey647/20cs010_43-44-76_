#include "lib/lmpt.h"
#include <iostream>

void handleInput(Lamport* lamport){
    std::cout << "Write REQUEST to REQUEST for some bitches\n>";
    std::string input;
    while(true){
        std::cin >> input;
        if(input == "REQUEST"){
            lamport->broadcast(Signal::REQUEST);
        }
        if(input == "EXIT"){
            exit(0);
        }
        if(input == "ADD"){
            //addNode
            int id;
            std::string ip;
            int port;
            std::cout << "Enter the process_id of the node\n>";
            std::cin >> id;
            std::cout << "Enter the ip of the node\n>";
            std::cin >> ip;
            std::cout << "Enter the port of the node\n>";
            std::cin >> port;
            lamport->addNode(id, ip, port);
        }
    }
}

int main(int argc, char* argv[]) {
    int port = 0;
    int sys_id = 0;

    //input check for port and sys_id
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " -p <PORT NO> -i <SYS ID>\n";
        return 1;
    }

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
    
    Lamport* lamport = new Lamport(sys_id, port);

    // Initialize the listener thread
    std::thread listenerThread(&Lamport::receive, lamport);
    // Initialize the queue handler thread
    std::thread queueHandlerThread(&Lamport::handleQueue, lamport);
    // Initialize the input handler thread
    std::thread inputHandlerThread(handleInput, lamport);

    // Wait for the listener thread to finish
    listenerThread.join();
    // Wait for the queue handler thread to finish
    queueHandlerThread.join();
    // Wait for the input handler thread to finish
    inputHandlerThread.join();

    return 0;
}