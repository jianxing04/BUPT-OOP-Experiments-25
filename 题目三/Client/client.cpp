#define NOMINMAX  // Prevents defining min and max macros
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <nlohmann/json.hpp>
#include <windows.h>
#include <limits>  // 确保包含此头文件
#pragma comment(lib, "ws2_32.lib")

using namespace std;
using json = nlohmann::json;

json currentUser; // 全局变量，当前用户信息

SOCKET clientSocket; // 全局变量，客户端套接字
// 基本操作类
class basicOperations {
public:
    // 发送请求并接收响应
    json sendRequest(const json& request)const {
        // 发送请求
        string requestStr = request.dump();
        send(clientSocket, requestStr.c_str(), requestStr.length(), 0);
        // 接收响应
        char buffer[4096];
        int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (recvSize > 0) {
            buffer[recvSize] = '\0';
            json response = json::parse(buffer);
            return response;
        }
		else {
            json rt;
			rt["status"] = "error";
			rt["message"] = "接收数据失败";
			return rt;
		}
    }
    // 注册账号
    void registerAccount()const {
        string userName, password;
        int accountType;
        cout << "请输入用户名：";
        while (true) {
            if (getline(cin, userName) && !userName.empty()) {
                break;
            }
            cout << "用户名不能为空，请重新输入：";
        }
        cout << "请输入密码：";
        while (true) {
            if (getline(cin, password) && !password.empty()) {
                break;
            }
            cout << "密码不能为空，请重新输入：";
        }
        cout << "输入账户类型：1-消费者，2-商家：";
        while (true) {
            if (cin >> accountType && (accountType == 1 || accountType == 2)) {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "输入无效，请输入 1 或 2。\n";
        }
        json request;
        request["action"] = "register";
        request["username"] = userName;
        request["password"] = password;
        request["accounttype"] = accountType;
        json response = sendRequest(request);
		if (response["status"] == "success") {
			cout << "注册成功！" << endl;
		}
		else {
			cout << "注册失败：" << response["message"] << endl;
		}
    }
    // 登录账号
    bool loginAccount()const {
        string userName, userPassword;
        cout << "输入账号名：";
        while (true) {
            if (getline(cin, userName) && !userName.empty()) {
                break;
            }
            cout << "账号名不能为空，请重新输入：" ;
        }
        cout << "请输入密码：";
        while (true) {
            if (getline(cin, userPassword) && !userPassword.empty()) {
                break;
            }
            cout << "密码不能为空，请重新输入：" ;
        }
        json request;
        request["action"] = "login";
        request["username"] = userName;
        request["password"] = userPassword;
        json response = sendRequest(request);
		if (response["status"] == "error") {
			cout << "登录失败：" << response["message"] << endl;
			return false;
		}
        cout << response["message"] << endl;
		currentUser = response["user"];
		cout << "欢迎 " << currentUser["username"] << "！" << endl;
        return true;
    }
    // 查看商品
    void viewMerchandise()const {
        json request;
        request["action"] = "viewMerchandise";
        json response = sendRequest(request);
        if (response["status"] == "success") {
            cout << "商品列表：" << endl;
			for (const auto& merchandise : response["merchandises"]) {
				cout << "商品ID: " << merchandise["id"] 
                    << ", 商品名称: " << merchandise["name"] 
                    << ", 商品价格: " << merchandise["price"] 
					<< ", 商品描述: " << merchandise["description"]
					<< ", 商品库存: " << merchandise["stock"]
                    << endl;
			}
        }
        else {
            cout << response["message"] << endl;
        }
    }
};
// 用户基类
class userBase {
public:
    virtual void getUserType()const = 0 {}
    virtual void interact()const = 0 {}
	// 退出登录
	void userLogout()const {
		json request;
		request["action"] = "logout";
		request["username"] = currentUser["username"];
		json response = bo.sendRequest(request);
		if (response["status"] == "success") {
			cout << "退出成功！" << endl;
			currentUser.clear(); // 清除当前用户信息
		}
		else {
			cout << "退出失败：" << response["message"] << endl;
		}
	}
	// 修改密码
    void changePassword()const {
		string newPassword;
		cout << "请输入新密码：";
		while (true) {
			if (getline(cin, newPassword) && !newPassword.empty()) {
				break;
			}
			cout << "密码不能为空，请重新输入：";
		}
		json request;
		request["action"] = "changePassword";
		request["username"] = currentUser["username"];
		request["newPassword"] = newPassword;
		json response = bo.sendRequest(request);
        if (response["status"] == "success") {
            cout << "密码修改成功！" << endl;
        }
		else {
			cout << "密码修改失败：" << response["message"] << endl;
		}
    }
	// 查看购物车
    void viewMerchandise()const {
        bo.viewMerchandise();
    }
	// 查询余额 
    void balanceInquiry()const {
        json request;
        request["action"] = "balanceInquiry";
        request["username"] = currentUser["username"];
        json response = bo.sendRequest(request);
        if (response["status"] == "success") {
            cout << "账户余额：" << response["balance"] << endl;
		}
        else {
            cout << response["message"] << endl;
        }
    }
	// 充值
    void recharge()const {
		double amount;
		cout << "请输入充值金额：";
		while (true) {
			if (cin >> amount && amount > 0) {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "输入无效，请输入一个正数：";
		}
		json request;
		request["action"] = "recharge";
		request["username"] = currentUser["username"];
		request["amount"] = amount;
		json response = bo.sendRequest(request);
        if (response["status"] == "success") {
            cout << "充值成功！" << endl;
        }
        else {
            cout << "充值失败：" << response["message"] << endl;
        }
    }
	// 搜索商品
    void searchForMerchandise()const {
        string merchandiseName;
        cout << "请输入商品名称：";
        while (true) {
            if (getline(cin, merchandiseName) && !merchandiseName.empty()) {
                break;
            }
            cout << "商品名不能为空，请重新输入：";
        }
        json request;
        request["action"] = "searchForMerchandise";
        request["merchandiseName"] = merchandiseName;
		json response = bo.sendRequest(request);
        if (response["status"] == "success") {
            cout << "搜索结果：" << endl;
            for (const auto& merchandise : response["results"]) {
				cout << "商品ID: " << merchandise["id"]
					<< ", 商品名称: " << merchandise["name"]
					<< ", 商品价格: " << merchandise["price"]
					<< ", 商品描述: " << merchandise["description"]
					<< ", 商品库存: " << merchandise["stock"]
					<< "，商品类别: " << merchandise["category"]
					<< endl;
            }
        }
        else {
			cout << "搜索失败：" << response["message"] << endl;
        }
    }
	basicOperations bo;
};
// 消费者类
class consumer : public userBase {
public:
	// 消费者交互函数
	void interact()const override {
        int choice;
        while (true) {
            cout << "\n*****************************************************************************\n";
            cout << "欢迎使用消费者功能！\n";
            cout << "1. 查看用户类型   2. 修改密码   3. 查看所有商品   4. 查询余额\n";
            cout << "5. 充值   6. 搜索商品   7. 向购物车增加商品   8. 查看我的购物车\n";
            cout << "9. 从购物车移除一件商品   10. 清空购物车   11. 根据购物车生成订单   12. 查看订单\n";
            cout << "13. 支付订单   14. 取消订单   其他.退出";
            cout << "\n*****************************************************************************\n";
            cout << "请输入你的选择：";
            while (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "输入无效，请输入一个数字：";
            }
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (choice == 1) {
                getUserType();
            }
            else if (choice == 2) {
                changePassword();
            }
			else if (choice == 3) {
				viewMerchandise();
			}
			else if (choice == 4) {
				balanceInquiry();
			}
			else if (choice == 5) {
				recharge();
			}
            else if (choice == 6) {
                searchForMerchandise();
            }
			else if (choice == 7) {
				addToShoppingTrolley();
			}
			else if (choice == 8) {
				viewMyShoppingTrolley();
			}
            else if (choice == 9) {
                removeFromShoppingTrolley();
            }
			else if (choice == 10) {
				clearShoppingTrolley();
			}
			else if (choice == 11) {
				generateOrder();
			}
            else if (choice == 12) {
                viewMyOrders();
            }
			else if (choice == 13) {
				payForTheOrder();
			}
            else if (choice == 14) {
                denyMyOrders();
            }
            else {
                cout << "感谢使用消费者功能！\n";
				userLogout(); // 退出登录
                break;
            }
        }
	}
	// 获取用户类型
	void getUserType()const override {
		cout << "用户类型：消费者" << endl;
	}
	// 向购物车增加商品
    void addToShoppingTrolley()const {
        int id;
		cout << "请输入商品id：";
        while (true) {
			if (cin >> id) {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "输入无效，请输入一个数字：";
        }
		json request;
		request["action"] = "addToShoppingTrolley";
		request["username"] = currentUser["username"];
        request["id"] = id;
		json response = bo.sendRequest(request);
        if (response["status"] == "success") {
            cout << "商品添加成功！" << endl;
        }
		else {
			cout << "添加失败：" << response["message"] << endl;
		}
    }
	// 查看我的购物车
    void viewMyShoppingTrolley()const {
		json request;
		request["action"] = "viewMyShoppingTrolley";
		request["username"] = currentUser["username"];
		json response = bo.sendRequest(request);
		if (response["status"] == "success") {
			cout << "购物车商品列表：" << endl;
			for (const auto& item : response["shoppingtrolley"]) {
				cout << "商品ID: " << item["id"]
					<< ", 商品名称: " << item["name"]
					<< ", 商品描述: " << item["description"]
					<< ", 商品类别: " << item["category"]
					<< ", 商品价格: " << item["price"]
					<< ", 商品库存: " << item["stock"]
					<< endl;
			}
		}
		else {
			cout << "查看失败：" << response["message"] << endl;
		}
    }
	// 从购物车移除商品
    void removeFromShoppingTrolley()const {
		int id;
		cout << "请输入商品id：";
		while (true) {
			if (cin >> id) {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "输入无效，请输入一个数字：";
		}
		json request;
		request["action"] = "removeFromShoppingTrolley";
		request["username"] = currentUser["username"];
		request["id"] = id;
		json response = bo.sendRequest(request);
        if (response["status"] == "success") {
            cout << "商品移除成功！" << endl;
        }
        else {
            cout << "移除失败：" << response["message"] << endl;
        }
    }
	// 清空购物车
    void clearShoppingTrolley()const {
		json request;
		request["action"] = "clearShoppingTrolley";
		request["username"] = currentUser["username"];
		json response = bo.sendRequest(request);
		if (response["status"] == "success") {
			cout << "购物车清空成功！" << endl;
		}
		else {
			cout << "清空失败：" << response["message"] << endl;
		}
    }
	// 根据购物车生成订单
    void generateOrder()const {
		json request1;
		request1["action"] = "viewMyShoppingTrolley";
		request1["username"] = currentUser["username"];
		json response1 = bo.sendRequest(request1);
		if (response1["status"] == "success") {//查看购物车
            json request2;
            request2["action"] = "balanceInquiry";
            request2["username"] = currentUser["username"];
            json response2 = bo.sendRequest(request2);
			if (response2["status"] == "success") {//查询余额
                json request3;
				request3["action"] = "generateOrder";
				request3["username"] = currentUser["username"];
                request3["orders"] = json::array();
                cout << "账户余额：" << response2["balance"] << endl;
				double balance = response2["balance"].get<double>();
				double totalPrice = 0;
                for (const auto& item : response1["shoppingtrolley"]) {
                    cout << "商品ID: " << item["id"]
                        << ", 商品名称: " << item["name"]
                        << ", 商品描述: " << item["description"]
                        << ", 商品类别: " << item["category"]
                        << ", 商品价格: " << item["price"]
                        << ", 商品库存: " << item["stock"]
                        << endl;
                    int count;
                    cout << "请输入购买数量：";
                    while (true) {
                        while (true) {
							if (cin >> count && count >= 0 ) {
								cin.ignore(numeric_limits<streamsize>::max(), '\n');
								break;
							}
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							cout << "输入无效，请输入一个非负数：";
                        }
						if (count > item["stock"].get<int>()) {
							cout << "库存不足，请重新输入购买数量：";
                            continue;
						}
						if (totalPrice + count * item["price"].get<double>() > balance) {
							cout << "余额不足，请重新输入购买数量：";
							continue;
						}
						break;
                    }
                    if (count > 0) {
                        totalPrice += count * item["price"].get<double>();
						json order;
						order["id"] = item["id"];
						order["name"] = item["name"];
                        order["count"] = count;
                        order["price"] = item["price"].get<double>() * count;
						order["ownerid"] = item["ownerid"];
						request3["orders"].push_back(order);
                    }
                }
				if (totalPrice > 0) {
					cout << "总金额：" << totalPrice << endl;
					request3["totalPrice"] = totalPrice;
					json response3 = bo.sendRequest(request3);
					if (response3["status"] == "success") {
						cout << "订单生成成功！" << endl;
					}
					else {
						cout << "订单生成失败：" << response3["message"] << endl;
					}
				}
				else {
					cout << "未选择商品，无法生成订单。" << endl;
					return;
				}
            }
            else {
                cout << "操作失败：" << response2["message"] << endl;
            }
        }
        else {
            cout << "操作失败：" << response1["message"] << endl;
        }
    }
	// 查看我的订单
    void viewMyOrders()const {
        json request;
        request["action"] = "viewMyOrders";
        request["username"] = currentUser["username"];
        json response = bo.sendRequest(request);
        if (response["status"] == "success") {
            cout << "订单列表：" << endl;
			int orderCount = 0;
            for (const auto& order : response["orders"]) {
				orderCount++;
				cout << "第 " << orderCount << " 个订单：" << endl;
				for (const auto& item : order) {
				    cout<< ", 商品ID: " << item["id"]
						<< ", 商品名称: " << item["name"]
						<< ", 商品总价: " << item["price"]
						<< ", 商品数量: " << item["count"]
						<< endl;
				}
            }
        }
        else {
            cout << "查看订单失败：" << response["message"] << endl;
        }
    }
	// 支付订单
    void payForTheOrder()const {
		json request_1;
		request_1["action"] = "viewMyOrders";
		request_1["username"] = currentUser["username"];
		json response_1 = bo.sendRequest(request_1);
		if (response_1["status"] != "success") {
			cout << "支付订单失败：" << response_1["message"] << endl;
			return;
		}
		json request;
		request["action"] = "payForTheOrder";
		request["username"] = currentUser["username"];
		int cnt = 0;
		int flg = 0;
		for (const auto&order :response_1["orders"]) {
			cnt++;
			cout << "第 " << cnt << " 个订单：" << endl;
			for (const auto& item : order) {
				cout << ", 商品ID: " << item["id"]
					<< ", 商品名称: " << item["name"]
					<< ", 商品总价: " << item["price"]
					<< ", 商品数量: " << item["count"]
					<< endl;
			}
			cout << "是否支付此订单？(y/n): ";
			char choice;
			while (true) {
				cin >> choice;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				if (choice == 'y' || choice == 'n') {
					break;
				}
				cout << "输入无效，请输入 'y' 或 'n': ";
			}
			if (choice == 'y') {
				flg = 1;
			}
			request["choices"].push_back(choice);
		}
		if (!flg) {
			cout << "未选择支付订单，操作取消。" << endl;
			return;
		}
		json response = bo.sendRequest(request);
        if (response["status"] == "success") {
            cout << "支付成功！" << endl;
        }
		else {
			cout << "支付失败：" << response["message"] << endl;
		}
    }
	// 取消订单
    void denyMyOrders()const {
		json request_1;
		request_1["action"] = "viewMyOrders";
		request_1["username"] = currentUser["username"];
		json response_1 = bo.sendRequest(request_1);
		if (response_1["status"] != "success") {
			cout << "取消订单失败：" << response_1["message"] << endl;
			return;
		}
		json request;
		request["action"] = "denyMyOrders";
		request["username"] = currentUser["username"];
		int cnt = 0;
		int flg = 0;
		for (const auto& order : response_1["orders"]) {
			cnt++;
			cout << "第 " << cnt << " 个订单：" << endl;
			for (const auto& item : order) {
				cout << ", 商品ID: " << item["id"]
					<< ", 商品名称: " << item["name"]
					<< ", 商品总价: " << item["price"]
					<< ", 商品数量: " << item["count"]
					<< endl;
			}
			cout << "是否取消此订单？(y/n): ";
			char choice;
			while (true) {
				cin >> choice;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				if (choice == 'y' || choice == 'n') {
					break;
				}
				cout << "输入无效，请输入 'y' 或 'n': ";
			}
			if (choice == 'y') {
				flg = 1;
			}
			request["choices"].push_back(choice);
		}
		if (!flg) {
			cout << "未选择取消订单，操作取消。" << endl;
			return;
		}
		json response = bo.sendRequest(request);
		if (response["status"] == "success") {
			cout << "取消订单成功！" << endl;
		}
        else {
            cout << "取消订单失败：" << response["message"] << endl;
        }
    }
};
// 商家类
class merchant : public userBase {
public:
	// 商家交互函数
	void interact()const override {
        int choice;
		while (true) {
			cout << "\n*****************************************************************************\n";
            cout << "欢迎使用商家功能！\n";
            cout << "1. 查看用户类型   2. 修改密码   3. 查看所有商品   4. 查询余额\n";
            cout << "5. 充值   6. 搜索商品   7. 添加一件商品   8. 移除商品\n";
			cout << "9. 修改产品价格   10. 补货   11. 按类别打折   12. 查看我的商品\n";
			cout << "13. 查看我的消息    其他.退出";
			cout << "\n*****************************************************************************\n";
			cout << "请输入你的选择：";
			while (!(cin >> choice)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "输入无效，请输入一个数字：";
			}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (choice == 1) {
				getUserType();
			}
			else if (choice == 2) {
				changePassword();
			}
			else if (choice == 3) {
				viewMerchandise();
			}
			else if (choice == 4) {
				balanceInquiry();
			}
            else if (choice == 5) {
                recharge();
            }
			else if (choice == 6) {
				searchForMerchandise();
			}
			else if (choice == 7) {
				addMerchandise();
			}
			else if (choice == 8) {
				removeMerchandise();
			}
			else if (choice == 9) {
				modifyMerchandisePrice();
			}
			else if (choice == 10) {
				stockpile();
			}
			else if (choice == 11) {
				discountByCategory();
			}
			else if (choice == 12) {
				viewMyMerchandise();
			}
			else if (choice == 13) {
				viewMyMessage();
			}
			else {
				cout << "感谢使用商家功能！\n";
				userLogout(); // 退出登录
				break;
			}
		}
	}
	// 获取用户类型
	void getUserType()const override {
		cout << "用户类型：商家" << endl;
	}
	// 添加商品
    void addMerchandise()const {
		string name, description, category;
		double price;
		int stock;
		cout << "请输入商品名称：";
		while (true) {
			if (getline(cin, name) && !name.empty()) {
				break;
			}
			cout << "商品名不能为空，请重新输入：";
		}
		cout << "请输入商品描述：";
		while (true) {
			if (getline(cin, description) && !description.empty()) {
				break;
			}
			cout << "商品描述不能为空，请重新输入：";
		}
		cout << "请输入商品类别：";
		while (true) {
			if (getline(cin, category) && !category.empty()) {
				break;
			}
			cout << "商品类别不能为空，请重新输入：";
		}
		cout << "请输入商品价格：";
        while (true) {
            if (cin >> price && price > 0) {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "输入无效，请输入一个正数：";
        }
		cout << "请输入商品库存：";
		while (true) {
			if (cin >> stock && stock >= 0) {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "输入无效，请输入一个非负数：";
		}
		json request;
		request["action"] = "addMerchandise";
		request["username"] = currentUser["username"];
		request["name"] = name;
		request["description"] = description;
		request["category"] = category;
		request["price"] = price;
		request["stock"] = stock;
		request["ownerid"] = currentUser["id"];
		json response = bo.sendRequest(request);
		if (response["status"] == "success") {
			cout << "商品添加成功！" << endl;
		}
        else {
            cout << "商品添加失败：" << response["message"] << endl;
        }
    }
	// 移除商品
    void removeMerchandise()const {
		string name;
		cout << "请输入商品名称：";
        while (true) {
            if (getline(cin, name) && !name.empty()) {
                break;
            }
            cout << "商品名不能为空，请重新输入：";
        }
		json request;
		request["action"] = "removeMerchandise";
		request["ownerid"] = currentUser["id"];
		request["name"] = name;
		json response = bo.sendRequest(request);
		if (response["status"] == "success") {
			cout << "商品删除成功！" << endl;
		}
		else {
			cout << "商品删除失败：" << response["message"] << endl;
		}
    }
	// 修改商品价格
    void modifyMerchandisePrice()const {
		string name;
		double price;
		cout << "请输入商品名称：";
        while (true) {
            if (getline(cin, name) && !name.empty()) {
                break;
            }
            cout << "商品名不能为空，请重新输入：";
        }
		cout << "请输入商品新价格：";
		while (true) {
			if (cin >> price && price > 0) {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "输入无效，请输入一个正数：";
		}
		json request;
		request["action"] = "modifyMerchandisePrice";
		request["ownerid"] = currentUser["id"];
		request["name"] = name;
		request["price"] = price;
		json response = bo.sendRequest(request);
        if (response["status"] == "success") {
            cout << "商品价格修改成功！" << endl;
        }
		else {
			cout << "商品价格修改失败：" << response["message"] << endl;
		}
    }
	// 补货
    void stockpile()const {
		string name;
		int stock;
		cout << "请输入商品名称：";
		while (true) {
			if (getline(cin, name) && !name.empty()) {
				break;
			}
			cout << "商品名不能为空，请重新输入：";
		}
		cout << "请输入商品库存：";
		while (true) {
			if (cin >> stock && stock >= 0) {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "输入无效，请输入一个正数：";
		}
		json request;
		request["action"] = "stockpile";
		request["ownerid"] = currentUser["id"];
		request["name"] = name;
		request["stock"] = stock;
		json response = bo.sendRequest(request);
		if (response["status"] == "success") {
			cout << "商品库存修改成功！" << endl;
		}
		else {
			cout << "商品库存修改失败：" << response["message"] << endl;
		}
    }
	// 按类别打折
	void discountByCategory()const {
		string category;
		double discount;
		cout << "请输入商品类别：";
		while (true) {
			if (getline(cin, category) && !category.empty()) {
				break;
			}
			cout << "商品类别不能为空，请重新输入：";
		}
		cout << "请输入折扣率（0-1）：";
		while (true) {
			if (cin >> discount && discount >= 0 && discount <= 1) {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "输入无效，请输入一个数字（0-1）：";
		}
		json request;
		request["action"] = "discountByCategory";
		request["ownerid"] = currentUser["id"];
		request["category"] = category;
		request["discount"] = discount;
		json response = bo.sendRequest(request);
		if (response["status"] == "success") {
			cout << "商品折扣修改成功！" << endl;
		}
		else {
			cout << "商品折扣修改失败：" << response["message"] << endl;
		}
	}
	// 查看我的商品
	void viewMyMerchandise()const {
		json request;
		request["action"] = "viewMyMerchandise";
		request["ownerid"] = currentUser["id"];
		json response = bo.sendRequest(request);
		if (response["status"] == "success") {
			cout << "商品列表：" << endl;
			for (const auto& merchandise : response["merchandises"]) {
				cout << "商品ID: " << merchandise["id"]
					<< ", 商品名称: " << merchandise["name"]
					<< ", 商品价格: " << merchandise["price"]
					<< ", 商品类别: " << merchandise["category"]
					<< ", 商品描述: " << merchandise["description"]
					<< ", 商品库存: " << merchandise["stock"]
					<< endl;
			}
		}
		else {
			cout << "查看商品失败：" << response["message"] << endl;
		}
	}
	// 查看我的消息
	void viewMyMessage()const {
		json request;
		request["action"] = "viewMyMessage";
		request["username"] = currentUser["username"];
		json response = bo.sendRequest(request);
		if (response["status"] == "success") {
			for (const auto& messages : response["message"]) {
				cout << messages << "\n";
			}
		}
		else {
			cout << "查看商家消息失败：" << response["message"] << endl;
		}
	}
};
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    // 初始化Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed." << endl;
        return 1;
    }
    // 创建套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Socket creation failed." << endl;
        WSACleanup();
        return 1;
    }
    // 连接服务器
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        cerr << "Invalid address or address not supported\n" ;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    serverAddr.sin_port = htons(8888);
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Connect failed.\n" ;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
	cout << "连接成功！\n";
	//进入循环，等待用户输入
    int choice;
	basicOperations bo;
    while (true) {
        cout << "\n*****************************************************************************\n";
        cout << "欢迎使用购物系统！   1. 注册账号   2. 登陆账号   3. 查看商品   其他. 退出";
        cout << "\n*****************************************************************************\n";
        cout << "请输入你的选择：";
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "输入无效，请输入一个数字。\n" ;
            cout << "请输入你的选择：";
        }
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (choice == 1) {
            bo.registerAccount();
        }
        else if (choice == 2) {
            if (bo.loginAccount()) {
				if (currentUser["accounttype"].get<int>() == 1) {
                    consumer c;
                    c.interact();
				}
				else if (currentUser["accounttype"].get<int>() == 2) {
                    merchant m;
                    m.interact();
				}
            }
        }
        else if (choice == 3) {
            bo.viewMerchandise();
        }
        else {
            cout << "感谢使用购物系统！\n";
            break;
        }
    }
	// 关闭套接字和清理Winsock
	closesocket(clientSocket);
	WSACleanup();
    return 0;
}