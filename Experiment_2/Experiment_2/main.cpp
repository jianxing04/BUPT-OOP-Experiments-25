#include<iostream>
#include<fstream>
#include<nlohmann/json.hpp>
#include<Windows.h>
using namespace std;
using json = nlohmann::json;
const string usersFilePath = "users.json";
const string merchandisesFilePath = "merchandises.json";
json allUsers;
json allMerchandises;
json currentUser;
class basicOperations {
public:
	void loadUsers()const {
		ifstream file(usersFilePath);
		if (file.is_open()) {
			file >> allUsers;
			file.close();
		}
		else {
			cout << "Unable to open users file." << endl;
		}
	}
	void loadMerchandises()const {
		ifstream file(merchandisesFilePath);
		if (file.is_open()) {
			file >> allMerchandises;
			file.close();
		}
		else {
			cout << "Unable to open merchandises file." << endl;
		}
	}
	void saveUsers()const {
		ofstream file(usersFilePath);
		if (file.is_open()) {
			file << allUsers.dump(4);
			file.close();
		}
		else {
			cout << "Unable to open users file." << endl;
		}
	}
	void saveMerchandises()const {
		ofstream file(merchandisesFilePath);
		if (file.is_open()) {
			file << allMerchandises.dump(4);
			file.close();
		}
		else {
			cout << "Unable to open merchandises file." << endl;
		}
	}
	void safeReadString(string& str)const {
		getline(cin, str);
		while (str.empty()) {
			getline(cin, str);
		}
	}
	void registerAccount() {
		string userName, password;
		int accountType;
		cout << "请输入用户名：";
		safeReadString(userName);
		while (1) {
			int flg = 0;
			for (const auto& user : allUsers) {
				if (user["username"] == userName) {
					flg = 1;
					break;
				}
			}
			if (flg) {
				cout << "该用户名已存在！请重新输入：";
				safeReadString(userName);
			}
			else {
				break;
			}
		}
		cout << "请输入密码：";
		safeReadString(password);
		cout << "输入账户类型：1-消费者，2-商家：";
		cin >> accountType;
		json newUser;
		newUser["accounttype"] = accountType;
		newUser["balance"] = 0.0;
		newUser["id"] = allUsers.size() + 1;
		newUser["orders"] = json::array();
		newUser["password"] = password;
		newUser["shoppingtrolley"] = json::array();
		newUser["username"] = userName;
		allUsers.push_back(newUser);
		cout << "账号注册成功！\n";
		saveUsers();
	}
	void loginAccount()const {
		string userName, userPassword;
		cout << "输入账号名：";
		safeReadString(userName);
		while (1) {
			int flg = 0;
			for (const auto& user : allUsers) {//遍历所有用户
				if (user["username"] == userName) {//找到匹配的用户名
					flg = 1;
					cout << "请输入密码：";
					safeReadString(userPassword);
					while (userPassword != user["password"]) {
						cout << "密码不正确！请重新输入：";
						safeReadString(userPassword);
					}
					currentUser = user;
					cout << "登陆成功！\n";
					break;
				}
			}
			if (flg) {
				break;
			}
			else {
				cout << "该账号不存在！请重新输入：";
				safeReadString(userName);
			}
		}
	}
	void viewMerchandise()const {
		cout << "商品列表:\n";
		for (const auto& merchandise : allMerchandises) {
			cout << "名称: " << merchandise["name"] << ", 价格: " << merchandise["price"] << "，余量：" << merchandise["stock"] << "\n";
		}
	}
};
class userBase {
public:
	userBase(int ID) : id(ID) {}
	virtual void getUserType() const = 0;
	virtual void interaction() const = 0;
	void changePassword()const {
		string newPassword;
		int flg = 0;
		cout << "请输入新密码: ";
		bo.safeReadString(newPassword);
		for (auto& user : allUsers) {
			if (user["id"] == id) {
				user["password"] = newPassword;
				bo.saveUsers();
				cout << "密码修改成功！\n";
				flg = 1;
				break;
			}
		}
		if (!flg) {
			cout << "密码修改失败！\n";
		}
	}
	void viewMerchandise()const {
		cout << "商品列表:\n";
		for (const auto& merchandise : allMerchandises) {
			cout << "名称: " << merchandise["name"] << ", 价格: " << merchandise["price"] << "，余量：" << merchandise["stock"] << "\n";
		}
	}
	void blanceInquiry()const {
		for (const auto& user : allUsers) {
			if (user["id"] == id) {
				cout << "当前余额: " << user["balance"] << "\n";
				break;
			}
		}
	}
	void recharge()const {
		double amount;
		cout << "请输入充值金额: ";
		cin >> amount;
		for (auto& user : allUsers) {
			if (user["id"] == id) {
				user["balance"] = user["balance"].get<double>() + amount;
				bo.saveUsers();
				cout << "充值成功！当前余额: " << user["balance"] << "\n";
				break;
			}
		}
	}
	void searchForMerchandise()const {
		string merchandiseName;
		cout << "请输入要搜索的商品名称: ";
		bo.safeReadString(merchandiseName);
		cout << "搜索结果如下\n";
		bool flg = 0;
		for (const auto& merchandise : allMerchandises) {
			if (merchandise["name"] == merchandiseName) {
				flg = 1;
				cout << "名称: " << merchandise["name"] << ", 价格: " << merchandise["price"] << "，余量：" << merchandise["stock"] << "\n";
			}
		}
		if (!flg) {
			cout << "此商品不存在!\n";
		}
	}
	int id;
	basicOperations bo;
};
class consumer : public userBase {
public:
	consumer(int ID) : userBase(ID) {}
	void getUserType() const override {
		cout << "用户类型: 消费者\n";
	}
	void viewMyShoppingTrolley()const {
		cout << "购物车商品列表:\n";
		for (const auto& user : allUsers) {
			if (user["id"] == id) {
				for (const auto& item : user["shoppingtrolley"]) {
					cout << "名称: " << item["name"] << ", 价格: " << item["price"] << "\n";
				}
				break;
			}
		}
	}
	void addToShoppingTrolley()const {
		string merchandiseName;
		int flg = 0;
		cout << "请输入要添加到购物车的商品名称: ";
		bo.safeReadString(merchandiseName);
		for (const auto& user : allUsers) {
			if (user["id"] == id) {
				for (const auto& item : user["shoppingtrolley"]) {
					if (item["name"] == merchandiseName) {
						cout << "购物车中已存在此商品！\n";
						return;
					}
				}
				break;
			}
		}
		for (const auto& merchandise : allMerchandises) {
			if (merchandise["name"] == merchandiseName) {
				flg = 1;
				cout << "商品信息如下：\n";
				cout << "名称: " << merchandise["name"] << ", 价格: " << merchandise["price"] << "，余量：" << merchandise["stock"] << "\n";
				for (auto& user : allUsers) {
					if (user["id"] == id) {
						json merchandiseCopy;
						merchandiseCopy["name"] = merchandise["name"];
						merchandiseCopy["price"] = merchandise["price"];
						merchandiseCopy["id"] = merchandise["id"];
						merchandiseCopy["ownerid"] = merchandise["ownerid"];
						user["shoppingtrolley"].push_back(merchandiseCopy);
						bo.saveUsers();
						cout << "商品已添加到购物车！\n";
						break;
					}
				}
				break;
			}
		}
		if (!flg) {
			cout << "此商品不存在!\n";
		}
	}
	void removeFromShoppingTrolley()const {
		string merchandiseName;
		int flg = 0;
		cout << "请输入要从购物车中移除的商品名称: ";
		bo.safeReadString(merchandiseName);
		for (auto& user : allUsers) {
			if (user["id"] == id) {
				for (auto it = user["shoppingtrolley"].begin(); it != user["shoppingtrolley"].end(); ++it) {
					if ((*it)["name"] == merchandiseName) {
						user["shoppingtrolley"].erase(it);
						bo.saveUsers();
						cout << "商品已从购物车中移除！\n";
						flg = 1;
						break;
					}
				}
				break;
			}
		}
		if (!flg) {
			cout << "此商品不在购物车中!\n";
		}
	}
	void clearShoppingTrolley()const {
		for (auto& user : allUsers) {
			if (user["id"] == id) {
				user["shoppingtrolley"].clear();
				bo.saveUsers();
				cout << "购物车已清空！\n";
				break;
			}
		}
	}
	void generateOrder()const {
		cout << "您的购物车清单如下：\n";
		viewMyShoppingTrolley();
		double totalPrice = 0.0;
		for (auto& user : allUsers) {
			if (user["id"] == id) {//找到当前用户
				json orders = json::array();
				double balance = user["balance"].get<double>();
				for (const auto& item : user["shoppingtrolley"]) {//遍历购物车
					json order;
					order["name"] = item["name"];
					order["price"] = item["price"];
					order["id"] = item["id"];
					order["ownerid"] = item["ownerid"];
					cout << "名称: " << item["name"] << ", 价格: " << item["price"] << "\n";
					cout << "对于上面这个商品，请输入购买数量：";
					int quantity;
					cin >> quantity;
					while (totalPrice + quantity * item["price"].get<double>() > balance) {//判断余额是否足够
						cout << "余额不足！请重新输入购买数量：\n";
						cin >> quantity;
					}
					for (auto& merchandise : allMerchandises) {
						if (merchandise["id"] == item["id"]) {
							while (merchandise["stock"].get<int>() < quantity) {//判断库存是否足够
								cout << "库存不足！请重新输入购买数量：\n";
								cin >> quantity;
							}
							merchandise["stock"] = merchandise["stock"].get<int>() - quantity;
							bo.saveMerchandises();
							break;
						}
					}
					totalPrice += quantity * item["price"].get<double>();
					order["quantity"] = quantity;
					if (quantity > 0) {//如果购买数量大于0，则添加到订单
						orders.push_back(order);
					}
				}
				user["orders"] = orders;
				bo.saveUsers();
				cout << "订单生成成功！一共 "<<totalPrice<<" 元\n";
				break;
			}
		}
	};
	void viewMyOrders()const {
		double totalPrice = 0.0;
		cout << "您的订单清单如下：\n";
		for (const auto& user : allUsers) {
			if (user["id"] == id) {//找到当前用户
				for (const auto& order : user["orders"]) {
					totalPrice += order["price"].get<double>() * order["quantity"].get<int>();
					cout << "名称: " << order["name"] << ", 价格: " << order["price"] << ", 数量: " << order["quantity"] << "\n";
				}
				cout << "总金额: " << totalPrice << "\n";
				break;
			}
		}
	}
	void payForTheOrder()const {
		double totalPrice = 0.0;
		for (auto& user : allUsers) {
			if (user["id"] == id) {//找到当前用户
				for (const auto& order : user["orders"]) {//遍历订单
					totalPrice += order["price"].get<double>() * order["quantity"].get<int>();
					for (auto& merchant : allUsers) {//遍历商家
						if (merchant["id"] == order["ownerid"]) {//找到商家
							merchant["balance"] = merchant["balance"].get<double>() + order["price"].get<double>() * order["quantity"].get<int>();
							break;
						}
					}
				}
				if (totalPrice > 0) {
					user["balance"] = user["balance"].get<double>() - totalPrice;
					user["orders"].clear();
					bo.saveUsers();
					cout << "支付成功！当前余额: " << user["balance"] << "\n";
				}
				else {
					cout << "没有订单需要支付！\n";
				}
				break;
			}
		}
	}
	void clearMyOrders()const {
		for (auto& user : allUsers) {
			if (user["id"] == id) {//找到当前用户
				user["orders"].clear();
				bo.saveUsers();
				cout << "订单已清空！\n";
				break;
			}
		}
	}
	void interaction() const override {
		int choice;
		while (1) {
			cout << "\n******************************************************************************************\n";
			cout << "欢迎使用购物系统！\n";
			cout << "0. 查看用户类型   1. 修改密码   2. 查看所有商品   3. 查询余额   4. 充值\n";
			cout << "5. 搜索商品   6.查看我的购物车   7.向我的购物车添加商品   8.从我的购物车删除一件商品   9.清空我的购物车\n";
			cout << "10.根据购物车生成订单   11.查看我的订单   12.支付订单   13.清空订单   其他.退出";
			cout << "\n******************************************************************************************\n";
			cout << "请输入你的选择：";
			cin >> choice;
			switch (choice) {
			case 0: getUserType(); break; //查看用户类型
			case 1: changePassword(); break;
			case 2: viewMerchandise(); break;
			case 3: blanceInquiry(); break;
			case 4: recharge(); break;
			case 5: searchForMerchandise(); break;
			case 6: viewMyShoppingTrolley(); break;
			case 7: addToShoppingTrolley(); break;
			case 8: removeFromShoppingTrolley(); break;
			case 9: clearShoppingTrolley(); break;
			case 10: generateOrder(); break;
			case 11: viewMyOrders(); break;
			case 12: payForTheOrder(); break;
			case 13: clearMyOrders(); break;
			default: return; //退出
			}
		}
	}
};
class merchant : public userBase {
public:
	merchant(int ID) : userBase(ID) {}
	void getUserType() const override {
		cout << "用户类型: 商家\n";
	}
	void addMerchandise()const {
		string name,description,category;
		double price;
		int stock;
		cout << "请输入商品名称: ";
		bo.safeReadString(name);
		cout << "请输入商品价格: ";
		cin >> price;
		while (price < 0) {
			cout << "价格不能为负数，请重新输入: ";
			cin >> price;
		}
		cout << "请输入商品库存: ";
		cin >> stock;
		while (stock < 0) {
			cout << "库存不能为负数，请重新输入: ";
			cin >> stock;
		}
		cout << "请输入商品描述: ";
		bo.safeReadString(description);
		cout << "请输入商品分类: ";
		bo.safeReadString(category);
		json newMerchandise;
		newMerchandise["name"] = name;
		newMerchandise["price"] = price;
		newMerchandise["stock"] = stock;
		newMerchandise["id"] = allMerchandises.size() + 1;
		newMerchandise["ownerid"] = id;
		newMerchandise["description"] = description;
		newMerchandise["category"] = category;
		allMerchandises.push_back(newMerchandise);
		bo.saveMerchandises();
		cout << "商品添加成功！\n";
	}
	void removeMerchandise()const {
		string merchandiseName;
		int flg = 0;
		cout << "请输入要删除的商品名称: ";
		bo.safeReadString(merchandiseName);
		for (auto it = allMerchandises.begin(); it != allMerchandises.end(); ++it) {
			if ((*it)["name"] == merchandiseName && (*it)["ownerid"] == id) {
				allMerchandises.erase(it);
				flg = 1;
				break;
			}
		}
		if (flg) {
			cout << "商品已删除！\n";
			bo.saveMerchandises();
		}
		else {
			cout << "此商品不存在或不是您的商品！\n";
		}
	}
	void modifyMerchandisePrice()const {
		string name;
		double newPrice;
		cout << "请输入要修改价格的商品名称: ";
		bo.safeReadString(name);
		int flg = 0;
		for (auto& merchandise : allMerchandises) {
			if (merchandise["name"] == name && merchandise["ownerid"] == id) {
				cout << "请输入新的价格: ";
				cin >> newPrice;
				while (newPrice < 0) {
					cout << "价格不能为负数，请重新输入: ";
					cin >> newPrice;
				}
				merchandise["price"] = newPrice;
				bo.saveMerchandises();
				cout << "价格修改成功！\n";
				flg = 1;
				break;
			}
		}
		if (!flg) {
			cout << "商品不存在或您没有权限修改此商品的价格！\n";
		}
	}
	void stockpile()const {
		string name;
		int stock;
		cout << "请输入要补货的商品名称: ";
		bo.safeReadString(name);
		int flg = 0;
		for (auto& merchandise : allMerchandises) {
			if (merchandise["name"] == name && merchandise["ownerid"] == id) {
				cout << "请输入补货数量: ";
				cin >> stock;
				while (stock < 0) {
					cout << "补货数量不能为负数，请重新输入: ";
					cin >> stock;
				}
				merchandise["stock"] = merchandise["stock"].get<int>() + stock;
				bo.saveMerchandises();
				cout << "补货成功！当前库存: " << merchandise["stock"] << "\n";
				flg = 1;
				break;
			}
		}
		if (!flg) {
			cout << "商品不存在或您没有权限补货！\n";
		}
	}
	void discountByCategory()const {
		string category;
		double discount;
		cout << "请输入商品分类: ";
		bo.safeReadString(category);
		cout << "请输入折扣率（0-1之间）: ";
		cin >> discount;
		while (discount < 0 || discount>1) {
			cout << "折扣率不合法，请重新输入（0-1之间）: ";
			cin >> discount;
		}
		int flg = 0;
		for (auto& merchandise : allMerchandises) {
			if (merchandise["category"] == category && merchandise["ownerid"] == id) {
				merchandise["price"] = merchandise["price"].get<double>() * discount;
				flg = 1;
			}
		}
		if (!flg) {
			cout << "没有找到符合条件的商品！\n";
		}
		else {
			cout << "折扣成功！\n";
			bo.saveMerchandises();
		}
	}
	void viewMyMerchandise()const {
		cout << "我的商品列表:\n";
		for (const auto& merchandise : allMerchandises) {
			if (merchandise["ownerid"] == id) {
				cout << "名称: " << merchandise["name"] << ", 价格: " << merchandise["price"] << "，余量：" << merchandise["stock"] << "\n";
			}
		}
	}
	void interaction() const override {
		int choice;
		while (1) {
			cout << "\n**********************************************************************\n";
			cout << "欢迎使用购物系统！\n";
			cout << "0. 查看用户类型   1. 修改密码   2. 查看所有商品   3. 查询余额   4. 充值\n";
			cout << "5. 搜索商品   6.添加商品   7.删除商品   8.修改产品价格   9.增加产品库存\n";
			cout << "10.根据商品类别打折   11.查看我的商品   其他.退出";
			cout << "\n**********************************************************************\n";
			cout << "请输入你的选择：";
			cin >> choice;
			switch (choice) {
			case 0: getUserType(); break; //查看用户类型
			case 1: changePassword(); break;
			case 2: viewMerchandise(); break;
			case 3: blanceInquiry(); break;
			case 4: recharge(); break;
			case 5: searchForMerchandise(); break;
			case 6: addMerchandise(); break;
			case 7: removeMerchandise(); break;
			case 8: modifyMerchandisePrice(); break;
			case 9: stockpile(); break;
			case 10: discountByCategory(); break;
			case 11: viewMyMerchandise(); break;
			default: return; //退出
			}
		}
	}
};
class merchandiseBase {
public:
	merchandiseBase(int ID) : id(ID) {}
	virtual void getPrice() const = 0;
	int id;
	double price, discount;
};
class book : public merchandiseBase {
public:
	book(int ID) : merchandiseBase(ID) {}
	void getPrice() const override {
		cout << "书籍价格: " << price * discount << "\n";
	}
};
class clothes : public merchandiseBase {
public:
	clothes(int ID) : merchandiseBase(ID) {}
	void getPrice() const override {
		cout << "衣服价格: " << price * discount << "\n";
	}
};
int main(){
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	basicOperations bo;
	bo.loadUsers();
	bo.loadMerchandises();
	int choice;
	while (1) {
		cout << "\n*****************************************************************************\n";
		cout << "欢迎使用购物系统！   1. 注册账号   2. 登陆账号   3. 查看所有商品   其他. 退出";
		cout << "\n*****************************************************************************\n";
		cout << "请输入你的选择：";
		cin >> choice;
		if (choice == 1) {
			bo.registerAccount();
		}
		else if (choice == 2) {
			bo.loginAccount();
			int userType = currentUser["accounttype"];
			int userID = currentUser["id"];
			if (userType == 1) {
				consumer c(userID);
				c.interaction();
			}
			else {
				merchant m(userID);
				m.interaction();
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
	bo.saveUsers();
	bo.saveMerchandises();
	return 0;
}