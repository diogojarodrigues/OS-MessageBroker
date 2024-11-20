#ifndef PROTOCOL_SERVER_HPP
#define PROTOCOL_SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <iostream>    
#include <vector> 
#include <algorithm>
#include <mutex>

#define PORT "58097"
#define HOST "192.168.1.71"

using namespace std;

extern const char* port;
extern bool verbose;

// UDP
extern int udp_socket;
extern socklen_t udp_addrlen;
extern struct addrinfo* udp_res;
extern struct sockaddr_in udp_addr;

int initialize_udp_socket();
string read_udp_message();
void write_udp_message(string message);
void close_udp_socket();


// TCP
extern int tcp_socket, sockett;
extern socklen_t tcp_addrlen;
extern struct addrinfo* tcp_res;
extern struct sockaddr_in tcp_addr;

int initialize_tcp_socket();
int connect_to_client();
string read_tcp_message(int sockett);
void write_tcp_message(int sockett, string message);
void close_tcp_socket();

#endif // PROTOCOL_SERVER_HPP