#include "AccountSystem.hpp"
#include "BookSystem.hpp"
#include "ConsolePanel.hpp"
#include <iostream>
#include <iomanip>

extern ConsolePanel H;
extern AccountSystem A;
extern int parseInt(const std::string&);
extern double parseFloat(const std::string&);

void BookSystem::pushStack()
{
    s.push(BookInfo());
}
void BookSystem::popStack()
{
    s.pop();
}
void BookSystem::updTop()
{
    s.pop();
    s.push(selected);
}
void splitKeyword(const StringC &kwd,std::vector<std::string>tar)
{
    //Waiting to Implement
}
BookSystem::BookInfo BookSystem::getBook(const StringC& ISBN)
{
    std::vector<BookInfo> g;
    ISBN_map.find(g,H[1]);
    return g[0];
}
void BookSystem::insertSelected()
{
    ISBN_map.insert(KaData<BookInfo>(selected.ISBN,selected));
    name_map.insert(KaData<BookInfo>(selected.bookname,selected));
    author_map.insert(KaData<BookInfo>(selected.author,selected));
    std::vector<std::string> kwdlist;
    splitKeyWord(selected.ISBN,kwdlist);
    for(auto kwd:kwdlist)
        kwd_map.insert(KaData<BookInfo>(kwd,selected));
}
void BookSystem::eraseSelected()
{
    ISBN_map.erase(KaData<BookInfo>(selected.ISBN,selected));
    name_map.erase(KaData<BookInfo>(selected.bookname,selected));
    author_map.erase(KaData<BookInfo>(selected.author,selected));
    std::vector<std::string> kwdlist;
    splitKeyWord(selected.ISBN,kwdlist);
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
        //Maybe not KVS because it may contain lots of books
        goto FINDBOOKOUTPUT;
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
        kwd_map.find(a,temp_tag.word);
    }
    else if(temp_tag.type==4) {H.invalidOperation(343);return ;}
    FINDBOOKOUTPUT:
    std::sort(a.begin(),a.end());
    if(a.empty()){std::cout<<std::endl;}
    else
    {
        for(int i=0;i<a.size();i++)
        {
            std::cout<<a[i].ISBN.toStr()<<"\t";
            std::cout<<a[i].bookname.toStr()<<"\t";
            std::cout<<a[i].keywords.toStr()<<"\t";
            std::cout<<a[i].keywords.toStr()<<"\t";
        }
    }
}

void BookSystem::buyBook()
{
    if(!A.privilegeEnable(1)) {H.invalidOperation(801);return ;}
    if(H.size()!=3) {H.invalidOperation(802);return ;}
    if(!validISBN(H[1])) {H.invalidOperation(803);return ;}
    int purchase_quantity=parseInt(H[2]);
    if(purchase_quantity==-1) {H.invalidOperation(804);return ;}
    BookInfo selected_origin=selected;
    selected=getBook(H[1]);
    if(selected.quantity<purchase_quantity)
        {selected=selected_origin;H.invalidOperation(805);return ;}
    else
    {
        eraseSelected();
        selected.quantity-=purchase_quantity;
        insertSelected();
        std::cout<<std::fixed<<std::setprecision(2)<<selected.price*purchase_quantity<<std::endl;
        selected=selected_origin;
    }
}

void BookSystem::selectBook()
{
    if(!A.privilegeEnable(3)) H.invalidOperation(701);
    if(H.size()!=2) H.invalidOperation(702);
    if(!validISBN(H[1])) H.invalidOperation(703);
    selected=(StringC)H[1];
    if(!ISBN_map.exist(H[1])) insertSelected();
    else selected=getBook(H[1]);
}

void BookSystem::modifyBook()
{
    //Waiting For Implementation.
}

void BookSystem::importBook()
{
    if(!A.privilegeEnable(3)) H.invalidOperation(901);
    if(H.size()!=3) H.invalidOperation(902);
    int p_quant=parseInt(H[1]);double p_totalcost=parseInt(H[2]);
    if(!parseInt(H[1])) H.invalidOperation(903);
    if(!parseFloat(H[2])) H.invalidOperation(904);
    //看不懂这要干什么。那就先不写。
}