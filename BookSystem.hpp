#ifndef Bookstore_BookSystem_HPP
#define Bookstore_BookSystem_HPP

#include<vector>
#include"ConsolePanel.hpp"
#include"StringC.hpp"
#include "AccountSystem.hpp"
#include"KeyValueStorage.hpp"

class BookSystem
{
    public:
    class BookInfo
    {
        public:
        StringC ISBN,bookname,author,keywords;
        int quantity;
        double price,totalcost;
        public:
        BookInfo()=default;
        BookInfo(StringC ISBN_res):ISBN(ISBN_res){};
        BookInfo(const BookInfo &res){(*this)=res;};
        BookInfo(const StringC &ISBN_res,const StringC &name_res,const StringC &author_res,const StringC &keywords_res,int quantity_res,double price_res,double totalcost_res)
        :ISBN(ISBN_res),bookname(name_res),author(author_res),keywords(keywords_res),quantity(quantity_res),price(price_res),totalcost(totalcost_res){};
        bool operator<(const BookInfo &res){return ISBN<res.ISBN;}
    };
    private:
    BookInfo selected;
    KVS<BookInfo>ISBN_map{"ISBN_order.mr"};
    KVS<BookInfo>name_map{"name_order.mr"};
    KVS<BookInfo>author_map{"author_order.mr"};
    KVS<BookInfo>kwd_map{"kwd_order.mr"};
    std::stack<BookInfo> s;
    private:
    bool validISBN(const std::string&);
    bool validBookname(const std::string&);
    bool validAuthor(const std::string&);
    bool validKeyWord(const std::string&);
    void splitKeyWord(const StringC&,std::vector<std::string>&);
    void insertSelected();
    void eraseSelected();
    public:
    void pushStack();
    void popStack();
    void updTop(); 
    BookInfo getBook(const StringC&);
    void findBook();
    void buyBook();
    void selectBook();
    void modifyBook();
    void importBook();
};


#endif