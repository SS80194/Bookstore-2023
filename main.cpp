#include"Alldef.hpp"
#include"ConsolePanel.hpp"

extern void InitHome();
extern void ServeHome();

int main()
{
    freopen("testcase3.in","r",stdin);
    //std::string s;std::cin>>s;std::cout<<s<<std::endl;
    //printf("shangdaole\n");
    InitHome();
    ServeHome();
}