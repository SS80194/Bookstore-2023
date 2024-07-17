#include"StringC.hpp"

const int StringC::max_length=64;
StringC::StringC(char *tar_c)
{
    int len=0;
    for(int i=0;i<=max_length&&tar_c[i]!=0;i++)
        len=i,s[i]=tar_c[i];
    s[len+1]=0;
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
bool StringC::validSetA()//Only contains numbers,alphabets,and underlines
{
    printf("%s\n",s);
    for(int i=0;i<=max_length;i++)
    {
        if(s[i]==0) break;
        printf("%d ",s[i]);
        bool flg=false;
        if(isNumber(s[i])||isLetter(s[i])||s[i]=='_')
            flg=true;
        if(!flg) return false;
    }
    return true;
}