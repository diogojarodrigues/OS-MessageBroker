#include "./utils.hpp"

vector<string> command;
string current_uid;
string current_password;

bool isNumeric(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return false; // If any character is not a digit, return false
        }
    }
    return true; // All characters are digits
}

bool isAlphanumeric(const string& str) {
    for (char c : str) {
        if (!isalnum(static_cast<unsigned char>(c))) {
            return false; // If any character is not alphanumeric, return false
        }
    }
    return true; // All characters are alphanumeric
}

string get_auctions_bids(string response){
    int k=1; // 1 and 3 in aid, 0 in status
    string final = response.substr(7, response.size()-7);
    string auctions="";
    for (char c : final) {
        if(c=='\n')
            continue;
        if(k>=1 && k<=4){
            auctions += c;
            k++;
            continue;
        }
        else if(c=='1')
            auctions += "is active\n";
        else if(c=='0')
            auctions += "is not active\n";
        else if(c==' ')
            k=1;
        }
    return auctions;  
}

void print_auctions(string auctions){
    
    int k=0; // 1 and 3 in aid, 0 in status
    for (char c : auctions) {
        if(k>=0 && k<=3){
            if (k==0)
                cout << "AID: ";
            cout << c;
            k++;
            continue;
        }
        else if(c=='1')
            cout << "is active\n";
        else if(c=='0')
            cout << "is not active\n";
        else if(c==' ')
            k=0;
        }
}


string bid_record(string messages){
    vector<string> bid_records;
    string message;
    messages = messages.substr(7, messages.size()-7);
    for(char c:messages){
        if(c==' ' or c=='\n'){
            bid_records.push_back(message);
            message="";
        }
        else
            message += c;
    }
    string bid_record="";
    bid_record += "Host_UID:" + bid_records[0] + "  Auction_name:" + bid_records[1] + "  Asset_filename:" + bid_records[2] + "  Start_value:" + bid_records[3] + "  Start_date-time:" + bid_records[4] + ' ' +bid_records[5] + "  Time_active:" + bid_records[6] + "\n";
    unsigned long int i=7;
    while (true)
    {
        if(i>=bid_records.size()){
           break;
        }
        if(bid_records[i]=="B"){
           bid_record += "Bidder_UID:" + bid_records[i+1] + "  Bid_value:" + bid_records[i+2] + "  Bid_date-time:" + bid_records[i+3] + " "+ bid_records[i+4] + "  Bid_sec-time:" + bid_records[i+5] + "\n";
           i+=6;
        }
        else if(bid_records[i]=="E"){
           bid_record += "End_date-time:" + bid_records[i+1] +" "+ bid_records[i+2]+ "  End_sec-time:" + bid_records[i+3] + "\n";
           break;
        }
    }
    return bid_record;
    

}

void create_file_copy(ifstream* source_file, const string& destination_filename) {

    source_file->clear(); // Clear any error flags that might affect reading
    source_file->seekg(0, ios::beg); // Move the file pointer to the beginning of the file

    string destination = "src/user/images/" + destination_filename;
    ofstream destFile(destination, ios::binary);
    if (!destFile.is_open()) {
        cerr << "Error: Unable to open destination file." << endl;
        return;
    }

    // Define a buffer size for reading/writing data
    constexpr size_t bufferSize = 2048;
    char buffer[bufferSize];

    while (!source_file->eof() && !source_file->fail()) {
        source_file->read(buffer, bufferSize);
        streamsize bytesRead = source_file->gcount();

        if (bytesRead > 0) {
            destFile.write(buffer, bytesRead);
        }

        memset(buffer, 0, bufferSize);
    }

    if (source_file->fail() && !source_file->eof()) {
        cerr << "Error occurred while reading the source file." << endl;
    }

    destFile.close();
}

bool isValidFileName(const string str) {
    if (str.length() > 24) {
        return false;
    }
    for (char c : str) {
        if (!isalnum(static_cast<unsigned char>(c)) && c != '.' && c != '_' && c != '-') {
            return false; // If any character is not alphanumeric, return false
        }
    }
    return true; // All characters are alphanumeric
}

bool isValidName(const string str) {
    if (str.length() > 10) {
        return false;
    }
    for (char c : str) {
        if (!isalnum(static_cast<unsigned char>(c)) && c != '_' && c != '-') {
            return false; // If any character is not alphanumeric, return false
        }
    }
    return true; // All characters are alphanumeric
}