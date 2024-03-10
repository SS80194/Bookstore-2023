#ifndef Bookstore_AccountSystem_HPP
#define Bookstore_AccountSystem_HPP

#include<stack>
#include<vector>
#include"ConsolePanel.hpp"
#include"StringC.hpp"
#include"KeyValueStorage.hpp"

class AccountData
{
    public:
    StringC user_id,user_name,passwd;
    int privilege;
    AccountData()=default;
    AccountData(const StringC& ori_id,const StringC& ori_passwd,int ori_privilege,const StringC& ori_name)
    {
        user_id=ori_id;passwd=ori_passwd;privilege=ori_privilege;user_name=ori_name;
    }
};
class AccountSystem
{
    private:
    bool validID(StringC);
    bool validPasswd(StringC);
    bool validUsername(StringC);
    bool validPrivilege(StringC);
    int convertPrivilege(StringC);
    std::stack<AccountData>s;
    std::map<StringC,int>mp;
    KVS<AccountData>Account_map{"Account.mr"};
    public:
    bool privilegeEnable(int);
    void quit();
    void logout();
    void su();
    void signup();
    void signupPro();
    void changePasswd();
    void deleteAccount();
};

//下面的可以搬到Accountsystem.cpp里边
extern ConsolePanel H;
bool AccountSystem::validID(StringC s)
{
    return s.strlen()<=30&&s.validSetA();
}
bool AccountSystem::validPasswd(StringC s)
{
    return s.strlen()<=30&&s.validSetA();
}
bool AccountSystem::validUsername(StringC s)
{
    return s.strlen()<=30;
}
bool AccountSystem::validPrivilege(StringC s)
{
    if(s.strlen()!=1) return 0;
    int x=s[0];return (x=='0'||x=='1'||x=='3'||x=='7');
}
int AccountSystem::convertPrivilege(StringC s)
{
    int x=s[0];return x-'0';
}
bool AccountSystem::privilegeEnable(int x)
{
    if(!x) return true;
    if(s.empty()) return false;
    return s.top().privilege>=x;
}
void AccountSystem::quit()
{
    if(!privilegeEnable(1)) {H.invalidOperation();return ;};
    if(H.size()>1) {H.invalidOperation();return ;}
    else exit(0);
}
void AccountSystem::su()
{
    if(!privilegeEnable(0)) {H.invalidOperation();return ;}
    int cur_privilege=(s.empty()?0:s.top().privilege);
    if(H.size()<2||H.size()>3) {H.invalidOperation();return ;}
    StringC userID_input=H[1],pwd_input;//current password,new password
    if(!validID(userID_input)||!Account_map.exist(userID_input)) {H.invalidOperation();return ;}; 
    std::vector<AccountData>A;Account_map.find(A,userID_input);
    AccountData target_user=A[0];
    if(H.size()==2) 
    {
        if(cur_privilege<=target_user.privilege) {H.invalidOperation();return ;}
    }
    else if(H.size()==3)
    {
        pwd_input=H[2];
        if(!validPasswd(pwd_input)||pwd_input!=target_user.passwd) {H.invalidOperation();return ;}
    }
    mp[userID_input]++;s.push(target_user);
}
void AccountSystem::logout()
{
    if(!privilegeEnable(1)) {H.invalidOperation();return ;}
    if(H.size()>1) {H.invalidOperation();return ;}
    if(s.empty()) {H.invalidOperation();return ;}
    else mp[s.top().user_id]--,s.pop();
}
void AccountSystem::signup()
{
    if(!privilegeEnable(0)) {H.invalidOperation();return ;}
    if(H.size()!=4) {H.invalidOperation();return ;}
    if(!validID(H[1])||!validPasswd(H[2])||!validUsername(H[3])) {H.invalidOperation();return ;};
    if(Account_map.exist(H[1])) {H.invalidOperation();return ;};;
    AccountData new_ac(H[1],H[2],1,H[3]);
    Account_map.insert(KaData<AccountData>(H[1],new_ac));
}
void AccountSystem::changePasswd()
{
    if(!privilegeEnable(1)) {H.invalidOperation();return ;}
    StringC userID_input,curpwd_input,newpwd_input;//current password,new password
    AccountData old_data;
    if(H.size()==3) 
    {
        if(!privilegeEnable(7)) {H.invalidOperation();return ;}
        userID_input=H[1];newpwd_input=H[2];
        if(!validID(userID_input)||!validPasswd(newpwd_input)) {H.invalidOperation();return ;}
        std::vector<AccountData>A;Account_map.find(A,userID_input);
        if(A.empty()) {H.invalidOperation();return ;}
        else old_data=A[0];
    }
    else if(H.size()==4)
    {
        userID_input=H[1],curpwd_input=H[2],newpwd_input=H[3];
        if(!validID(userID_input)||!validPasswd(newpwd_input)) {H.invalidOperation();return ;}
        std::vector<AccountData>A;Account_map.find(A,userID_input);
        if(A.empty()) {H.invalidOperation();return ;}
        else
        {
            if(A[0].passwd!=curpwd_input) {H.invalidOperation();return ;}
            old_data=A[0];
        }
    }
    else {H.invalidOperation();return ;}
    AccountData new_data;new_data.passwd=newpwd_input;
    Account_map.erase(KaData(userID_input,old_data));
    Account_map.insert(KaData(userID_input,new_data));
}
void AccountSystem::signupPro()
{
    if(!privilegeEnable(3)) {H.invalidOperation();return ;}
    if(H.size()!=5) {H.invalidOperation();return ;}
    StringC userID_input=H[1],pwd_input=H[2],pri_input=H[3],name_input=H[4];
    if(!validID(userID_input)||!validPasswd(pwd_input)||!validUsername(name_input)) {H.invalidOperation();return ;};
    if(!validPrivilege(pri_input)){H.invalidOperation();return ;}
    if(Account_map.exist(userID_input)) {H.invalidOperation();return ;};
    if(!privilegeEnable(convertPrivilege(pri_input))) {H.invalidOperation();return ;};
    AccountData new_ac(userID_input,pwd_input,convertPrivilege(pri_input),name_input);
    Account_map.insert(KaData<AccountData>(userID_input,new_ac));
}
void AccountSystem::deleteAccount()
{
    if(!privilegeEnable(7)) {H.invalidOperation();return ;}
    if(H.size()!=2) {H.invalidOperation();return ;}
    StringC userID_input=H[1];
    if(!validID(userID_input)||!Account_map.exist(userID_input)) {H.invalidOperation();return ;};
    if(mp[userID_input]>0) {H.invalidOperation();return ;}
    Account_map.erase(userID_input);
    
}
#endif