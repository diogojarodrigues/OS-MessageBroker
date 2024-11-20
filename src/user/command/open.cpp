#include "../header/open.hpp"

void openn() {

    // Check if the user is logged in
    if (current_uid.empty() || current_password.empty()) {
        cout << "You should log in first!" << endl;
        return;
    }

    // Check if the command is valid
    if (command.size() != 5) {
        cout << "open: format not valid!" << endl;
        return;
    }

    // Handling parameters
    string name, fname, start_value, timeactive;
    name = command[1];
    fname = command[2];
    start_value = command[3];
    timeactive = command[4];

    // Check if the parameters are valid
    if (!isValidName(name)) {
        cout << "open: name must be alphanumeric and have less or equal than 10 digits" << endl;
        return;
    }

    if (!isValidFileName(fname)) {
        cout << "open: file name must be alphanumeric and have less or equal than 24 digits" << endl;
        return;
    }

    if (start_value.length() > 6 || !isNumeric(start_value)) {
        cout << "open: start value must be numeric" << endl;
        return;
    }

    if (timeactive.length() > 5 || !isNumeric(timeactive)) {
        cout << "open: time active must be numeric" << endl;
        return;
    }

    // Open file and get its size
    string path = "assets/" + fname;

    ifstream file(path, ios::binary);
    if (!file.is_open()) {
        cerr << "open: file does not exist" << endl;
        return;
    }

    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0) {
        cerr << "Error accessing the image information." << endl;
    }

    file.close();

    // Send request
    string request = "OPA " + current_uid + " " + current_password + " " + name + " " + start_value + " " + timeactive + " " + fname + " " + to_string(fileInfo.st_size) + " ";
    string response = send_tcp_request(request, SEND_TCP_IMAGE, path);

    // Handle response
    if (response == "ROA NOK\n") {
        cout << "auction could not be started" << endl;
    } else if (response == "ROA NLG\n") {
        cout << "user is not logged in" << endl;
        // current_uid.clear();
        // current_password.clear();
    } else if (response.length() == 11 && response.substr(0, 7) == "ROA OK ") {
        cout << "Auction started with AID: " + response.substr(7, 3) << endl;
    } else {
        cout << "open: error" << endl;
    }
};
