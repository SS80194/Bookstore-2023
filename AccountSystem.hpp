#ifndef Bookstore_AccountSystem_HPP
#define Bookstore_AccountSystem_HPP

#include"ConsolePanel.hpp"

class AccountSystem
{
    private:
    public:
    void quit();
    void logout();
};

//下面的可以搬到Accountsystem.cpp里边
extern ConsolePanel H;


void AccountSystem::quit()
{
    if(H.size()>1) {H.invalidOperation();return ;}
    else exit(0);
}
void AccountSystem::logout()
{
    printf("Trying to logout");
}
#endif