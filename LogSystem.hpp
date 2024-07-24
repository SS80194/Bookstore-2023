#include<iostream>
#include<vector>
#include "MemoryRiver.hpp"
#include "StackList.hpp"
#include <string>
#include <sstream>

class SingleLog
{
    public:
    int type;
    double income,spend;
    public:
    SingleLog():income(0),spend(0){};
    SingleLog(double res_inc,double res_spd):income(res_inc),spend(res_spd){};
};
class LogSystem
{
    private:
    StackList<SingleLog>finance_log{"finance_log.mr"};
    public:
    void showFinance();
    void AddIncome(double x);
    void AddCost(double x);
};