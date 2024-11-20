#include "../header/show_asset.hpp"

void show_asset() {
    if (command.size()!=2) {
        cout << "show_asset: format not valid!" << endl;
        return;
    }

    string aid;
    aid = command[1];
    if(aid.length()!=3 || !isNumeric(aid)){
        cout << "show_asset: aid must be numeric and have 3 digits" << endl;
        return;
    };

    string request = "SAS " + aid +"\n";
    string response = send_tcp_request(request, RECEIVE_TCP_IMAGE);  
  
};