#include "protocol.hpp"

const char* server_ip = SERVER_IP;
const char* port = PORT;
struct addrinfo* server_info_udp;
int client_udp_socket;

/* ####################### UDP MESSAGE #######################  */

int create_socket(bool udp) {

    int sockett;
    if (udp) {
        sockett = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
    } else {
        sockett = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
    }

    if (sockett == -1) {
        cerr << "Error creating socket" << endl;
        return -1;
    }

    // Set a timeout for socket operations (e.g., 5 seconds)
    struct timeval timeout;
    timeout.tv_sec = 5; // 5 seconds timeout
    timeout.tv_usec = 0;

    if (setsockopt(sockett, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        cerr << "Error setting timeout: " << strerror(errno) << endl;
        close(sockett);
        return -1;
    }

    return sockett;
}

int get_server_info(struct addrinfo** res, bool udp) {
    struct addrinfo hints;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;          // IPv4
    udp ? hints.ai_socktype = SOCK_DGRAM : hints.ai_socktype = SOCK_STREAM;

    int errcode = getaddrinfo(server_ip, port, &hints, res);
    if (errcode != 0) {
        cerr << "Error getting address info: " << gai_strerror(errcode) << endl;
        return -1;
    }

    return 0;
}

// ####################### UDP REQUEST #######################  //

int send_udp_message(int socket, struct addrinfo* info, string message) {
    int aux = sendto(socket, message.c_str(), message.size(), 0, info->ai_addr, info->ai_addrlen);
    if (aux == -1) {
        cerr << "Send failed: " << strerror(errno) << endl;
        return -1;
    }

    return 0;
}

int receive_udp_message(int socket, struct addrinfo* info, char* buffer) {
    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof(addr);

    int aux = recvfrom(socket, buffer, 8192, 0, (struct sockaddr*) &addr, &addrlen);
    if (aux == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            cerr << "Timeout: Server did not respond within the specified time." << endl;
        } else {
            cerr << "Receive failed: " << strerror(errno) << endl;
        }
        return -1;
    }

    return aux;
}

string send_udp_request(string message) {

    // Sending request
    if (send_udp_message(client_udp_socket, server_info_udp, message) == -1) {
        return "ERR\n";
    }

    // Receiving response
    char buffer[8192] = "";
    if (receive_udp_message(client_udp_socket, server_info_udp, buffer) == -1) {
        return "ERR\n";
    }

    return buffer;
}

/* ####################### TCP MESSAGE #######################  */

int connectToServer(int socket, struct addrinfo* info) {
    int aux = connect(socket, info->ai_addr, info->ai_addrlen);
    if (aux == -1) {
        cerr << "Connection failed: " << strerror(errno) << endl;
        close(socket);
        return -1;
    }

    return 0;
}

int write_tcp_message(int socket, string message) {
    int aux = write(socket, message.c_str(), message.size());
    if (aux == -1) {
        cerr << "Send failed: " << strerror(errno) << endl;
        close(socket);;
        return -1;
    }

    return 0;
}

int read_tcp_message(int socket, char* buffer, int size) {
    
    int aux = read(socket, buffer, size);
    if (aux == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            cerr << "Timeout: Server did not respond within the specified time." << endl;
        } else {
            cerr << "Receive failed: " << strerror(errno) << endl;
        }
        close(socket);;
        return -1;
    }

    return aux;
}

void receive_tcp_image(int client_tcp_socket){

    ssize_t aux;
    char buffer[2048];
    int i=0;
    int k=0;
    string response, fname, fsize;
    while (1){
        if (read_tcp_message(client_tcp_socket, buffer, 1) == -1) {
            cerr << "Error reading image from server" << endl;
            exit(1);
        }

        if (i == 0 && buffer[i]=='E') {
            cerr << "something went wrong" << endl;
            return;
        }

        if (buffer[i]==' ' || buffer[i]=='\n'){
            k++;

            
            if(k==2 && response == "RSA NOK" ){
                cout << "asset does not exist" << endl;
                return;
            }
            if(k==1){
                response+=' ';
            }
            if(k>3){
                break;
            }
            continue;
        }
        if(k<2){
            response+= buffer[i];
        }
        if(k==2){
            fname+=buffer[i];
        }
        if(k==3){
            fsize+=buffer[i];
        }
        
    }

    string path="src/user/images/" + fname;

    FILE *file;
    file = fopen(path.c_str(), "w");

    ssize_t file_size=stoi(fsize);
    while (1)
    {
        aux=read(client_tcp_socket,buffer,2048);
        if(aux==-1) exit(1); /*error*/
        if(aux==0)
            break;
        if(file_size==aux-1){
            aux=fwrite(buffer,1,aux-1,file);
            file_size-=aux;
            if(file_size==0)
                break;
            else{
                fclose(file);
                cout << "erro" <<endl;
                return;
            }
        }                 
        aux=fwrite(buffer,1,aux,file);
        file_size-=aux;
    }
    fclose(file);
    
    cout << "asset file was saved in src/user/images/" << fname << " with " << fsize << " bytes"<<endl;

}

void send_tcp_image(int client_tcp_socket, string path) {

    ifstream file(path, ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening image file" << endl;
        exit(1);
    }

    const int file_data_size = 2048;
    char file_data[file_data_size] = "";

    int bytes_read = 0;
    int aux = 0;

    while (file.read(file_data, sizeof(file_data))) {
        bytes_read = file.gcount();

        aux=write(client_tcp_socket, file_data, bytes_read);
        if(aux==-1) {
            cerr << "Error sending image to server" << endl;
            file.close();
            exit(1);
        }                        /*error*/

        memset(file_data, 0, sizeof(file_data));       // Clear the buffer
    }
    
    bytes_read = file.gcount();
    aux=write(client_tcp_socket, file_data, bytes_read);
    if(aux==-1) {
        cerr << "Error sending image to server" << endl;
        file.close();
        exit(1);
    }

    aux=write(client_tcp_socket, "\n", 1);
    if(aux==-1) {
        cerr << "Error sending image to server" << endl;
        file.close();
        exit(1);
    }

    file.close();
}

string send_tcp_request(string message, type type, string path) {

    // Creating socket
    int client_tcp_socket = create_socket(false);
    if (client_tcp_socket == -1) {
        return "ERR\n";
    }

    // Getting server info
    struct addrinfo* server_info_tcp;
    if (get_server_info(&server_info_tcp, false) == -1) {
        return "ERR\n";
    }

    // Connecting to server
    if (connectToServer(client_tcp_socket, server_info_tcp) == -1) {
        return "ERR\n";
    }

    //Sending message
    if (write_tcp_message(client_tcp_socket, message) == -1) {
        return "ERR\n";
    }

    // Show Asset Command
    if (type == RECEIVE_TCP_IMAGE) {
        receive_tcp_image(client_tcp_socket);
        return " ";
    }

    // Open Asset Command
    if (type == SEND_TCP_IMAGE) {
        send_tcp_image(client_tcp_socket, path);
    }

    //Receiving message
    char buffer[8192] = "";
    if (read_tcp_message(client_tcp_socket, buffer, 8192) == -1) {
        return "ERR\n";
    }

    close(client_tcp_socket);

    return buffer;
}
