
//下面的可以搬到Accountsystem.cpp里边
#include<stack>
#include<vector>
#include"ConsolePanel.hpp"
#include"StringC.hpp"
#include"KeyValueStorage.hpp"
#include"AccountSystem.hpp"
#include"BookSystem.hpp"

//About AccountData
bool operator<(AccountData x,AccountData y){return x.user_id<y.user_id;}
bool operator==(AccountData x,AccountData y){return x.user_id==y.user_id;}

//About AccountSystem
extern ConsolePanel H;
extern BookSystem B;
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
    if(s.empty()) return 0>=x;
    return s.top().privilege>=x;
}
void AccountSystem::initAccount()
{
    //create an account with id=root & passwd=sjtu
    if(Account_map.exist("root")) return ;
    StringC userID_input="root",pwd_input="sjtu",pri_input="7",name_input="root";
    if(!validID(userID_input)||!validPasswd(pwd_input)||!validUsername(name_input)) {H.invalidOperation();return ;};
    if(!validPrivilege(pri_input)){H.invalidOperation();return ;}
    AccountData new_ac(userID_input,pwd_input,convertPrivilege(pri_input),name_input);
    Account_map.insert(KaData<AccountData>(userID_input,new_ac));

}
void AccountSystem::quit()
{
    if(H.size()>1) {H.invalidOperation(502);return ;}
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
    //std::cout<<"A.size="<<A.size()<<std::endl;
    if(H.size()==2) 
    {
        if(cur_privilege<=target_user.privilege) {H.invalidOperation();return ;}
    }
    else if(H.size()==3)
    {
        pwd_input=H[2];
        if(!validPasswd(pwd_input)||pwd_input!=target_user.passwd) {H.invalidOperation();return ;}
    }
    //std::cout<<"S:"<<userID_input.toStr()<<" + "<<target_user.user_id.toStr()<<" "<<target_user.privilege<<" "<<mp[userID_input]<<std::endl;
    mp[userID_input]++;
    s.push(target_user);B.pushStack();
}
void AccountSystem::logout()
{
    if(!privilegeEnable(1)) {H.invalidOperation();return ;}
    if(H.size()>1) {H.invalidOperation();return ;}
    //std::cout<<"L:"<<s.top().user_id.toStr()<<" "<<mp[s.top().user_id]<<std::endl;
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
    AccountData new_data=old_data;new_data.passwd=newpwd_input;
    Account_map.erase(KaData(userID_input,old_data));
    Account_map.insert(KaData(userID_input,new_data));
}
void AccountSystem::signupPro()
{
    if(!privilegeEnable(3)) {H.invalidOperation();return ;}
    if(H.size()!=5) {H.invalidOperation();return ;}
    StringC userID_input=H[1],pwd_input=H[2],pri_input=H[3],name_input=H[4];
    if(!validID(userID_input)||!validPasswd(pwd_input)||!validUsername(name_input)) {H.invalidOperation(1);return ;};
    if(!validPrivilege(pri_input)){H.invalidOperation(2);return ;}
    if(Account_map.exist(userID_input)) {H.invalidOperation(3);return ;};
    if(!privilegeEnable(convertPrivilege(pri_input)+1)) {H.invalidOperation(4);return ;};
    AccountData new_ac(userID_input,pwd_input,convertPrivilege(pri_input),name_input);
    Account_map.insert(KaData<AccountData>(userID_input,new_ac));
}
void AccountSystem::deleteAccount()
{
    if(!privilegeEnable(7)) {H.invalidOperation(191);return ;}
    if(H.size()!=2) {H.invalidOperation(192);return ;}
    StringC userID_input=H[1];
    //std::cout<<"D:"<<H[1]<<" + "<<userID_input.toStr()<<" "<<mp[userID_input]<<std::endl;
    if(!validID(userID_input)||!Account_map.exist(userID_input)) {H.invalidOperation(193);return ;};
    if(mp[userID_input]>0) {H.invalidOperation(mp[userID_input]);return ;}
    Account_map.erase(userID_input);
    
}
