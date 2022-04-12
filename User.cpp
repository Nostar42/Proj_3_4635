#include "User.h"

void User::Register(std::string UserID, std::string Password){
    std::fstream fs;
    fs.open("Users.txt", std::fstream::in
    this->UserID = UserID;
    this->Password = Password;
    fstream 
}
void User::SetOnline(){
    this->Online = 1;
}
