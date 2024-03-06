//Bookstore-File storage

#ifndef BPT_FILE_STORAGE_BLIST_HPP
#define BPT_FILE_STORAGE_BLIST_HPP


#include<cstdio>
#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<cstring>
#include <fstream>
#include"MemoryRiver.hpp"

typedef int TTA;
typedef unsigned long long ull;

class LinkListConfiguration
{
    public:
    const static int B,P,M;
    const static TTA min_value,max_value;
    
    const static std::string default_name_file_basis;
    const static std::string default_name_file_detailed;

};
const int LinkListConfiguration::B=540,LinkListConfiguration::P=2*B,LinkListConfiguration::M=3540;
const TTA LinkListConfiguration::min_value=-2147483647;const TTA LinkListConfiguration::max_value=2147483647;
const std::string LinkListConfiguration::default_name_file_basis="KeyValueStorage1.mr";
const std::string LinkListConfiguration::default_name_file_detailed="KeyValueStorage2.mr";

//class StringC the string used to store index;
class StringC
{
    private:
    const static int max_length;
    char s[68];
    public:
    StringC() {s[0]=0;};
    StringC(char *);
    StringC(std::string);
    ull toHash();
    friend bool operator<(StringC,StringC);
    friend bool operator==(StringC,StringC);
};
const int StringC::max_length=64;
StringC::StringC(char *tar_c)
{
    for(int i=0;i<=max_length&&tar_c[i]!=0;i++)
        s[i]=tar_c[i];
}
StringC::StringC(std::string ori_s)
{
    const char *p=ori_s.c_str();
    std::strcpy(s,p);
}
ull StringC::toHash()
{
    ull ans=0,bss=1;
    for(int i=0;i<=max_length&&s[i]!=0;i++)
        ans+=bss*(s[i]-'a'),bss*=26;
    return ans;
}
bool operator<(StringC a,StringC b)
{
    for(int i=0;i<=StringC::max_length;i++)
    {
        if(b.s[i]==0) return false;
        if(a.s[i]==0) return true;
        if(a.s[i]<b.s[i]) return true;
        if(a.s[i]>b.s[i]) return false;
    }
    return false;
}
bool operator==(StringC a,StringC b)
{
    for(int i=0;i<=a.max_length;i++)
    {
        if(b.s[i]==0&&a.s[i]==0) break;
        if(a.s[i]!=b.s[i]) return false;
    }
    return true;
}

class KaData
{
    public:
    ull hash_id;StringC s;TTA value;
    KaData()=default;
    KaData(StringC,TTA);
    friend bool operator<(KaData ,KaData );
    friend bool operator==(KaData,KaData );//强等
    friend bool operator!=(KaData,KaData );//强等
    friend bool equal(KaData,KaData);//弱等
};
KaData::KaData(StringC ori_s,TTA ori_value)
{
    s=ori_s;hash_id=s.toHash();
    value=ori_value;
}
bool operator<(KaData a,KaData b)
{
    if(a.hash_id!=b.hash_id) return a.hash_id<b.hash_id;
    if(!(a.s==b.s)) return a.s<b.s;
    return a.value<b.value;
}
bool operator==(KaData a,KaData b)
{
    return (a.hash_id==b.hash_id&&a.value==b.value&&a.s==b.s);
}
bool operator!=(KaData a,KaData b)
{
    return !(a==b);
}
bool equal(KaData a,KaData b)
{
    return a.hash_id==b.hash_id&&a.s==b.s;
}
class ListNode
{
    private:
    int n,pre,nxt;
    KaData val[LinkListConfiguration::P+5];
    public:
    friend class LinkList;
    ListNode()
    {
        n=0,pre=0,nxt=0;
    }
};
class BasicInfos
{
    private:
    int n,pos[LinkListConfiguration::M];
    KaData first[LinkListConfiguration::M],end[LinkListConfiguration::M];
    public:
    friend class LinkList;
    BasicInfos(){n=0;}
    BasicInfos(int ,int );
    void push_back(int,const KaData&,const KaData&);
    void clear();
};
BasicInfos::BasicInfos(int ori_n,int ori_pos)
{
    n=ori_n;pos[n]=ori_pos;
}
void BasicInfos::push_back(int ori_pos,const KaData &ori_first,const KaData &ori_end)
{
    n++;
    pos[n]=ori_pos;
    first[n]=ori_first;
    end[n]=ori_end;
}
void BasicInfos::clear()
{
    n=0;
}
class LinkList
{
    private:
    BasicInfos content;
    MemoryRiver<ListNode> file_detailed;
    std::string name_file_detailed;
    public:
    friend class ListNode;
    friend class BasicInfo;
    //文件名使用static储存，待补充
    LinkList()//使用默认文件名构建
    {
        ifstream ifs;
        name_file_detailed=LinkListConfiguration::default_name_file_detailed;

        file_detailed=name_file_detailed;
        
        //detailed_file
        ifs.open(name_file_detailed,std::ios::in);
        if(!ifs.is_open())//文件不存在
        {
            ofstream ofs(name_file_detailed);
            ofs.close();
            file_detailed.initialize();
        }
        else ifs.close();

        if(!begin_pos()) initialize();
        else syncBasisFile();
    }
    LinkList(std::string ori_basis_name,std::string ori_detailed_name)
    {
        
        name_file_detailed=ori_detailed_name;

        file_detailed=ori_detailed_name;

        ifstream ifs;        
        //detailed_file
        ifs.open(name_file_detailed,std::ios::in);
        if(!ifs.is_open())//文件不存在
        {
            ofstream ofs(name_file_detailed);
            ofs.close();
            file_detailed.initialize();
        }
        else ifs.close();

        if(!begin_pos()) initialize();
        else syncBasisFile();
    }
    
    int begin_pos();
    void setBegin_pos(int );//请使用setBegin_pos方法来设置begin_pos
    bool empty();
    void syncBasisFile();
    void initialize();
    void splitNode(int );
    void eraseNode(int );
    void insert(KaData);
    void find(std::vector<TTA>&,KaData);
    bool erase(KaData);
};

int LinkList::begin_pos()
{
    int x;file_detailed.get_info(x,1);
    return x;
}
void LinkList::setBegin_pos(int x)
{
    file_detailed.write_info(x,1);
}
bool LinkList::empty()
{
    return (begin_pos()==0?true:false);
}
void LinkList::syncBasisFile()
{
    //SC++;
    BasicInfos &A=content;
    A.clear();
    for(int itr=begin_pos();itr;)
    {
        ListNode cur;file_detailed.read(cur,itr);
        A.push_back(itr,cur.val[0],cur.val[cur.n-1]);
        itr=cur.nxt;
    }
}
void LinkList::initialize()//对空链表初始化一个节点
{
    ListNode first_node;
    int first_pos=file_detailed.write(first_node);
    setBegin_pos(first_pos);
    content.n=1;content.pos[1]=first_pos;
}
void LinkList::insert(KaData val)
{
    //插入valx
    if(empty()) initialize();
    int target_node_i=0,target_cur_i=0;
    BasicInfos &cur=content;
    for(int i=1;i<=cur.n;i++)
    {
        target_node_i=cur.pos[i];
        target_cur_i=i;
        if(val<cur.end[i]) break;
    }
    ListNode opt_node;file_detailed.read(opt_node,target_node_i);
    int insert_pos=std::lower_bound(opt_node.val,opt_node.val+opt_node.n,val)-opt_node.val;
    //insert_pos是第一个>=val的点所在位置
    opt_node.n++;
    for(int i=opt_node.n-1;i>=insert_pos+1;i--) opt_node.val[i]=opt_node.val[i-1];
    opt_node.val[insert_pos]=val;
    file_detailed.update(opt_node,target_node_i);
    if(cur.first[target_cur_i]!=opt_node.val[0]||cur.end[target_cur_i]!=opt_node.val[opt_node.n-1])
    {
        cur.first[target_cur_i]=opt_node.val[0];
        cur.end[target_cur_i]=opt_node.val[opt_node.n-1];
        //cur.first=opt_node.val[0];cur.end=opt_node.val[opt_node.n-1];
    }
    if(opt_node.n>LinkListConfiguration::P) splitNode(target_node_i);
}
void LinkList::splitNode(int node1_id)
{
    ListNode node1,node2,node3;
    file_detailed.read(node1,node1_id);
    int node2_id=file_detailed.write(node2),node3_id=node1.nxt;
    if(node3_id)
    {
        file_detailed.read(node3,node3_id);
        node3.pre=node2_id;node2.nxt=node3_id;
    }
    else node2.nxt=0;
    node2.pre=node1_id;node1.nxt=node2_id;
    for(int i=LinkListConfiguration::B;i<node1.n;i++)
        node2.val[node2.n++]=node1.val[i];
    node1.n=LinkListConfiguration::B;
    if(node1_id) file_detailed.update(node1,node1_id);
    if(node2_id) file_detailed.update(node2,node2_id);
    if(node3_id) file_detailed.update(node3,node3_id);
    syncBasisFile();

}
void LinkList::eraseNode(int node2_id)
{
    ListNode node1,node2,node3;
    file_detailed.read(node2,node2_id);
    int node1_id=node2.pre,node3_id=node2.nxt;
    if(node1_id)
    {
        file_detailed.read(node1,node1_id);
        node1.nxt=node3_id;
    }
    else setBegin_pos(node3_id);
    if(node3_id)
    {
        file_detailed.read(node3,node3_id);
        node3.pre=node1_id;
    }
    file_detailed.Delete(node2_id);
    if(node1_id) file_detailed.update(node1,node1_id);
    if(node3_id) file_detailed.update(node3,node3_id);
    syncBasisFile();
}
bool LinkList::erase(KaData val)
{
    if(empty()) return false;
    int target_node_i=0,target_cur_i=0;
    BasicInfos &cur=content;
    for(int i=1;i<=cur.n;i++)
    {
        target_node_i=cur.pos[i];
        target_cur_i=i;
        if(val<cur.end[i]||val==cur.end[i]) break;
    }
    if(!target_node_i) return false;
    ListNode opt_node;file_detailed.read(opt_node,target_node_i);
    int delete_pos=std::lower_bound(opt_node.val,opt_node.val+opt_node.n,val)-opt_node.val;
    if(delete_pos>=opt_node.n||!(opt_node.val[delete_pos]==val)) return false;
    opt_node.n--;
    for(int i=delete_pos;i<opt_node.n;i++) opt_node.val[i]=opt_node.val[i+1];
    if(!opt_node.n) eraseNode(target_node_i);
    else
    {
        file_detailed.update(opt_node,target_node_i);
        if(cur.first[target_cur_i]!=opt_node.val[0]||cur.end[target_cur_i]!=opt_node.val[opt_node.n-1])
        {
            cur.first[target_cur_i]=opt_node.val[0];
            cur.end[target_cur_i]=opt_node.val[opt_node.n-1];
        //cur.first=opt_node.val[0];cur.end=opt_node.val[opt_node.n-1];
        }
    }
    return true;
}
void LinkList::find(std::vector<TTA>&a,KaData val)
{
    int target_node_i=0;
    BasicInfos &cur=content;
    KaData vall(val.s,LinkListConfiguration::min_value);
    KaData valr(val.s,LinkListConfiguration::max_value);
    for(int i=1;i<=cur.n;i++)
    {    
        target_node_i=cur.pos[i];
        if(cur.end[i]<vall) {continue;}
        if(valr<cur.first[i]) break;
        ListNode opt_node;file_detailed.read(opt_node,target_node_i);
        for(int i=0;i<opt_node.n;i++)
            if(equal(val,opt_node.val[i])) a.push_back(opt_node.val[i].value);
    }
}

#endif