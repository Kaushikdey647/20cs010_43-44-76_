# Lamport's Mutual Exclusion Algorithm

## Team Members

- Kaushik Dey (20CS01043)
- Akshat Anand (20CS01044)
- Giriraj Singh (20CS01076)

## Description

This project implements Lamport's Mutual Exclusion Algorithm, a distributed algorithm for achieving mutual exclusion in a distributed system. It ensures that only one process can access a shared resource (critical section) at any given time, preventing race conditions and ensuring data consistency.

![IMG](/img/ssc_1.png)

## Project Structure

```
.
├── Makefile
├── README.md
├── lib
│   ├── lmpt.cpp
│   └── lmpt.h
├── main
└── main.cpp
```

- `Makefile`: Contains rules for building the project.
- `README.md`: This file, providing an overview of the project.
- `lib/lmpt.cpp`: Implementation of the Lamport's Mutual Exclusion Algorithm.
- `lib/lmpt.h`: Header file containing the class declaration and necessary data structures.
- `main.cpp`: Entry point of the application, handles command-line arguments and initializes the algorithm.

## Building the Project

To build the project, navigate to the project directory and run the following command:

```
make
```

This will compile the source files and generate the executable `main`.

## Running the Project

After building the project, you can run the executable with the following command:

```
./main -p <PORT_NO> -i <SYSTEM_ID> -f <CONFIG_FILE>
```

Replace `<PORT_NO>` with the desired port number for the process to listen on, `<SYSTEM_ID>` with a unique identifier for the process and `<CONFIG_FILE>` with the path to the configuration file containing information about other nodes in the distributed system. It should look something like

```
8080 1
9090 2

```
and so on

The program will start listening for incoming messages on the specified port and will wait for user input.
During execution, the program will prompt you to enter additional commands:

- `REQUEST`: Send a request for accessing the critical section.
- `STATUS`: Add a new node to the distributed system.
- `EXIT`: Terminate the program.

## Lamport Class

The `Lamport` class is a C++ implementation of Lamport's distributed mutual exclusion algorithm. It provides a way to ensure that only one node is in its critical section at a time in a distributed system.

### Class Members

- `int logicalClock`: An atomic variable representing the logical clock of the node.
- `int listenPort`: The port number on which the node listens for incoming messages.
- `std::mutex clockMutex`: A mutex used to protect updates to the logical clock.
- `std::map<int, struct sockaddr_in> nodeList`: A map storing information about other nodes in the system. The key is the node ID and the value is the node's address.
- `std::priority_queue<std::pair<int, int>> requestQueue`: A priority queue storing requests from other nodes. Each request is a pair of integers representing the timestamp and the node ID.
- `std::set<int> replyMap`: A set storing the reply status of other nodes.

### Constructor and Destructor

- `Lamport(int id, int lport)`: Constructor that initializes a new `Lamport` object with a given node ID and listening port.
- `~Lamport()`: Destructor that cleans up the `Lamport` object when it is no longer needed.

### Member Functions

- `void addNode(int id, std::string ip, int port)`: Adds a new node to the `nodeList` with the given ID, IP address, and port number.
- `int unicast(Signal sig, int sysId)`: Sends a signal to the node with the given system ID. The signal is sent via unicast.

This class provides the basic functionalities needed to implement Lamport's distributed mutual exclusion algorithm in a distributed system.

## Algorithm Overview

Lamport's Mutual Exclusion Algorithm is a distributed algorithm that ensures mutual exclusion in a distributed system. It works by assigning logical timestamps to events (requests for critical section access) and using these timestamps to determine the order in which processes can enter the critical section.

The algorithm follows these steps:

1. Each process maintains a logical clock that is incremented whenever an event occurs.
2. When a process wants to enter the critical section, it broadcasts a request message containing its current logical clock value to all other processes.
3. Upon receiving a request, each process updates its logical clock to be the maximum of its current clock value and the received request's timestamp, plus one.
4. Each process then sends a reply message to the requesting process, acknowledging the request.
5. The requesting process enters the critical section once it has received reply messages from all other processes.
6. After exiting the critical section, the process broadcasts a release message to all other processes, allowing them to proceed with their requests, if any.

By following this protocol, the algorithm guarantees that only one process can be in the critical section at any given time, ensuring mutual exclusion and preventing race conditions.
