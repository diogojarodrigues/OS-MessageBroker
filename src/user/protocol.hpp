#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

#define PORT "58097"
#define SERVER_IP "localhost"
//#define PORT "58011"
//#define SERVER_IP "tejo.tecnico.ulisboa.pt"

using namespace std;

enum type {
    RECEIVE_TCP_IMAGE,
    SEND_TCP_IMAGE,
    DEFAULT
};

extern const char* port;
extern const char* server_ip;
extern struct addrinfo* server_info_udp;
extern int client_udp_socket;

int create_socket(bool udp);
int get_server_info(struct addrinfo** server_info_udp, bool udp);

string send_udp_request(string message);
string send_tcp_request(string message, type type=DEFAULT, string path="");

#endif // PROTOCOL_HPP
