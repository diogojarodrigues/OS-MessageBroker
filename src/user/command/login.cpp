#include "../header/login.hpp"


using namespace std;

void login() {

    // Check if the user is already logged in
    if (!current_uid.empty() || !current_password.empty()) {
        cout << "You are already logged in" << endl;
        return;
    }

    // Check if the command is valid
    if (command.size()!=3) {
        cout << "login: format not valid!" << endl;
        return;
    }

    string uid, password;
    uid = command[1];
    password = command[2];

    if (uid.length() != 6 || isNumeric(uid) == false) {
        cout << "login: uid must be numeric and have 6 digits" << endl;
        return;
    }

    if (password.length() != 8 || isAlphanumeric(password) == false) {
        cout << "login: password must be alphanumeric and have 8 digits" << endl;
        return;
    }

    // Send the message to the server
    string request = "LIN " + uid + " " + password + "\n";
    string response = send_udp_request(request);

    // Check the response
    if (response == "RLI REG\n") {
        cout << "new user registered" << endl;
    } else if (response == "RLI OK\n") {
        cout << "successful login" << endl;
    } else if (response == "RLI NOK\n") {
        cout << "incorrect login attempt" << endl;
        return;
    } else {
        cout << "login: error" << endl;
        return;
    }

    // Save the current uid and password
    current_uid = uid;
    current_password = password;
}