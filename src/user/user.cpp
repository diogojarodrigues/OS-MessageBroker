#include "./utils.hpp"
#include "./header/login.hpp"
#include "./header/logout.hpp"
#include "./header/unregister.hpp"
#include "./header/exit.hpp"
#include "./header/open.hpp"
#include "./header/close.hpp"
#include "./header/myauctions.hpp"
#include "./header/mybids.hpp"
#include "./header/list.hpp"
#include "./header/show_asset.hpp"
#include "./header/bid.hpp"
#include "./header/show_record.hpp"
#include "./header/show_user.hpp"

void get_input(vector<string>* command) {
    string line;
    command->clear();
    
    // Read the entire line
    if (getline(cin, line).fail()) {
        exit(0);
        return;
    }

    istringstream iss(line);                // Creating a string stream from the line
    string word;

    // Extract words from the line using stringstream and store them in the vector
    while (iss >> word) {
        command->push_back(word);
    }
}

int main(int argc, char** argv) {

    if(argc==3) {
        if(!strcmp(argv[1],"-n")){
            server_ip = (const char*) argv[2];
        }
        else if(!strcmp(argv[1],"-p")){
            port = (const char*)argv[2];
        }
    } if(argc==5) {
        if(!strcmp(argv[1],"-n")){
            server_ip = (const char*)argv[2];
        }
        if(!strcmp(argv[3],"-p")){
            port = (const char*)argv[4];
        }
        if(!strcmp(argv[1],"-p")){
            port = (const char*)argv[2];
        }
        if(!strcmp(argv[3],"-n")){
            server_ip = (const char*)argv[4];
        }
    }
    
    server_info_udp = nullptr;
    if (get_server_info(&server_info_udp, true) == -1) {
        cout << "Error getting server info" << endl;
        return -1;
    }

    // Creating socket
    client_udp_socket = create_socket(true); 
    if (client_udp_socket == -1) {
        cout << "Error creating udp socket" << endl;
        return -1;
    }

    while (true) {
        
        if (!current_uid.empty() && !current_password.empty()) {
            cout << current_uid << " ";
        } else {
            cout << "anonymous ";
        }
        cout << ">> ";
        get_input(&command);

        if (command.empty()) {
            cout << "Invalid command" << endl;
            continue;
        }

        if (command[0]=="login") {
            login();
        } else if (command[0]=="logout"){
            logout();
        } else if (command[0]=="unregister"){
            unregister();
        } else if (command[0]=="exit"){
            exitt();
        } else if (command[0]=="open"){
            openn();
        } else if (command[0]=="close"){
            closee();
        } else if (command[0]=="myauctions" || command[0] == "ma"){
            myauctions();
        } else if (command[0]=="mybids" || command[0] == "mb"){
            mybids();
        } else if (command[0]=="list" || command[0] == "l"){
            list();
        } else if (command[0]=="show_asset" || command[0] == "sa"){
            show_asset();
        } else if (command[0]=="bid" || command[0] == "b"){
            bid();
        } else if (command[0]=="show_record" || command[0] == "sr"){
            show_record();
        } else if (command[0]=="show_user" || command[0] == "user"){
            show_current_user();
        } else {
            cout << "Invalid command" << endl;
        }

        command.clear();
        cout << endl;
    } 

    freeaddrinfo(server_info_udp);
    close(client_udp_socket);

    return 0;
}
