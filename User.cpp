#include "User.h"

int User::Register(std::string UserID, std::string Password){
    std::string filetext;
    std::fstream fs;
    this->UserID = UserID;
    this->Password = Password;
    fs.open("Users.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    
    if(UserID.length() > 16){        
        fs.close();
        return 3;
    }
    else if(Password.length() > 16){
        fs.close();
        return 2;
    }

    if(fs.is_open()){ 
        std::cout << "Opening File Users.txt" << std::endl;
        std::cout << "Writng Username: " << this->UserID << " and Password: " << this->Password << std::endl;
        //fs << UserID << '\t' << Password << std::endl;
        fs << std::left <<  std::setw(16) <<  UserID << '\t' << std::setw(16) <<  Password << std::endl;
        fs.clear();
        fs.seekg(0, std::ios::beg);
        while(!fs.eof()){
            std::getline(fs,filetext);
            std::cout << filetext << std::endl;
            if(this->UserID == filetext){
                fs.close();
                return 4;
            }
        }
        fs.close();
        return 1;
    }

    std::cout << "Failed to open file" << std::endl;
    return -1;
}
void User::SetOnline(){
    if(this->Online == 0){
        this->Online = 1;
    }
    if(this->Online == 1){
        this->Online = 0;
    }
}
