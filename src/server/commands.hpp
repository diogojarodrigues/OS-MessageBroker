#ifndef COMMANDS_SERVER_HPP
#define COMMANDS_SERVER_HPP

// ################ UDP COMMANDS ################
void login(string request);
void logout(string request, bool unregister = false);
void my_auctions(string request);
void my_bids(string request);
void list(string request);
void show_record(string request);

// ################ TCP COMMANDS ################
void openn(int sockett, string request);
void closee(int sockett, string request);
void show_asset(int sockett, string request);
void bid(int sockett, string request);


#endif // COMMANDS_SERVER_HPP