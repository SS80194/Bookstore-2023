#ifndef Bookstore_StringC
#define Bookstore_StringC 

#include<cstring>
#include<string>

typedef unsigned long long ull;
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

#endif