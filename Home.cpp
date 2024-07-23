#include<cstdio>
#include<vector>
#include<string>
#include<sstream>
#include<iostream>
#include<map>
#include"ConsolePanel.hpp"
#include"AccountSystem.hpp"
#include"BookSystem.hpp"

ConsolePanel H;
AccountSystem A;
BookSystem B;
//LogSystem L;

std::map<std::string,void (AccountSystem::*)()> map_A;
std::map<std::string,void (BookSystem::*)()> map_B;
//std::map<std::string,void (LogSystem::*)()> map_L;

void InitHome()
{
    //map_A
    map_A["quit"]=&AccountSystem::quit;
    map_A["exit"]=&AccountSystem::quit;
    map_A["logout"]=&AccountSystem::logout;
    map_A["su"]=&AccountSystem::su;
    map_A["register"]=&AccountSystem::signup;
    map_A["passwd"]=&AccountSystem::changePasswd;
    map_A["delete"]=&AccountSystem::deleteAccount;
    map_A["useradd"]=&AccountSystem::signupPro;
    //map_B
    map_B["show"]=&BookSystem::findBook;
    map_B["buy"]=&BookSystem::buyBook;
    map_B["select"]=&BookSystem::selectBook;
    map_B["modify"]=&BookSystem::modifyBook;
    map_B["import"]=&BookSystem::importBook;
    //map_H
    //----------Waiting For Implement

    A.initAccount();
}

void ServeHome()
{
    while(true)
    {
        H.get();
        if(H.s_ori=="") exit(0);
        std::cout<<"T:"<<H.s_ori<<std::endl;
        if(!H.size()) H.invalidOperation();
        else if(!H.safetyCheck()) H.invalidOperation();
        else if(map_A.find(H.type())!=map_A.end())
            (A.*map_A[H.type()])();
        else H.invalidOperation(588);
        //Wait to insert B&L
    }
}