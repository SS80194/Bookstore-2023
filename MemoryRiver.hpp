#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>
#include<iostream>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2>
class MemoryRiver {
private:
    /* your code here */
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
    int rubbish_head=0;//rubbish_head是一个伪装成int的指针类型
public:
    friend class LinkList;
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    void initialise(string FN = "") 
    {
        if (FN != "") file_name = FN;
        file.open(file_name, std::ios::out|std::ios::binary);
        int tmp = 0;
        for (int i = 0; i <= info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }
    void initialize(string FN = "") 
    {
        initialise(FN);
    }
    void open()
    {
        file.open(file_name, std::ios::in|std::ios::binary);
    }
    void close()
    {
        file.close();
    }
    //读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, int n) 
    {
        if (n<0 || n > info_len) return;
        file.open(file_name, std::ios::in|std::ios::binary);
        file.seekg(n*sizeof(int),std::ios::beg);
        file.read((char*)&tmp,sizeof(int));
        file.close();
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) 
    {
        if (n<0 || n > info_len) return;
        file.open(file_name, std::ios::out|std::ios::in|std::ios::binary);
        file.seekp(n*sizeof(int),std::ios::beg);
        file.write((char*)&tmp,sizeof(int));
        file.close();
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    int write(T &t) 
    {
        /* your code here */
        get_info(rubbish_head,0);
        file.open(file_name, std::ios::out|std::ios::in|std::ios::binary);
        int target,index;
        if(rubbish_head)
        {
            index=rubbish_head;
            file.seekg(index,std::ios::beg);
            file.read((char*)&rubbish_head,sizeof(int));
            file.seekp(index,std::ios::beg);
        }
        else
        {
            file.seekp(0,std::ios::end);
            index=(int)(file.tellp());
            //printf("%d %llX\n",index,t);
        }
        file.write((char*)&t,sizeofT);
        file.close();
        write_info(rubbish_head,0);
        return index;
    }

    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T &t, const int index) {
        file.open(file_name, std::ios::out|std::ios::in|std::ios::binary);
        file.seekp(index,std::ios::beg);
        file.write((char*)&t,sizeofT);
        file.close();
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void read(T &t, const int index) {
        file.open(file_name,std::ios::in|std::ios::binary);
        file.seekg(index,std::ios::beg);
        //std::cout<<"NPT:"<<index<<" "<<file.tellg()<<std::endl;
        file.read((char*)&t,sizeofT);
        file.close();
    }

    //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
    //问题：Delete函数不能保证index一定可以删除,请在使用前多加确认。
    void Delete(int index) {
        get_info(rubbish_head,0);//获得老rubbish_head
        file.open(file_name,std::ios::in|std::ios::out|std::ios::binary);
        file.seekp(index,std::ios::beg);
        file.write((char*)&rubbish_head,sizeof(int));
        rubbish_head=index;
        file.close();
        write_info(rubbish_head,0);
    }
};

#endif //BPT_MEMORYRIVER_HPP