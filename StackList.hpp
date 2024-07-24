#ifndef BPT_STACKLIST_HPP
#define BPT_STACKLIST_HPP

#include "MemoryRiver.hpp"
#include <fstream>

//StackList 支持在后面插入（含文件保存），以及取出最后几条
template<typename T>class StackList
{
    private:
    class SLNode
    {
        public:
        T val;
        int past_stop;
        public:
        SLNode()=default;
        SLNode(const T& val_res){val=val_res;}
        SLNode(const T&val_res,int past_stop_res){val=val_res;past_stop=past_stop_res;};
    };
    private:
    std::string file_name;
    MemoryRiver<SLNode,3>file;
    int size,begin_pos;
    public:
    StackList()=default;
    StackList(const string& file_name_res)
    {
        //LogSystem1:进一步实现这一个StackList类。
        ifstream ifs;
        file_name=file_name_res;
        file=file_name;

        ifs.open(file_name,std::ios::in);
        if(!ifs.is_open())//文件不存在
        {
            ofstream ofs(file_name);
            ofs.close();
            file.initialize();
        }
        else ifs.close();
        
        begin_pos=getBeginpos();
        size=getSize();

            //Do Something to initialize
    };
    
    int size_() const {return size;}

    int getSize() {int x;file.get_info(x,2);return x;}
    int getBeginpos() {int x;file.get_info(x,3);return x;}

    void setSize(int x) {file.write_info(x,2);}
    void setBeginpos(int x) {file.write_info(x,3);}

    void push_back(const T &val)
    {
        SLNode TK(val,begin_pos);
        begin_pos=file.write(TK);
        setSize(++size);setBeginpos(begin_pos);
    }
    SLNode getNode(int index)
    {
        SLNode x;
        file.read(x,index);
        return x;
    }
    void surfBack(int n,std::vector<T> &tar)
    {
        if(n>size) return ;
        
        for(int p=begin_pos,i=1;p&&i<=n;i++)
        {
            SLNode x=getNode(p);
            tar.push_back(x.val);
            p=x.past_stop;
        }

        reverse(tar.begin(),tar.end());
    }
};

#endif