#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <iomanip>
using namespace std;
using json = nlohmann::json;

class UserLogin {
private:
    json users;
    string filePath;
    void loadUsers();// 从文件中加载用户数据
    void saveUsers();// 将用户数据保存到文件中
    bool isUsernameExists(const string& username);// 检查用户名是否存在
    bool validateUser(const string& username, const string& password);// 验证用户名和密码
    int getUserId(const string& username);
    json login();// 登录功能
    void registerUser();// 注册功能

public:
    UserLogin(const string& path);
    json mainMenu();// 主菜单
};