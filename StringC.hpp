#ifndef Bookstore_StringC
#define Bookstore_StringC 

#include<cstring>
#include<string>

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
    StringC(char *);
    StringC(std::string);
    ull toHash();
    char& operator[](int k){return (k<max_length?s[k]:s[max_length]);}
    int strlen();
    bool validSetA();
    friend bool operator<(StringC,StringC);
    friend bool operator==(StringC,StringC);
    friend bool operator!=(StringC a,StringC b){return !(a==b);}
};

#endif