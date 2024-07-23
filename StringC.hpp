#ifndef Bookstore_StringC
#define Bookstore_StringC 

#include<cstring>
#include<string>
#include<vector>

typedef unsigned long long ull;
class StringC
{
    private:
    char s[68];
    bool isLetter(char c){return ('a'<=c&&c<='z')||('A'<=c&&c<='Z');}
    bool isNumber(char c){return ('0'<=c&&c<='9');}
    public:
    const static int max_length;
    StringC() {s[0]=0;};
    StringC(const char *);
    StringC(std::string);
    ull toHash();
    std::string toStr();
    char& operator[](int k) {return (k<max_length?s[k]:s[max_length]);}
    int strlen() const;
    bool validSetA();
    friend bool operator<(StringC,StringC);
    friend bool operator==(StringC,StringC);
    friend bool operator!=(StringC a,StringC b){return !(a==b);}
    friend void splitKeyword(const StringC&,std::vector<std::string>&);
    //friend int parseInt(const StringC&);
};

#endif