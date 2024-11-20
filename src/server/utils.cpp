#include "utils.hpp"

// ###########################################################
// #                   AUXILIARY FUNCTIONS                   #
// ###########################################################

bool isAlpha(const string str) {
    for (char c : str) {
        if (!isalpha(static_cast<unsigned char>(c))) {
            return false; // If any character is not a letter, return false
        }
    }
    return true; // All characters are letters
}

bool isNumeric(const string str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return false; // If any character is not a digit, return false
        }
    }
    return true; // All characters are digits
}

bool isAlphanumeric(const string str) {
    for (char c : str) {
        if (!isalnum(static_cast<unsigned char>(c))) {
            return false; // If any character is not alphanumeric, return false
        }
    }
    return true; // All characters are alphanumeric
}

bool isValidFileName(const string str) {
    for (char c : str) {
        if (!isalnum(static_cast<unsigned char>(c)) && c != '.' && c != '_' && c != '-') {
            return false; // If any character is not alphanumeric, return false
        }
    }
    return true; // All characters are alphanumeric
}

bool isValidName(const string str) {
    for (char c : str) {
        if (!isalnum(static_cast<unsigned char>(c)) && c != ' ' && c != '-') {
            return false; // If any character is not alphanumeric, return false
        }
    }
    return true; // All characters are alphanumeric
}

// Function to get the last ID from the directory
int getLastAid(const string& directory) {
    for (int i = 1; i < 1000; i++) {
        ostringstream name;
        name << setfill('0') << setw(3) << i;
        string filename = name.str();

        if (!fs::exists(directory + "/" + filename)) {
            return i;
        }
    }
    return -1;
}


// ###########################################################
// #                 VALIDATION FUNCTIONS                    #
// ###########################################################
bool isUid(const string str) {
    if (str.length() != 6) {
        if (DEBUG) cout << "uid length is not 6\n";
        return false;
    }

    bool aux = isNumeric(str);
    if (!aux && DEBUG) cout << "uid is not numeric\n";
    return aux;
}

bool isAid(const string str) {
    if (str.length() != 3) {
        if (DEBUG) cout << "aid length is not 3\n";
        return false;
    }

    bool aux = isNumeric(str);
    if (!aux && DEBUG) cout << "aid is not numeric\n";
    return aux;
}

bool isPassword(const string str) {
    if (str.length() != 8) {
        if (DEBUG) cout << "password length is not 8\n";
        return false;
    }

    bool aux = isAlphanumeric(str);
    if (!aux && DEBUG) cout << "password is not alphanumeric\n";
    return aux;
}

bool isFileName(const string str) {
    if (str.length() > 24) {
        if (DEBUG) cout << "filename length is greater than 24\n";
        return false;
    }

    bool aux = isValidFileName(str);
    if (!aux && DEBUG) cout << "filename is not valid\n";
    return aux;
}

bool isFileSize(const string str) {
    if (str.length() > 8) {
        if (DEBUG) cout << "file size length is greater than 8\n";
        return false;
    }

    bool aux = isNumeric(str);
    if (!aux && DEBUG) cout << "file size is not numeric\n";
    return aux;
}

bool isName(const string str) {
    if (str.length() > 10) {
        if (DEBUG) cout << "name length is greater than 10\n";
        return false;
    }

    bool aux = isValidName(str);
    if (!aux && DEBUG) cout << "name is not alphanumeric\n";
    return aux;    
}

bool isValue(const string str) {
    if (str.length() > 6) {
        if (DEBUG) cout << "value length is greater than 6\n";
        return false;
    }

    bool aux = isNumeric(str);
    if (!aux && DEBUG) cout << "value is not numeric: " << str << "\n";
    return aux;
}

bool isDuration(const string str) {
    if (str.length() > 5) {
        if (DEBUG) cout << "duration length is greater than 5\n";
        return false;
    }

    bool aux = isNumeric(str);
    if (!aux && DEBUG) cout << "duration is not numeric\n";
    return aux;
}

// ###########################################################
// #                    UTILS FUNCTIONS                      #
// ###########################################################
bool user_exists(const string uid) {
    const string directory = "src/server/data/users/" + uid + "/";
    return fs::exists(directory);
}

bool user_loggged_in(const string uid) {
    const string path = "src/server/data/users/" + uid + "/login.txt";
    return fs::exists(path);
}

bool user_registered(const string uid) {
    const string path = "src/server/data/users/" + uid + "/pass.txt";
    return fs::exists(path);
}

bool auction_exists(const string aid) {
    const string directory = "src/server/data/auctions/" + aid + "/";
    return fs::exists(directory);
}

bool passwordsMatch(const string uid, const string password) {

    const string path = "src/server/data/users/" + uid + "/pass.txt";

    ifstream inputPassFile(path);
    if (!inputPassFile.is_open()) {
        return false;
    }

    //check if password is correct
    string correctPassword;
    inputPassFile >> correctPassword;
    inputPassFile.close();

    return password == correctPassword;
}

string format_datetime(time_t timestamp) {
    struct tm *timeinfo;
    char buffer[20]; // Sufficiently large buffer to hold the formatted date and time

    timeinfo = localtime(&timestamp);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    return string(buffer);
}

bool auction_closed(const string aid) {
    const string path_end = "src/server/data/auctions/" + aid + "/end.txt";
    if (fs::exists(path_end)){
        return true;
    }

    const string path = "src/server/data/auctions/" + aid + "/start.txt";
    ifstream inputStartFile(path);
    if (!inputStartFile.is_open()) {
        cerr << "Error opening file: " << path << endl;
        exit(-1);
    }

    const int buffer_size = 512;
    char buffer[buffer_size] = "";
    inputStartFile.read(buffer, buffer_size);
    inputStartFile.close();

    //UID name assetfname startvalue timeactive start_ datetime startfulltime
    int k=0;
    string timeactive,start_fulltime;
    for(char c : buffer){
        if(c==' ')
            k++;
        else if(k==4)
            timeactive+=c;
        else if(k==7)
            start_fulltime+=c;
    }

    if(stoi(timeactive)+stoi(start_fulltime)>time(NULL)){
        return false;
    }

    string file = "src/server/data/auctions/" + aid + "/end.txt";
    time_t timestamp = stoi(timeactive)+stoi(start_fulltime);
    string content = format_datetime(timestamp)+ " " + timeactive;
    createFile(file, content);
    return true;

}

string getAuctionOwner(const string aid) {
    const string path = "src/server/data/auctions/" + aid + "/start.txt";

    ifstream inputOwnerFile(path);
    if (!inputOwnerFile.is_open()) {        
        cerr << "Error opening file" << endl;
        exit(-1);
        return "";
    }

    //check if password is correct
    string owner;
    inputOwnerFile >> owner;
    inputOwnerFile.close();

    return owner;

}

string getAuctionDuration(const string aid, time_t inicial_time) {
    const string path = "src/server/data/auctions/" + aid + "/start.txt";

    ifstream inputOwnerFile(path);
    if (!inputOwnerFile.is_open()) {        
        cerr << "Error opening file" << endl;
        exit(-1);
        return "";
    }

    vector<string> fields = split(readFile(path));

    if (fields.size() != 8) {
        if (DEBUG) cout << "getAuctionDuration: wrong number of fields\n";
        exit(-1);
    }

    time_t duration = inicial_time - stoi(fields[7]);

    return to_string(duration);
}

vector<string> split(string str) {
    vector<string> tokens;
    
    if (str[str.length() - 1] == '\n') {
        str = str.substr(0, str.length() - 1);
    }

    stringstream ss(str);
    string token;
    
    while (getline(ss, token, ' ')) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to generate the next ID with three digits
string generateAid() {
    const string directory = "src/server/data/auctions/";

    int id = getLastAid(directory);
    if (id == 999) return "ERR";
    
    ostringstream oss;
    oss << setw(3) << setfill('0') << id;
    return oss.str();
}

void createFile(const string path, const string content) {
    ofstream file(path);

    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        exit(-1);
    }
    if (content != "") file << content;

    file.close();
}

string readFile(const string path) {
    if (!fs::exists(path)) return string();

    ifstream file(path);

    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        exit(-1);
    }

    string content((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));

    file.close();

    return content;
}

int saveImage(int socket, string file_path, int size) {

    ofstream assetFile(file_path, ios::binary);
    if (!assetFile.is_open()) {
        cerr << "Error opening file" << endl;
        return -1;
    }

    char buffer[BUFFER_SIZE] = {0}; // Initialize buffer
    int bytes_read = 0;
    int count = 0;

    while ((bytes_read = read(socket, buffer, sizeof(buffer))) > 0) {
        count += bytes_read;

        // Check if total bytes read match the expected size
        if (count >= size) {
            assetFile.write(buffer, bytes_read-1);
            memset(buffer, 0, sizeof(buffer));
            break; // Exit the loop if size is reached
        }

        assetFile.write(buffer, bytes_read);
        memset(buffer, 0, sizeof(buffer));

    }

    if (bytes_read == -1) {
        cerr << "Error reading from socket" << endl;
        return -1;
    }

    assetFile.close();

    count = count -1;
    if (count != size) {
        cerr << "saveImage: (count) " << count << " != " << size << " (size)" << endl;
    }
    
    return count;
}

void sendImage(int sockett, const string aid) {
    string path = "src/server/data/auctions/" + aid + "/asset/";

    // Check if the directory exists
    if (!fs::exists(path) || !fs::is_directory(path)) {
        cerr << "Directory does not exist or is not a directory" << endl;
        exit(-1);
    }

    // Get the first file in the directory
    fs::directory_iterator itr(path);
    if (itr == fs::end(itr)) {
        cerr << "No files found in the directory" << endl;
        exit(-1);
    }

    string filePath = itr->path().string(); // Get the file path

    ifstream assetFile(filePath, ios::binary);
    if (!assetFile.is_open()) {
        cerr << "Error opening file" << endl;
        exit(-1);
    }

    string filename = fs::path(filePath).filename().string();
    string filesize = to_string(fs::file_size(filePath));

    // Print file name and file size
    write(sockett, filename.c_str(), filename.length());
    write(sockett, " ", 1);
    write(sockett, filesize.c_str(), filesize.length());
    write(sockett, " ", 1);

    char buffer[BUFFER_SIZE];
    int bytes_read = 0;

    while (!assetFile.eof()) {
        bytes_read = assetFile.read(buffer, sizeof(buffer)).gcount();
        write(sockett, buffer, bytes_read);
        memset(buffer, 0, sizeof(buffer));
    }

    write(sockett, "\n", 1);

    assetFile.close();
}

int getHighestBid(string aid){
    string path = "src/server/data/auctions/" + aid + "/bids/";
    int highestBid = 0;
    for (const auto & entry : fs::directory_iterator(path)){
        string filePath = entry.path().string();
        string s_bid = filePath.substr(34,6);
        int bid = stoi(s_bid);
        if(bid>highestBid){
            highestBid = bid;
        }
    }
    if(highestBid==0){
        const string path = "src/server/data/auctions/" + aid + "/start.txt";
        ifstream inputStartFile(path);
        if (!inputStartFile.is_open()) {
            cerr << "Error opening file" << endl;
            exit(-1);
            return false;
        }
        const int buffer_size = 512;
        char buffer[buffer_size] = "";
        inputStartFile.read(buffer, buffer_size);
        inputStartFile.close();
        //UID name assetfname startvalue timeactive start_ datetime startfulltime
        int k=0;
        string initial_bid;
        for(char c : buffer){
            if(c==' ')
                k++;
            else if(k==3)
                initial_bid+=c;
            else if(k==4)
                break;
        }
        highestBid = stoi(initial_bid)-1;
    }
    return highestBid;
}

    // readFile(entry.path().string());
    
vector<string> getBidders(const string aid) {
    string path = "src/server/data/auctions/" + aid + "/bids/";
    vector<string> bidders;
    
    for (const auto &entry : fs::directory_iterator(path)) {
        bidders.push_back(entry.path().filename().string());
    }
    
    // Sorting the bidders based on the numerical value in descending order
    sort(bidders.begin(), bidders.end(), [](const string& a, const string& b) {
        return stoi(a) > stoi(b); // Assuming bidder names are convertible to integers
    });

    // Selecting the top 50 bidders (or fewer if less than 50 bidders exist)
    size_t numBidders = min(bidders.size(), static_cast<size_t>(50));
    bidders.resize(numBidders);

    // Reverse the vector to display the highest 50 in ascending order (from lowest to highest)
    reverse(bidders.begin(), bidders.end());

    vector<string> bidders_content;
    for (string bidder : bidders) {
        string content = readFile(path + bidder);
        bidders_content.push_back(content);
    }

    return bidders_content;
}