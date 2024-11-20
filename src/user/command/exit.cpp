#include "../header/exit.hpp"


void exitt() {

    // Check if the user is logged in
    if (!current_uid.empty() || !current_password.empty()) {
        cout << "You should logout first" << endl;
        return;
    }

    cout << "Exiting..." << endl;
    exit(0);
};