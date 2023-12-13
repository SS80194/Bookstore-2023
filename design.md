# 设计文档

## 一、 功能设计

### 解码器
- **实现思路**：获得命令后，如果检测到关键词，则打开相对应的函数处理接下来的操作，运行完成后解除嵌套，继续从输入命令的行里读取。如果检测关键词失败，则报错。

### 操作函数
定义统一格式：
- **输入数据**： 一个字符串指针，指向当前位置。
- **返回值**：一个字符串指针，指向当前指向命令的位置。 

详见 functions.md

## 二、 类设计

### Book 类
指一种书。

包含：
1. (string) ISBN
2. (string) name 书名
2. (string) author 作者
3. (vector\<string\>) keyword 关键词
4. (int) quantity 图书数量
5. (double) price 图书单价
6. (double) total_cost 交易总额

### User 类
指一个用户。

包含：
1. (string) user_name
2. (string) password
3. (int) adlevel 权限等级
4. (int) pre 它之前一个登录的用户

## 三、 存储文件设计

### 后缀名与文件类型
1. .mp 文件。表示在 A2074 中实现过的 map-int 类型的映射
2. .fa 文件。表示 file_array。
3. .sfa 文件 表示基于 file_array 实现的 stack

### 文件列表
#### 1. commandsmenu.mp
**只读。**
将各种命令名称映射到int上。工程文件自带。

#### 2. accountstack.sa
记录当前登录的所有账号。

#### 3. 

## 四、 重要的内存全局变量设计

### string s-line
当前命令行。

### vector\<int(*)(int)\> commands[]
命令名映射到一个函数指针。

### int current_userid;User current_user;

## 五、 无关函数

#### 1. int jumpvoid(int)
功能：跳过当前所有的空格，直到下一个非空或本行末尾。返回跳到的位置。

#### 2. int 