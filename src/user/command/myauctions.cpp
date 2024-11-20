#include "../header/myauctions.hpp"


void myauctions() {
    if (current_uid.empty()) {
        cout << "You should log in first!" << endl;
        return;
    }
    string request = "LMA " + current_uid +"\n";
    string response = send_udp_request(request);
    string status = response.substr(0, 7);
    if (status == "RMA NOK") {
        cout << "user UID has no ongoing auctions" << endl;
    } else if (status == "RMA NLG") {
        cout << "user is not logged in" << endl;
        // current_uid.clear();
        // current_password.clear();
    } else if (status == "RMA OK ") {
        string auctions = get_auctions_bids(response);
        cout << auctions ;
    } else 
        cout << "unknown error" << endl;
};