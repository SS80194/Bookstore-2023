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
int StringC::strlen()
{
    for(int i=0;i<=max_length;i++)
        if(!s[i]) return i;
    return max_length;
}
bool StringC::validSetA()
{
    for(int i=0;i<=max_length;i++)
    {
        if(s[i]==0) break;
        bool flg=false;
        if(isNumber(s[i])||isLetter(s[i])||s[i]=='_')
            flg=true;
        if(!flg) return false;
    }
    return true;
}
#endif