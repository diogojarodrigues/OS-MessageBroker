#include "../header/logout.hpp"


void logout() {

    // Check if the command is valid
    if (current_uid.empty() || current_password.empty()) {
        cout << "You should log in first!" << endl;
        return;
    }

    // Send the message to the server
    string request = "LOU " + current_uid + " " + current_password + "\n";
    string response = send_udp_request(request);

    // Check the response
    if (response == "RLO OK\n") {
        cout << "successful logout" << endl;
    } else if (response == "RLO NOK\n") {
        cout << "user not logged in" << endl;
        // current_uid.clear();
        // current_password.clear();
    } else if (response == "RLO UNR\n") {
        cout << "user not registered" << endl;
    } else {
        cout << "logout: error" << endl;
    }

    // Clear the current uid and password
    current_uid.clear();
    current_password.clear();
};
