#include "./utils.hpp"
#include "./protocol.hpp"
#include "./commands.hpp"

void handle_udp_message() {

    string request = read_udp_message();

    if (
        request.length() > 21
        || request.length() < 3
        || request[request.size() - 1] != '\n'
    ) {
        if (DEBUG) cout << "handle_udp_message: wrong arguments\n";
        sendto(udp_socket, "ERR\n", 4, 0, (struct sockaddr*)&udp_addr, udp_addrlen);
        return;
    }

    string opcode = request.substr(0, 3);

    if (opcode == "LIN") {
        if (verbose || DEBUG) cout << "LOGIN REQUEST\n";
        login(request);
    } else if (opcode == "LOU") {
        if (verbose || DEBUG) cout << "LOGOUT REQUEST\n";
        logout(request);
    } else if (opcode == "UNR") {
        if (verbose || DEBUG) cout << "UNREGISTER REQUEST\n";
        logout(request, true);
    } else if (opcode == "LMA") {
        if (verbose || DEBUG) cout << "MY AUCTIONS REQUEST\n";
        my_auctions(request);
    } else if (opcode == "LMB") {
        if (verbose || DEBUG) cout << "MY BIDS REQUEST\n";
        my_bids(request);
    } else if (opcode == "LST") {
        if (verbose || DEBUG) cout << "LIST REQUEST\n";
        list(request);
    } else if (opcode == "SRC") {
        if (verbose || DEBUG) cout << "SHOW RECORD REQUEST\n";
        show_record(request);
    } else {
        if (verbose || DEBUG) cout << "INVALID REQUEST\n";
        cout << "invalid udp command\n";
    }

    cout << endl;
}

void handle_tcp_message() {
    
    // Create a new process with fork
    pid_t pid = fork();
    if (pid == -1) {
        if (DEBUG) cout << "handle_tcp_message: error forking\n";
        exit(-1);
    }
    if (pid != 0) return;   // Parent process

    // Create a new socket for the new client
    int sockett = connect_to_client();
    if (sockett == -1) {
        if (DEBUG) cout << "handle_tcp_message: error connecting to client\n";
        exit(-1);
    }

    string request = read_tcp_message(sockett);
    if (
        request.length() < 3
    ) {
        if (DEBUG) cout << "handle_tcp_message: wrong arguments\n";
        write_tcp_message(sockett, "ERR\n");
        return;
    }

    string opcode = request.substr(0, 3);
    if (opcode == "OPA") {
        if (verbose || DEBUG) cout << "OPEN AUCTION REQUEST\n";
        openn(sockett, request);
    } else if (opcode == "CLS") {
        if (verbose || DEBUG) cout << "CLOSE AUCTION REQUEST\n";
        closee(sockett, request);
    } else if (opcode == "SAS") {
        if (verbose || DEBUG) cout << "SHOW ASSET REQUEST\n";
        show_asset(sockett, request);
    } else if (opcode == "BID") {
        if (verbose || DEBUG) cout << "BID REQUEST\n";
        bid(sockett, request);
    } else {
        if (verbose || DEBUG) cout << "INVALID REQUEST\n";
        cout << "invalid tcp command\n";
    }

    close(sockett);
    exit(0);
}

int main(int argc, char** argv) {

    // Ignore SIGPIPE
    struct sigaction act;
    memset(&act,0,sizeof act);
    act.sa_handler=SIG_IGN;
    if(sigaction(SIGPIPE,&act,NULL)==-1) exit(1);

    // Delete fork chieldrens
    signal(SIGCHLD, SIG_IGN);

    // Create data directory if it doesn't exist
    string path = "src/server/data";
    if (!fs::exists(path)) fs::create_directory(path);
    if (!fs::exists(path + "/users")) fs::create_directory(path + "/users");
    if (!fs::exists(path + "/auctions")) fs::create_directory(path + "/auctions");

    if(argc==2){
        if(!strcmp(argv[1],"-v")){
            verbose = true;
        }
    }
    if(argc==3){
        if(!strcmp(argv[1],"-p")){
            port = (const char*) argv[2];
        }
    }if(argc==4){
        if(!strcmp(argv[1],"-v")){
            verbose = true;
        }
        if(!strcmp(argv[2],"-p")){
            port = (const char*)argv[3];
        }
        if(!strcmp(argv[1],"-p")){
            port = (const char*)argv[2];
        }
        if(!strcmp(argv[3],"-v")){
            verbose = true;
        }
    }

    if (initialize_udp_socket() == -1) {
        cerr << "main: error initializing udp socket\n";
        exit(-1);
    }

    if (initialize_tcp_socket() == -1) {
        cerr << "main: error initializing tcp socket\n";
        exit(-1);
    }

    //File descriptor set for select
    fd_set readfds;
    while (1) {

        FD_ZERO(&readfds);

        // Add UDP socket to set
        FD_SET(udp_socket, &readfds);

        // Add TCP socket to set
        FD_SET(tcp_socket, &readfds);

        // Use select to monitor sockets
        int max_sd = (udp_socket > tcp_socket) ? udp_socket : tcp_socket;
        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            cerr << "main: select error\n";
            exit(-1);
        }

        //If UDP socket is ready
        if (FD_ISSET(udp_socket, &readfds)) {
            handle_udp_message();
        }

        // If TCP socket is ready for a new connection
        if (FD_ISSET(tcp_socket, &readfds)) {
            handle_tcp_message();
        }
    }

    close_udp_socket();
    close_tcp_socket();

    return 0;
}
