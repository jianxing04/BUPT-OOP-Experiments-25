#include<iostream>
#include<fstream>
#include<string>
#include<nlohmann/json.hpp>
#define NOMINMAX
#include<Windows.h>
#include"UserLogin.h"
using json = nlohmann::json;
using namespace std;
const string usersFilePath = "users.json";
const string merchandisesFilePath = "merchandises.json";
json users,merchandises;
class prepare {
public:
	void loadJsonFiles(const string& usersPath, const string& merchandisesPath) const{
		ifstream usersFile(usersPath);
		ifstream merchandisesFile(merchandisesPath);
		if (usersFile.is_open()) {
			usersFile >> users;
			usersFile.close();
		}
		else {
			cout << "无法打开文件: " << usersPath << endl;
		}
		if (merchandisesFile.is_open()) {
			merchandisesFile >> merchandises;
			merchandisesFile.close();
		}
		else {
			cout << "无法打开文件: " << merchandisesPath << endl;
		}
	}
	void saveJsonFiles(const string& filePath,const json& jsonFile) const{
		ofstream file(filePath);
		if (file.is_open()) {
			file << setw(4) << jsonFile;
			file.close();
		}
		else {
			cout << "无法打开文件: " << filePath << endl;
		}
	}
	void viewMerchandise() const{
		cout << "商品列表:\n";
		for (const auto& merchandise : merchandises) {
			cout << "名称: " << merchandise["name"] << ", 价格: " << merchandise["price"] << "，余量：" << merchandise["stock"] << "\n";
		}
	}
};
class userBase {
public:
	userBase(int ID) : id(ID) {}
	virtual void getUserType() const = 0;
	void changePassword() const {
		string newPassword;
		cout << "请输入新密码: ";
		while (true) {
			getline(cin, newPassword);
			if (!newPassword.empty()) break;
			cout << "密码不能为空，请重新输入: ";
		}
		for (auto& user : users) {
			if (user["id"] == id) {
				user["password"] = newPassword;
				p.saveJsonFiles(usersFilePath, users);
				cout << "密码修改成功！\n";
				return;
			}
		}
		cout << "密码修改失败！\n";
	}
	void viewMerchandise()const {
		cout << "商品列表:\n";
		for (const auto& merchandise : merchandises) {
			cout << "名称: " << merchandise["name"] << ", 价格: " << merchandise["price"] <<"，余量：" <<merchandise["stock"] << "\n";
		}
	}
	void blanceInquiry()const {
		for (const auto& user : users) {
			if (user["id"] == id) {
				cout << "当前余额: " << user["balance"] << "\n";
				break;
			}
		}
	}
	void recharge()const {
		double amount;
		cout << "请输入充值金额: ";
		while (true) {
			cin >> amount;
			if (cin.fail() || amount <= 0) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "充值金额无效，请重新输入: ";
			}
			else {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
		}
		for (auto& user : users) {
			if (user["id"] == id) {
				user["balance"] = user["balance"].get<double>() + amount;
				p.saveJsonFiles(usersFilePath, users);
				cout << "充值成功！当前余额: " << user["balance"] << "\n";
				break;
			}
		}
	}
	void searchForMerchandise()const {
		string merchandiseName;
		cout << "请输入要搜索的商品名称: ";
		while (true) {
			getline(cin, merchandiseName);
			if (!merchandiseName.empty()) break;
			cout << "商品名称不能为空，请重新输入: ";
		}
		cout << "搜索结果如下\n";
		bool flg = 0;
		for (const auto& merchandise : merchandises) {
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
	prepare p;
};
class customer : public userBase {
public:
	customer(int ID) : userBase(ID) {}
	void getUserType() const override {
		cout << "用户类型: 顾客\n";
	}
	void purchase()const {
		string merchandiseName;
		cout << "请输入要购买的商品名称: ";
		while (true) {
			getline(cin, merchandiseName);
			if (!merchandiseName.empty()) break;
			cout << "商品名称不能为空，请重新输入: ";
		}
		for (auto& merchandise : merchandises) {
			if (merchandise["name"] == merchandiseName) {
				double price = merchandise["price"];
				int stock = merchandise["stock"];
				if (stock > 0) {
					for (auto& user : users) {
						if (user["id"] == id) {
							double balance = user["balance"];
							if (balance >= price) {
								user["balance"] = balance - price;
								merchandise["stock"] = stock - 1;
								p.saveJsonFiles(usersFilePath, users);
								p.saveJsonFiles(merchandisesFilePath, merchandises);
								cout << "购买成功！当前余额: " << user["balance"] << "\n";
							}
							else {
								cout << "余额不足，购买失败！\n";
							}
							break;
						}
					}
				}
				else {
					cout << "商品已售罄！\n";
				}
				break;
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
	void addNewMerchandise()const {
		string name,description,category;
		double price;
		int stock;
		cout << "请输入商品名称: ";
		while (true) {
			getline(cin, name);
			if (!name.empty()) break;
			cout << "商品名称不能为空，请重新输入: ";
		}
		cout << "请输入商品价格: ";
		while (true) {
			cin >> price;
			if (cin.fail() || price <= 0) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "商品价格无效，请重新输入: ";
			}
			else {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
		}
		cout << "请输入商品库存: ";
		while (true) {
			cin >> stock;
			if (cin.fail() || stock < 0) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "商品库存无效，请重新输入: ";
			}
			else {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
		}
		cout << "请输入商品描述: ";
		while (true) {
			getline(cin, description);
			if (!description.empty()) break;
			cout << "商品描述不能为空，请重新输入: ";
		}
		cout << "请输入商品分类: ";
		while (true) {
			getline(cin, category);
			if (!category.empty()) break;
			cout << "商品分类不能为空，请重新输入: ";
		}
		json newMerchandise;
		newMerchandise["id"] = merchandises.size() + 1;
		newMerchandise["ownerId"] = id;
		newMerchandise["name"] = name;
		newMerchandise["price"] = price;
		newMerchandise["stock"] = stock;
		newMerchandise["description"] = description;
		newMerchandise["category"] = category;
		merchandises.push_back(newMerchandise);
		p.saveJsonFiles(merchandisesFilePath, merchandises);
		cout << "商品添加成功！\n";
	}
	void modifyMerchandisePrice()const {
		string name;
		double newPrice;
		cout << "请输入要修改价格的商品名称: ";
		while (true) {
			getline(cin, name);
			if (!name.empty()) break;
			cout << "商品名称不能为空，请重新输入: ";
		}
		cout << "请输入新的价格: ";
		while (true) {
			cin >> newPrice;
			if (cin.fail() || newPrice <= 0) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "价格无效，请重新输入: ";
			}
			else {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
		}
		int flg = 0;
		for (auto& merchandise : merchandises) {
			if (merchandise["name"] == name && merchandise["ownerId"] == id) {
				merchandise["price"] = newPrice;
				p.saveJsonFiles(merchandisesFilePath, merchandises);
				cout << "价格修改成功！\n";
				flg = 1;
				break;
			}
		}
		if (!flg) {
			cout << "商品不存在或您没有权限修改此商品的价格！\n";
		}
	}
	void discountByCategory()const {
		string category;
		double discount;
		cout << "请输入商品分类: ";
		while (true) {
			getline(cin, category);
			if (!category.empty()) break;
			cout << "商品分类不能为空，请重新输入: ";
		}
		cout << "请输入折扣率（0-1之间）: ";
		while (true) {
			cin >> discount;
			if (cin.fail() || discount < 0 || discount > 1) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "折扣率无效，请重新输入（0-1之间）: ";
			}
			else {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
		}
		int flg = 0;
		for (auto& merchandise : merchandises) {
			if (merchandise["category"] == category && merchandise["ownerId"] == id) {
				double price = merchandise["price"];
				merchandise["price"] = price * discount;
				flg = 1;
			}
		}
		if (!flg) {
			cout << "没有找到符合条件的商品！\n";
			return;
		}
		p.saveJsonFiles(merchandisesFilePath, merchandises);
		cout << "折扣设置成功！\n";
	}
	void stockpile()const {
		string name;
		int stock;
		cout << "请输入要补货的商品名称: ";
		while (true) {
			getline(cin, name);
			if (!name.empty()) break;
			cout << "商品名称不能为空，请重新输入: ";
		}
		cout << "请输入补货数量: ";
		while (true) {
			cin >> stock;
			if (cin.fail() || stock <= 0) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "补货数量无效，请重新输入: ";
			}
			else {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
		}
		int flg = 0;
		for (auto& merchandise : merchandises) {
			if (merchandise["name"] == name && merchandise["ownerId"] == id) {
				int currentStock = merchandise["stock"];
				merchandise["stock"] = currentStock + stock;
				p.saveJsonFiles(merchandisesFilePath, merchandises);
				cout << "补货成功！当前库存: " << merchandise["stock"] << "\n";
				flg = 1;
				break;
			}
		}
		if (!flg) {
			cout << "商品不存在或您没有权限补货！\n";
		}
	}
};
class merchandiseBase {
public:
	merchandiseBase(int ID) : id(ID) {}
	virtual void getPrice() const = 0;
	int id;
	double price,discount;
};
class book : public merchandiseBase {
public:
	book(int ID) : merchandiseBase(ID) {}
	void getPrice() const override {
		cout << "书籍价格: " << price*discount << "\n";
	}
};
class clothes : public merchandiseBase {
public:
	clothes(int ID) : merchandiseBase(ID) {}
	void getPrice() const override {
		cout << "衣服价格: " << price * discount << "\n";
	}
};
int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	prepare p;
	p.loadJsonFiles(usersFilePath, merchandisesFilePath);
	int choice;
	cout << "请选择操作：输入 1 是登录，输入 2 查看商品信息: ";
	while (true) {
		cin >> choice;
		if (cin.fail() || (choice != 1 && choice != 2)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "输入无效，请重新输入: ";
		}
		else {
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除换行符
			break;
		}
	}
	while (choice == 2) {
		p.viewMerchandise();
		cout << "请选择操作：输入 1 是登录，输入 2 查看商品信息: ";
		while (true) {
			cin >> choice;
			if (cin.fail() || (choice != 1 && choice != 2)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "输入无效，请重新输入: ";
			}
			else {
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除换行符
				break;
			}
		}
	}
	UserLogin userLogin("users.json");
	json user=userLogin.mainMenu();
	p.loadJsonFiles(usersFilePath, merchandisesFilePath);
	if (user["accountType"] == 1) {
		customer c(user["id"]);
		while (1) {
			cout << "输入 1 是查看用户类型，输入 2 是修改密码，输入 3 是余额查询，输入 4 是充值，输入 5 是购买商品，输入 6 是搜索商品，输入 7 是查看所有商品,输入其他数字退出: ";
			while (true) {
				cin >> choice;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "输入无效，请重新输入: ";
				}
				else {
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除换行符
					break;
				}
			}
			if (choice == 1) {
				c.getUserType();
			}
			else if (choice == 2) {
				c.changePassword();
			}
			else if (choice == 3) {
				c.blanceInquiry();
			}
			else if (choice == 4) {
				c.recharge();
			}
			else if (choice == 5) {
				c.purchase();
			}
			else if (choice == 6) {
				c.searchForMerchandise();
			}
			else if (choice == 7) {
				c.viewMerchandise();
			}
			else {
				break;
			}
		}
	}
	else {
		merchant m(user["id"]);
		while (1) {
			cout << "输入 1 是查看用户类型，输入 2 是修改密码，输入 3 是余额查询，输入 4 是充值，输入 5 是添加商品，输入 6 是修改商品价格，输入 7 是设置折扣，输入 8 是补货，输入9是查看所有商品，输入其他数字退出: ";
			while (true) {
				cin >> choice;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "输入无效，请重新输入: ";
				}
				else {
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除换行符
					break;
				}
			}
			if (choice == 1) {
				m.getUserType();
			}
			else if (choice == 2) {
				m.changePassword();
			}
			else if (choice == 3) {
				m.blanceInquiry();
			}
			else if (choice == 4) {
				m.recharge();
			}
			else if (choice == 5) {
				m.addNewMerchandise();
			}
			else if (choice == 6) {
				m.modifyMerchandisePrice();
			}
			else if (choice == 7) {
				m.discountByCategory();
			}
			else if (choice == 8) {
				m.stockpile();
			}
			else if (choice == 9) {
				m.viewMerchandise();
			}
			else {
				break;
			}
		}
	}
	return 0;
}
