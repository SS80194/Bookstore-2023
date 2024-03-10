#include<cstdio>
#include<vector>
#include<string>
#include<sstream>
#include<iostream>
#include<map>
#include"ConsolePanel.hpp"
#include"AccountSystem.hpp"

ConsolePanel H;
AccountSystem A;
//BookSystem B;
//LogSystem L;

std::map<std::string,void (AccountSystem::*)()> map_A;


void InitHome()
{
    map_A["quit"]=AccountSystem::quit;
    map_A["exit"]=AccountSystem::quit;
    map_A["logout"]=AccountSystem::logout;

}

void ServeHome()
{
    while(true)
    {
        H.get();
        if(!H.size()) H.invalidOperation();
        else if(map_A.find(H.type())!=map_A.end())
        {
            (A.*map_A[H.type()])();
        }
        else H.invalidOperation();
    }
}