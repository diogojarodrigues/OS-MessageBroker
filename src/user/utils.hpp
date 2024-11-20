#ifndef UTILS_HPP
#define UTILS_HPP

#include "./protocol.hpp"

#include <string>
#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
#include <sstream>
#include <fstream>
#include <cstring>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

extern vector<string> command;
extern string current_uid;
extern string current_password;

bool isNumeric(const string& str);
bool isAlphanumeric(const string& str);
string get_auctions_bids(string response);
void print_auctions(string auctions);
void create_file_copy(ifstream* source_file, const string& destination_filename);
string bid_record(string message);
bool isValidFileName(const string str);
bool isValidName(const string str);

#endif // UTILS_HPP
