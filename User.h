#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <ios>
#include <limits>
#include <sstream>
class User{
    public:
    private:
    bool Online = 0;
    std::string UserID;
    std::string Password;
    public:
    int Register(std::string UserID, std::string Password);
    void SetOnline();
};
