#ifndef BPT_ConsolePanel_HPP
#define BPT_ConsolePanel_HPP

#include<string>
#include<vector>
#include<iostream>
#include<sstream>
#include"StringC.hpp"

class tagInfo
{
    public:
    int type;
    std::string word;
    public:
    tagInfo()=default;
    tagInfo(int res_type):type(res_type){};
};

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
        //本质上是vector的s.clear() 使用swap是为了防止内存泄漏
        std::string temp;
        while(s_flow>>temp) {s.push_back(temp);}
    }
    void print()
    {
        for(std::string x:s)
            std::cout<<x<<std::endl;
    }
    int size(){return s.size();}
    bool safetyCheck(){for(std::string &x:s) if(x.size()>StringC::max_length) return false;return true;}
    void invalidOperation(int typ=0)
    {
        //if(typ) printf("Invalid Error code=%d\n",typ);else printf("Invalid\n");
        printf("Invalid\n");
        //std::cout<<s_ori<<std::endl;
        fflush(stdout);return ;
    }
    std::string operator[](int k){if(k>=size()) return "";return s[k];}
    std::string type(){return (size()?s[0]:"");}
    
    tagInfo parseTag(const std::string &res)
    {
        //固定的格式是-ISBN=""或什么的
        if(!res.size()) return tagInfo(-1);
        int i=0,len=res.size();
        tagInfo ret_value;
        std::string type1,infos;
        for(i=1;i<len;i++)
        {
            if(res[i]=='=')
                {++i;break;}
            else type1+=res[i];
        }
        if(type1=="ISBN") ret_value.type=0;
        else if(type1=="name") ret_value.type=1;
        else if(type1=="author") ret_value.type=2;
        else if(type1=="keyword") ret_value.type=3;
        else if(type1=="price") ret_value.type=4;
        else return tagInfo(-1);
        if(1<=ret_value.type&&ret_value.type<=3)
        {
            if(i==len-1||res[i]!='"'||res[len-1]!='"')
                return tagInfo(-1);
            i++;
        }
        for(;i<len;i++)
        {
            if(i!=len-1&&res[i]=='"') return tagInfo(-1);
            else if(res[i]!='"') infos+=res[i];
        }
        if(infos.size()>StringC::max_length) return tagInfo(-1);
        ret_value.word=infos;
        return ret_value;
    }
};

#endif