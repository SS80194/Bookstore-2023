#include "AccountSystem.hpp"
#include "BookSystem.hpp"
#include "LogSystem.hpp"
#include "ConsolePanel.hpp"
#include <iostream>
#include <iomanip>

extern ConsolePanel H;
extern AccountSystem A;
extern LogSystem L;
extern int parseInt(const std::string&);
extern double parseFloat(const std::string&);

void BookSystem::pushStack()
{
    s.push(BookInfo());
    selected=BookInfo();
}
void BookSystem::popStack()
{
    s.pop();
    //selected=s.empty()?BookInfo():s.top();
    if(s.empty()) selected=BookInfo();
    else
    {
        selected=s.top();
        std::string selected_id=std::to_string(selected.id);
        std::vector<StringC>a_temp;
        StringC ISBN_new;id_map.find(a_temp,selected_id);
        if(!a_temp.size()) selected=BookInfo();
        else
        {
            ISBN_new=a_temp[0];
            selected=getBook(ISBN_new);
        }
        
    }
}
void BookSystem::updTop()
{
    s.pop();
    s.push(selected);
}
bool BookSystem::validISBN(const std::string &s)
{
    if(s.size()>20) return false;
    return true;
}
bool BookSystem::validBookname(const std::string&s)
{
    if(s.size()>60) return false;
    for(int i=0;i<s.size();i++)
        if(s[i]=='"') return false;
    return true;
}
bool BookSystem::validAuthor(const std::string &s)
{
    if(s.size()>60) return false;
    for(int i=0;i<s.size();i++)
        if(s[i]=='"') return false;
    return true;
}
bool BookSystem::validKeyWord(const std::string&s)
{
    if(s.size()>60) return false;
    for(int i=0;i<s.size();i++)
    {
        if(s[i]=='"') return false;
        if(i>0&&s[i]=='|'&&s[i-1]=='|') return false;
    }
    if(s[s.size()-1]=='|') return false;
    return true;    
}
void BookSystem::splitKeyWord(StringC &kwd,std::vector<std::string>&tar)
{
    //按 | 分割
    std::string temp;tar.clear();
    for(int i=0;i<StringC::max_length;i++)
    {
        if(kwd[i]=='|'||kwd[i]==0) tar.push_back(temp),temp.clear();
        if(kwd[i]==0) break;
        else if(kwd[i]!='|') temp+=kwd[i];
    }
    sort(tar.begin(),tar.end());

}
BookSystem::BookInfo BookSystem::getBook(const StringC& ISBN)
{
    std::vector<BookInfo> g;
    ISBN_map.find(g,ISBN);
    return g[0];
}
void BookSystem::insertSelected()
{
    ISBN_map.insert(KaData<BookInfo>(selected.ISBN,selected));
    name_map.insert(KaData<BookInfo>(selected.bookname,selected));
    author_map.insert(KaData<BookInfo>(selected.author,selected));
    id_map.insert(KaData<StringC>(std::to_string(selected.id),selected.ISBN));
    std::vector<std::string> kwdlist;
    splitKeyWord(selected.keywords,kwdlist);
    for(auto kwd:kwdlist)
        kwd_map.insert(KaData<BookInfo>(kwd,selected));
}
void BookSystem::eraseSelected()
{
    ISBN_map.erase(KaData<BookInfo>(selected.ISBN,selected));
    name_map.erase(KaData<BookInfo>(selected.bookname,selected));
    author_map.erase(KaData<BookInfo>(selected.author,selected));
    id_map.erase(KaData<StringC>(std::to_string(selected.id),selected.ISBN));
    std::vector<std::string> kwdlist;
    splitKeyWord(selected.keywords,kwdlist);
    for(auto kwd:kwdlist)
        kwd_map.erase(KaData<BookInfo>(kwd,selected));
}

void BookSystem::findBook()
{
    if(!A.privilegeEnable(1)) {H.invalidOperation(301);return ;}
    std::vector<BookInfo> a;tagInfo temp_tag;
    if(H.size()==1)
    {
        //show all books;Waiting for implementation in KVS
        ISBN_map.findAll(a);
        goto FINDBOOKOUTPUT;
    }
    if(H[1]=="finance")
    {
        L.showFinance();
        return ;
    }
    else if(H.size()!=2) {H.invalidOperation(302);return ;}
    temp_tag=H.parseTag(H[1]);
    if(temp_tag.type<0) {H.invalidOperation(303);return ;}
    if(temp_tag.type==0)
    {
        if(!validISBN(temp_tag.word)) H.invalidOperation(330);
        ISBN_map.find(a,temp_tag.word);
    }
    else if(temp_tag.type==1)
    {
        if(!validBookname(temp_tag.word)) H.invalidOperation(331);
        name_map.find(a,temp_tag.word);
    }
    else if(temp_tag.type==2)
    {
        if(!validAuthor(temp_tag.word)) H.invalidOperation(332);
        author_map.find(a,temp_tag.word);
    }
    else if(temp_tag.type==3)
    {
        if(!validKeyWord(temp_tag.word)) H.invalidOperation(333);
        std::vector<std::string> kwdlist;
        StringC temp_kwds=temp_tag.word;
        splitKeyWord(temp_kwds,kwdlist);
        for(auto kwd:kwdlist) kwd_map.find(a,kwd);

    }
    else if(temp_tag.type==4) {H.invalidOperation(343);return ;}
    FINDBOOKOUTPUT:
    std::sort(a.begin(),a.end());
    a.erase(std::unique(a.begin(), a.end()), a.end());
    if(a.empty()){std::cout<<std::endl;}
    else
    {
        for(int i=0;i<a.size();i++)
        {
            std::cout<<a[i].ISBN.toStr()<<"\t";
            std::cout<<a[i].bookname.toStr()<<"\t";
            std::cout<<a[i].author.toStr()<<"\t";
            std::cout<<a[i].keywords.toStr()<<"\t";
            std::cout<<std::fixed<<std::setprecision(2)<<a[i].price<<"\t";
            std::cout<<a[i].quantity;
            std::cout<<std::endl;
        }
    }
}

void BookSystem::buyBook()
{
    if(!A.privilegeEnable(1)) {H.invalidOperation(801);return ;}
    if(H.size()!=3) {H.invalidOperation(802);return ;}
    if(!validISBN(H[1])) {H.invalidOperation(803);return ;}
    int purchase_quantity=parseInt(H[2]);
    if(purchase_quantity==-1||purchase_quantity==0) {H.invalidOperation(804);return ;}
    if(!ISBN_map.exist(StringC(H[1]))) return H.invalidOperation(870);
    BookInfo selected_origin=selected;
    selected=getBook(H[1]);
    if(selected.quantity<purchase_quantity)
        {selected=selected_origin;return H.invalidOperation(805);}
    else
    {
        eraseSelected();
        selected.quantity-=purchase_quantity;
        insertSelected();
        std::cout<<std::fixed<<std::setprecision(2)<<selected.price*purchase_quantity<<std::endl;
        L.AddIncome(selected.price*purchase_quantity);
        selected=selected_origin;
    }
}

void BookSystem::selectBook()
{
    if(!A.privilegeEnable(3)) {H.invalidOperation(701);return ;}
    if(H.size()!=2) {H.invalidOperation(702);return ;}
    if(!validISBN(H[1])) return H.invalidOperation(703);
    selected=(StringC)H[1];
    if(!ISBN_map.exist(H[1]))//Not Exist this ISBN.
    {
        //Create A new Book.
        int current_tot=id_map.info();
        current_tot++;
        id_map.setInfo(current_tot);
        selected.id=current_tot;
        insertSelected();
    }
    else selected=getBook(H[1]);
    updTop();
}

void BookSystem::modifyBook()
{
    if(!A.privilegeEnable(3)) return H.invalidOperation(401);
    if(H.size()<2) return H.invalidOperation(402);
    std::vector<std::string>Q;Q.resize(5);
    for(int i=1;i<H.size();i++)
    {
        tagInfo temp_tag=H.parseTag(H[i]);
        if(temp_tag.type==-1) return H.invalidOperation(410);
        if(!Q[temp_tag.type].empty()) return H.invalidOperation(411);
        Q[temp_tag.type]=temp_tag.word;
    }
    //From 1 to 4 Modify!
    if(selected.ISBN.toStr().empty()) return H.invalidOperation(420);
    //不存在选中的图书
    BookInfo mdfed_book=selected;
    if(!Q[0].empty())//Not Empty ISBN
    {
        if(!validISBN(Q[0])) return H.invalidOperation(501);
        if(ISBN_map.exist(Q[0])) return H.invalidOperation(502);
        mdfed_book.ISBN=Q[0];
    }
    if(!Q[1].empty())//Not Empty BookName
    {
        if(!validBookname(Q[1])) return H.invalidOperation(511);
        mdfed_book.bookname=Q[1];
    }
    if(!Q[2].empty())//Not Empty Author
    {
        if(!validAuthor(Q[2])) return H.invalidOperation(521);
        mdfed_book.author=Q[2];
    }
    if(!Q[3].empty())//Not Empty Keyword
    {
        if(!validKeyWord(Q[3])) return H.invalidOperation(531);
        std::vector<std::string> kwd_tmp;
        StringC temp(Q[3]);
        splitKeyWord(temp,kwd_tmp);

        for(int i=1;i<kwd_tmp.size();i++)
            if(kwd_tmp[i]==kwd_tmp[i-1]) return H.invalidOperation(532);
        mdfed_book.keywords=Q[3];
    }
    if(!Q[4].empty())//Not Empty Price
    {
        double target_price=parseFloat(Q[4]);
        if(target_price<0) return H.invalidOperation(541);
        mdfed_book.price=target_price;
    }
    eraseSelected();
    selected=mdfed_book;
    insertSelected();
    updTop();
}

void BookSystem::importBook()
{
    if(!A.privilegeEnable(3)) return H.invalidOperation(901);
    if(selected.ISBN.toStr().empty()) return H.invalidOperation(913);
    if(H.size()!=3) return H.invalidOperation(902);
    int p_quant=parseInt(H[1]);double p_totalcost=parseInt(H[2]);
    if(parseInt(H[1])<=0) return H.invalidOperation(903);
    if(parseFloat(H[2])<=0) return H.invalidOperation(904);
    //可能，这里的逻辑有一些问题。
    BookInfo mdfed_book=selected;
    eraseSelected();
    mdfed_book.totalcost+=parseFloat(H[2]);
    L.AddCost(parseFloat(H[2]));
    mdfed_book.quantity+=parseInt(H[1]);
    selected=mdfed_book;
    insertSelected();
    updTop();
}