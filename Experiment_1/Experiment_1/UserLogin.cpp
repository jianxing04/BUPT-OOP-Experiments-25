#include"UserLogin.h"

UserLogin::UserLogin(const string& path) : filePath(path) {
    loadUsers();
}
void UserLogin::loadUsers() {
    ifstream file(filePath);
    if (file.is_open()) {
        file >> users;
        file.close();
    }
}
void UserLogin::saveUsers() {
    ofstream file(filePath);
    if (file.is_open()) {
        file << setw(4) << users;
        file.close();
    }
}
bool UserLogin::isUsernameExists(const string& username) {
    for (const auto& user : users) {
        if (user["username"] == username) {
            return true;
        }
    }
    return false;
}
bool UserLogin::validateUser(const string& username, const string& password) {
    for (const auto& user : users) {
        if (user["username"] == username && user["password"] == password) {
            return true;
        }
    }
    return false;
}
int UserLogin::getUserId(const string& username) {
    for (const auto& user : users) {
        if (user["username"] == username) {
            return user["id"];
        }
    }
}
json UserLogin::login() {
    string username, password;
    json user;
    while (true) {
        cout << "请输入用户名: ";
        while (true) {
            getline(cin, username);
            if (!username.empty()) break;
            cout << "用户名不能为空！请重新输入：";
        }
        if (!isUsernameExists(username)) {
            cout << "用户名不存在，请重新输入。\n";
            continue;
        }
        while (true) {
            cout << "请输入密码: ";
            while (true) {
                getline(cin, password);
                if (!password.empty()) break;
                cout << "密码不能为空！请重新输入：";
            }
            if (validateUser(username, password)) {
                cout << "登录成功！\n";
				for (const auto& u : users) {
					if (u["username"] == username) {
						user = u;
						break;
					}
				}
                return user;
            }
            else {
                cout << "密码不正确，请重新输入。\n";
            }
        }
    }
}
void UserLogin::registerUser() {
    string username, password;
    int usertype;
    while (true) {
        cout << "请输入要注册的用户名: ";
        while (true) {
            getline(cin, username);
            if (!username.empty()) break;
            cout << "用户名不能为空！请重新输入：";
        }
        if (isUsernameExists(username)) {
            cout << "该用户名已存在，请重新输入。\n";
        }
        else {
            break;
        }
    }
    cout << "请输入密码: ";
    while (true) {
        getline(cin, password);
        if (!password.empty()) break;
        cout << "密码不能为空！请重新输入：";
    }
    cout << "请输入用户类型（1-消费者，2-商家）: ";
    while (true) {
        cin >> usertype;
        if (cin.fail() || (usertype != 1 && usertype != 2)) {
            cin.clear(); // 清除错误状态
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清空输入缓冲区
            cout << "输入无效，请输入 1（消费者）或 2（商家）：";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除换行符
            break;
        }
    }
    json newUser;
    newUser["id"] = users.size() + 1;
    newUser["username"] = username;
    newUser["password"] = password;
    newUser["balance"] = 0.0;
    newUser["accountType"] = usertype;
    users.push_back(newUser);
    saveUsers();
    cout << "注册成功！自动返回登录界面。\n";
}
json UserLogin::mainMenu() {
    int choice;
    cout << "请选择操作：输入 1 是登录，输入 2 是注册: ";
    while (true) {
        cin >> choice;
        // 检查输入是否合法
        if (cin.fail() || (choice != 1 && choice != 2)) {
            cin.clear(); // 清除错误状态
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清空输入缓冲区
            cout << "输入无效，请输入 1（登录）或 2（注册）：";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除换行符
            break;
        }
    }
    if (choice == 2) {
        registerUser();
    }
    return login();
}