#include "../header/show_record.hpp"

void show_record() {
    
    if (command.size()!=2) {
        cout << "show_record: format not valid!" << endl;
        return;
    }
    
    string aid = command[1];
    if(aid.length()!=3 || !isNumeric(aid)){
        cout << "show_record: aid must be numeric and have 3 digits" << endl;
        return;
    };

    string request = "SRC " + aid +"\n";
    string response = send_udp_request(request); 
    
    string r=response.substr(0, 7);
    if(response=="RRC NOK\n"){
        cout << "show_record: asset does not exist" << endl;
        return;
    }
    else if(r=="RRC OK "){
        string auctions = bid_record(response);
        cout << auctions;
    }
    else {
        cout << "show_record: unknown error" << endl;
    }
};