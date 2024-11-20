#include "./protocol.hpp"
#include "./utils.hpp"

/* ################### GLOBAL VARIABLES ################### */

int udp_socket, tcp_socket;
struct addrinfo *udp_res, *tcp_res;
struct sockaddr_in udp_addr, tcp_addr;
socklen_t udp_addrlen, tcp_addrlen;
const char* port = PORT;
bool verbose=0;

/* ################### UDP ################### */

int initialize_udp_socket() {
    struct addrinfo hints;
    int aux;

    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket == -1) return -1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    aux = getaddrinfo(NULL, port, &hints, &udp_res);
    if (aux != 0) return -1;

    aux = ::bind(udp_socket, udp_res->ai_addr, udp_res->ai_addrlen);
    if (aux == -1) return -1;

    return 0;
}

string read_udp_message() {
    const int buffer_size = 21;                  //UDP messages are at most 20 bytes long
    char buffer[buffer_size] = "\0";

    udp_addrlen = sizeof(udp_addr);
    int aux = recvfrom(udp_socket, buffer, buffer_size, 0, (struct sockaddr*) &udp_addr, &udp_addrlen);
    if (aux == -1) return "";
    
    if (verbose || DEBUG) {
        cout << "BEGIN: received UDP request (" << aux << " bytes): " << buffer;
        cout << "From IP: " << udp_addr.sin_addr.s_addr << " and port: "<< udp_addr.sin_port << "\n" ;
    }    
    
    return buffer;
}

void write_udp_message(string message) {
    int aux = sendto(udp_socket, message.c_str(), message.length(), 0, (struct sockaddr*)&udp_addr, udp_addrlen);
    if (aux == -1) {
        if (DEBUG) cout << "write_udp_message: sendto error\n";
        exit(-1);
    }

    if (DEBUG || verbose) cout << "END: sent UDP response (" << message.size() << " bytes): " << message;
}

void close_udp_socket() {
    freeaddrinfo(udp_res);
    close(udp_socket);
}

/* ################### TCP ################### */

int initialize_tcp_socket() {
    struct addrinfo hints;
    int aux;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    aux = getaddrinfo(NULL, port, &hints, &tcp_res);
    if (aux != 0) return -1;

    tcp_socket = socket(tcp_res->ai_family, tcp_res->ai_socktype, tcp_res->ai_protocol);
    if (tcp_socket == -1) return -1;

    aux = ::bind(tcp_socket, tcp_res->ai_addr, tcp_res->ai_addrlen);
    if (aux == -1) return -1;

    // aux = listen(tcp_socket, 20);
    aux = listen(tcp_socket, 1);
    if (aux == -1) return -1;

    return 0;
}

int connect_to_client() {

    int sockett = accept(tcp_socket, (struct sockaddr*)&tcp_addr, &tcp_addrlen);
    if (sockett < 0) {
        cerr << "Error occurred: " << strerror(errno) << endl;
        return -1;
    }

    // Set timeout
    struct timeval timeout;
    timeout.tv_sec = 4; // 4 seconds timeout
    timeout.tv_usec = 0;

    if (setsockopt(sockett, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        cerr << "Error setting timeout: " << strerror(errno) << endl;
        close(sockett);
        return -1;
    }

    return sockett;
}


string read_tcp_message(int sockett) {
    string command = "";

    char buffer[BUFFER_SIZE] = "\0";
    int bytes_read = read(sockett, buffer, 1);
    if (bytes_read < 0) {
        cerr << "Error occurred: " << strerror(errno) << endl;
        exit(-1);
    }
    command += buffer[0];
    int k = 0;
    if(buffer[0] == 'O'){               //OPA COMMAND
        while(true){
            bytes_read = read(sockett, buffer, 1);
            if (bytes_read <0) {
                cerr << "Error occurred: " << strerror(errno) << endl;
                exit(-1);
            }
            if (buffer[0] == '\n') break;
            if(buffer[0] == ' ') k++;
            if(k==8) break;
            command += buffer[0];
        }
        command += '\n';

    }else{                              //OTHER COMMANDS
        bytes_read = read(sockett, buffer, sizeof(buffer));
        if (bytes_read < 0) {
            cerr << "Error occurred: " << strerror(errno) << endl;
            exit(-1);
        }
        command += buffer;
    }

    if(verbose || DEBUG) cout << "BEGIN: received TCP request (" << command.size() << " bytes): " << command << "From IP: " << tcp_addr.sin_addr.s_addr << " and port: "<< tcp_addr.sin_port << "\n" ;

    return command;
}

void write_tcp_message(int sockett, string message) {

    int aux = write(sockett, message.c_str(), message.size());
    if (aux == -1) {
        cerr << "Error occurred: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    if (DEBUG || verbose) cout << "END: sent TCP response (" << message.size() << " bytes): " << message << "\n";
}

void close_tcp_socket() {
    freeaddrinfo(tcp_res);
    close(tcp_socket);
}