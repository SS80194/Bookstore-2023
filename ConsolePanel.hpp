#ifndef BPT_ConsolePanel_HPP
#define BPT_ConsolePanel_HPP

#include<string>
#include<vector>
#include<iostream>
#include<sstream>

class ConsolePanel
{
    public:
    std::string s_ori;
    std::vector<std::string>s;
    public:
    void get()
    {
        std::getline(std::cin,s_ori);
        std::stringstream s_flow(s_ori);
        std::vector<std::string>().swap(s);
        std::string temp;
        while(s_flow>>temp) {s.push_back(temp);}
    }
    void print()
    {
        for(std::string x:s)
            std::cout<<x<<std::endl;
    }
    int size(){return s.size();}
    void invalidOperation(){printf("Invalid\n");fflush(stdout);return ;}
    std::string operator[](int k){if(k>=size()) return 0;return s[k];}
    std::string type(){return (size()?s[0]:"");}
};

#endif