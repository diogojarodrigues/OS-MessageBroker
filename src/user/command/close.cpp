#include "../header/close.hpp"



void closee() {
    if (current_uid.empty() || current_password.empty()) {
        cout << "You should log in first!" << endl;
        return;
    }
    
    if (command.size()!=2) {
        cout << "close: format not valid!" << endl;
        return;
    }

    string aid;
    aid = command[1];
    if (aid.length()!= 3 || !isNumeric(aid)) {
        cout << "close: aid must be numeric and have 3 digits" << endl;
        return;
    };

    string request = "CLS " + current_uid + " " + current_password + " "+ aid +"\n";
    string response = send_tcp_request(request, DEFAULT, nullptr);

    if (response == "RCL OK\n") {
        cout << "auction was closed" << endl;
    } else if (response == "RCL NLG\n") {
        cout << "user is not logged" << endl;
        // current_uid.clear();
        // current_password.clear();
    } else if (response == "RCL EAU\n") {
        cout << "auction does not exist" << endl;
    } else if (response == "RCL EOW\n") {
        cout << "auction not owned by the user" << endl;
    } else if (response == "RCL END\n") {
        cout << "auction has already finished" << endl;
    } else {
        cout << "close: error" << endl;
    }
    
}
