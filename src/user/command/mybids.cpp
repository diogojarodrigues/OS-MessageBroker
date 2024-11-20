#include "../header/mybids.hpp"


void mybids() {
    if (current_uid.empty()) {
        cout << "You should log in first!" << endl;
        return;
    }
    string request = "LMB " + current_uid +"\n";
    string response = send_udp_request(request);
    string status = response.substr(0, 7);

    if (status == "RMB NOK") {
        cout << "user UID has no ongoing bids" << endl;
    } else if (status == "RMB NLG") {
        cout << "user is not logged in" << endl;
        // current_uid.clear();
        // current_password.clear();
    } else if (status == "RMB OK ") {
        string bids= get_auctions_bids(response);   
        cout << bids;
        return;
    } else 
        cout << "unknown error" << endl;
};