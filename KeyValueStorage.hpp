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
#include"StringC.hpp"

typedef unsigned long long ull;

class LinkListConfiguration
{
    public:
    const static int B,P,M;
    
    const static std::string default_name_file_basis;
    const static std::string default_name_file_detailed;

};
const int LinkListConfiguration::B=540,LinkListConfiguration::P=2*B,LinkListConfiguration::M=3540;
const std::string LinkListConfiguration::default_name_file_basis="KeyValueStorage1.mr";
const std::string LinkListConfiguration::default_name_file_detailed="KeyValueStorage2.mr";

template<typename TTA> class KaData
{
    public:
    ull hash_id;StringC s;TTA value;
    KaData()=default;
    KaData(StringC,TTA);
    template<typename TTS>friend bool operator<(KaData<TTS> ,KaData<TTS> );
    template<typename TTS>friend bool operator==(KaData<TTS>,KaData<TTS> );//强等
    template<typename TTS>friend bool operator!=(KaData<TTS>,KaData<TTS> );//强等
    template<typename TTS>friend bool operator==(StringC,KaData<TTS> );//弱等
    template<typename TTS>friend bool operator!=(StringC,KaData<TTS> );//弱等
    template<typename TTS>friend bool operator==(KaData<TTS>,StringC );//弱等
    template<typename TTS>friend bool operator!=(KaData<TTS>,StringC );//弱等
    template<typename TTS>friend bool equal(KaData<TTS>,KaData<TTS>);//弱等
};
template<typename TTA>KaData<TTA>::KaData(StringC ori_s,TTA ori_value)
{
    s=ori_s;hash_id=s.toHash();
    value=ori_value;
}
template<typename TTA>bool operator<(KaData<TTA> a,KaData<TTA> b)
{
    if(!(a.s==b.s)) return a.s<b.s;
    return a.value<b.value;
}
template<typename TTA>bool operator==(KaData<TTA> a,KaData<TTA> b)
{
    return (a.hash_id==b.hash_id&&a.value==b.value&&a.s==b.s);
}
template<typename TTA>bool operator!=(KaData<TTA> a,KaData<TTA> b)
{
    return !(a==b);
}
template<typename TTA>bool operator==(KaData<TTA> a,StringC b)
{
    return a.s==b;
}
template<typename TTA>bool operator!=(KaData<TTA> a,StringC b)
{
    return !(a==b);
}
template<typename TTA>bool operator==(StringC a,KaData<TTA> b)
{
    return a==b.s;
}
template<typename TTA>bool operator!=(StringC a,KaData<TTA> b)
{
    return !(a==b.s);
}
template<typename TTA>bool equal(KaData<TTA> a,KaData<TTA> b)
{
    return a.hash_id==b.hash_id&&a.s==b.s;
}
template<typename TTA>bool operator<(KaData<TTA> a,StringC b)
{
    return a.s<b;
}
template<typename TTA>bool operator<(StringC a,KaData<TTA> b)
{
    return a<b.s;
}

template<typename TTA> class ListNode
{
    private:
    int n,pre,nxt;
    KaData<TTA> val[LinkListConfiguration::P+5];
    public:
    template<typename TTS> friend class KVS;
    ListNode()
    {
        n=0,pre=0,nxt=0;
    }
};
template<typename TTA>class BasicInfos
{
    private:
    int n,pos[LinkListConfiguration::M];
    KaData<TTA> first[LinkListConfiguration::M],end[LinkListConfiguration::M];
    public:
    template<typename TTS> friend class KVS;
    BasicInfos(){n=0;}
    BasicInfos(int ,int );
    void push_back(int,const KaData<TTA>&,const KaData<TTA>&);
    void clear();
};
template<typename TTA>BasicInfos<TTA>::BasicInfos(int ori_n,int ori_pos)
{
    n=ori_n;pos[n]=ori_pos;
}
template<typename TTA>void BasicInfos<TTA>::push_back(int ori_pos,const KaData<TTA> &ori_first,const KaData<TTA> &ori_end)
{
    n++;
    pos[n]=ori_pos;
    first[n]=ori_first;
    end[n]=ori_end;
}
template<typename TTA>void BasicInfos<TTA>::clear()
{
    n=0;
}


template<typename TTA>class KVS
{
    private:
    BasicInfos<TTA> content;
    MemoryRiver<ListNode<TTA>> file_detailed;
    std::string name_file_detailed;
    public:
    template<typename TTS> friend class ListNode;
    template<typename TTS> friend class BasicInfos;
    //文件名使用static储存，待补充
    KVS()//使用默认文件名构建
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
    KVS(std::string ori_basis_name,std::string ori_detailed_name)
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
    void insert(KaData<TTA>);
    void find(std::vector<TTA>&,StringC index);
    bool exist(StringC index);
    bool erase(KaData<TTA>);
    bool erase(StringC);
};

template<typename TTA>int KVS<TTA>::begin_pos()
{
    int x;file_detailed.get_info(x,1);
    return x;
}
template<typename TTA>void KVS<TTA>::setBegin_pos(int x)
{
    file_detailed.write_info(x,1);
}
template<typename TTA>bool KVS<TTA>::empty()
{
    return (begin_pos()==0?true:false);
}
template<typename TTA>void KVS<TTA>::syncBasisFile()
{
    //SC++;
    BasicInfos<TTA> &A=content;
    A.clear();
    for(int itr=begin_pos();itr;)
    {
        ListNode<TTA> cur;file_detailed.read(cur,itr);
        A.push_back(itr,cur.val[0],cur.val[cur.n-1]);
        itr=cur.nxt;
    }
}
template<typename TTA>void KVS<TTA>::initialize()//对空链表初始化一个节点
{
    ListNode<TTA> first_node;
    int first_pos=file_detailed.write(first_node);
    setBegin_pos(first_pos);
    content.n=1;content.pos[1]=first_pos;
}
template<typename TTA>void KVS<TTA>::insert(KaData<TTA> val)
{
    //插入valx
    if(empty()) initialize();
    int target_node_i=0,target_cur_i=0;
    BasicInfos<TTA> &cur=content;
    for(int i=1;i<=cur.n;i++)
    {
        target_node_i=cur.pos[i];
        target_cur_i=i;
        if(val<cur.end[i]) break;
    }
    ListNode<TTA> opt_node;file_detailed.read(opt_node,target_node_i);
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
template<typename TTA>void KVS<TTA>::splitNode(int node1_id)
{
    ListNode<TTA> node1,node2,node3;
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
template<typename TTA>void KVS<TTA>::eraseNode(int node2_id)
{
    ListNode<TTA> node1,node2,node3;
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
template<typename TTA>bool KVS<TTA>::erase(KaData<TTA> val)
{
    if(empty()) return false;
    int target_node_i=0,target_cur_i=0;
    BasicInfos<TTA> &cur=content;
    for(int i=1;i<=cur.n;i++)
    {
        target_node_i=cur.pos[i];
        target_cur_i=i;
        if(val<cur.end[i]||val==cur.end[i]) break;
    }
    if(!target_node_i) return false;
    ListNode<TTA> opt_node;file_detailed.read(opt_node,target_node_i);
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
template<typename TTA>bool KVS<TTA>::erase(StringC index)
{
    if(empty()) return false;
    int target_node_i=0,target_cur_i=0;
    BasicInfos<TTA> &cur=content;
    for(int i=1;i<=cur.n;i++)
    {
        target_node_i=cur.pos[i];
        target_cur_i=i;
        if(index<cur.end[i]||index==cur.end[i]) break;
    }
    if(!target_node_i) return false;
    ListNode<TTA> opt_node;file_detailed.read(opt_node,target_node_i);
    int delete_pos=std::lower_bound(opt_node.val,opt_node.val+opt_node.n,index)-opt_node.val;
    if(delete_pos>=opt_node.n||opt_node.val[delete_pos]!=index) return false;
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
template<typename TTA>void KVS<TTA>::find(std::vector<TTA>&a,StringC index)
{
    int target_node_i=0;
    BasicInfos<TTA> &cur=content;
    for(int i=1;i<=cur.n;i++)
    {    
        target_node_i=cur.pos[i];
        if(cur.end[i]<index) {continue;}
        if(index<cur.first[i]) break;
        ListNode<TTA> opt_node;file_detailed.read(opt_node,target_node_i);
        for(int i=0;i<opt_node.n;i++)
            if(index==opt_node.val[i]) a.push_back(opt_node.val[i].value);
    }
}
template<typename TTA>bool KVS<TTA>::exist(StringC index)
{
    int target_node_i=0;bool flg=false;
    BasicInfos<TTA> &cur=content;
    for(int i=1;i<=cur.n;i++)
    {    
        target_node_i=cur.pos[i];
        if(cur.end[i]<index) {continue;}
        if(index<cur.first[i]) break;
        ListNode<TTA> opt_node;file_detailed.read(opt_node,target_node_i);
        for(int i=0;i<opt_node.n;i++)
            if(index==opt_node.val[i]) {return true;}
    }
    return false;
}

#endif