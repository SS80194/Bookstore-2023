#ifndef Bookstore_AccountSystem_HPP
#define Bookstore_AccountSystem_HPP

#include<stack>
#include<vector>
#include<map>
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
    std::map<StringC,int>mp;//mp用来统计这个账户登陆的此书
    KVS<AccountData>Account_map{"Account.mr"};
    public:
    bool privilegeEnable(int);
    void initAccount();
    void quit();
    void logout();
    void su();
    void signup();
    void signupPro();
    void changePasswd();
    void deleteAccount();
};

#endif