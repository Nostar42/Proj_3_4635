#pragma once
#include <string>
#include <fstream>

class User{
    public:
    private:
    bool Online = 0;
    std::string UserID;
    std::string Password;

    void Register(std::string UserID, std::string Password);
    void SetOnline();
};
