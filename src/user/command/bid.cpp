#include "../header/bid.hpp"

void bid() {
    if(current_uid.empty() or current_password.empty()) {
        cout << "You should log in first!\n";
        return;
    }
    if(command.size() != 3) {
        cout << "Invalid command\n";
        return;
    }
    if(!isNumeric(command[1])or command[1].size()!=3) {
        cout << "Invalid AID\n";
        return;
    }
    if(!isNumeric(command[2]) or command[2].size()>6) {
        cout << "Invalid amount\n";
        return;
    }
    string request= "BID " + current_uid + " " + current_password + " " + command[1] + " " + command[2] + "\n";
    string response = send_tcp_request(request);
    if(response=="RBD ACC\n") {
        cout << "Bid placed\n";
    } else if(response=="RBD NOK\n") {
        cout << "Bid not placed, auction not active\n";
    } else if(response=="RBD NLG\n") {
        cout << "Bid not placed, user is not logged in\n";
        // current_uid.clear();
        // current_password.clear();
    } else if(response=="RBD REF\n") {
        cout << "Bid not placed, auction has a larger bid\n";
    } else if(response=="RBD ILG\n") {
        cout << "Bid not placed, auction is hosted by the user\n";
    } else {
        cout << "Bid not placed, unknown error\n";
    }
};
