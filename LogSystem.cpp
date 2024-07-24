#include "ConsolePanel.hpp"
#include "StringC.hpp"
#include "LogSystem.hpp"
#include "AccountSystem.hpp"
#include <vector>
#include<iostream>
#include<iomanip>

extern ConsolePanel H;
extern AccountSystem A;
extern int parseInt(const std::string &);

void LogSystem::showFinance()
{
    if(!A.privilegeEnable(7)) return H.invalidOperation(1048);
    if(H.size()>3) return H.invalidOperation(1001);
    int count=0,max_count=finance_log.size_();
    if(H.size()==3)
    {
        count=parseInt(H[2]);
        if(count<0) return H.invalidOperation(1002);
    }
    else count=max_count;
    if(count>max_count) return H.invalidOperation(1003);
    if(!count) {std::cout<<std::endl;return ;}
    std::vector<SingleLog> a;
    finance_log.surfBack(count,a);
    double income=0,cost=0;
    for(SingleLog v:a)
    {
        income+=v.income;
        cost+=v.spend;
    }
    std::cout<<"+ "<<std::fixed<<std::setprecision(2)<<income<<" ";
    std::cout<<"- "<<std::fixed<<std::setprecision(2)<<cost<<std::endl;
}

void LogSystem::AddIncome(double x)
{
    finance_log.push_back(SingleLog(x,0));
}

void LogSystem::AddCost(double x)
{
    finance_log.push_back(SingleLog(0,x));
}