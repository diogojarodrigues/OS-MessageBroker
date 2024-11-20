#include "../header/unregister.hpp"

void unregister() {

    // Check if the command is valid
    if (current_uid.empty() || current_password.empty()) {
        cout << "You should log in first!" << endl;
        return;
    }

    // Send the message to the server
    string request = "UNR " + current_uid + " " + current_password + "\n";
    string response = send_udp_request(request);

    // Check the response
    if (response == "RUR OK\n") {
        cout << "successful unregister" << endl;
    } else if (response == "RUR NOK\n") {
        cout << "user not logged in" << endl;
        // current_uid.clear();
        // current_password.clear();
    } else if (response == "RUR UNR\n") {
        cout << "user not registered" << endl;
    } else {
        cout << "unregister: error" << endl;
    }

    // Clear the current uid and password
    current_uid.clear();
    current_password.clear();
};