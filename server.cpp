#include<iostream>
#include<fstream>
#include<winsock2.h>
#include<windows.h>
#include<nlohmann/json.hpp>
#include<thread>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
using json = nlohmann::json;

const string usersFilePath = "users.json";
const string merchandisesFilePath = "merchandises.json";
const string merchandiseIDFilePath = "merchandiseID.json";

json allUsers;
json allMerchandises;
json merchandiseID;

class basicOperations {
public:
	// 加载用户和商品数据
    void loadUsers()const {
        ifstream usersFile(usersFilePath);
        if (usersFile.is_open()) {
            usersFile >> allUsers;
            usersFile.close();
        }
        else {
            cout << "Failed to open users file.\n";
        }
    }
	// 加载商品数据
    void loadMerchandises()const {
        ifstream merchandisesFile(merchandisesFilePath);
        if (merchandisesFile.is_open()) {
            merchandisesFile >> allMerchandises;
            merchandisesFile.close();
        }
        else {
            cout << "Failed to open merchandises file.\n";
        }
        ifstream merchandiseIDFile(merchandiseIDFilePath);
        if (merchandiseIDFile.is_open()) {
            merchandiseIDFile >> merchandiseID;
            merchandiseIDFile.close();
        }
        else {
            cout << "Failed to open merchandise ID file.\n";
        }
    }
	// 保存用户和商品数据
    void saveUsers()const {
        ofstream usersFile(usersFilePath);
        if (usersFile.is_open()) {
            usersFile << allUsers.dump(4);
            usersFile.close();
        }
        else {
            cout << "Failed to open users file for writing.\n";
        }
    }
	// 保存商品数据
    void saveMerchandises()const {
        ofstream merchandisesFile(merchandisesFilePath);
        if (merchandisesFile.is_open()) {
            merchandisesFile << allMerchandises.dump(4);
            merchandisesFile.close();
        }
        else {
            cout << "Failed to open merchandises file for writing.\n";
        }
        ofstream merchandiseIDFile(merchandiseIDFilePath);
        if (merchandiseIDFile.is_open()) {
            merchandiseIDFile << merchandiseID.dump(4);
            merchandiseIDFile.close();
        }
        else {
            cout << "Failed to open merchandise ID file for writing.\n";
        }
    }
	// 注册账户
    void registerAccount(const json& request, json& response)const {
        string username = request["username"];
        string password = request["password"];
		for (const auto& user : allUsers) {
			if (user["username"] == username) {
				response["status"] = "error";
				response["message"] = "Username already exists.";
				return;
			}
		}
		json newUser;
		newUser["accounttype"] = request["accounttype"]; // 设置账户类型
		newUser["balance"] = 0; // 设置账户余额
		newUser["id"] = allUsers.size() + 1; // 设置用户ID
		newUser["orders"] = json::array(); // 初始化订单列表
		newUser["password"] = password;// 设置密码
		newUser["shoppingtrolley"] = json::array(); // 初始化购物车
		newUser["username"] = username;
		allUsers.push_back(newUser);
		saveUsers();
		response["status"] = "success";
		response["message"] = "Registration successful.";
    }
	// 登录账户
    void loginAccount(const json& request, json& response)const {
        string username = request["username"];
        string password = request["password"];
        for (const auto& user : allUsers) {
			if (user["username"] == username) {
				if (user["password"] == password) {
					response["status"] = "success";
					response["message"] = "Login successful.";
                    response["user"] = user;
				}
				else {
					response["status"] = "error";
					response["message"] = "Incorrect password.";
				}
                return;
			}
        }
        response["status"] = "error";
        response["message"] = "User not found.";
    }
	// 查看商品
    void viewMerchandise(json& response)const {
        response["merchandises"] = allMerchandises;
        response["status"] = "success";
    }
	// 修改密码
    void changePassword(const json& request, json& response)const {
		string username = request["username"];
		string newPassword = request["newPassword"];
        for (auto& user : allUsers) {
            if (user["username"] == username) {
                user["password"] = newPassword;
                saveUsers();
                response["status"] = "success";
                return;
            }
        }
    }
    //查询余额
	void balanceInquiry(const json& request, json& response)const {
		string username = request["username"];
		for (const auto& user : allUsers) {
			if (user["username"] == username) {
				response["status"] = "success";
				response["balance"] = user["balance"];
				return;
			}
		}
	}
    //充值
    void recharge(const json& request, json& response)const {
		string username = request["username"];
		double amount = request["amount"];
		for (auto& user : allUsers) {
			if (user["username"] == username) {
				user["balance"] = user["balance"].get<double>() + amount;
				saveUsers();
				response["status"] = "success";
				response["message"] = "Recharge successful.";
				return;
			}
		}
    }
    //搜索商品
	void searchForMerchandise(const json& request, json& response)const {
		string searchTerm = request["merchandiseName"];
		json searchResults = json::array();
		int flg = 0;
		for (const auto& merchandise : allMerchandises) {
			if (merchandise["name"].get<string>()==searchTerm) {
				searchResults.push_back(merchandise);
				flg = 1;
			}
		}
		if (flg == 0) {
			response["status"] = "error";
			response["message"] = "Merchandise not found.";
			return;
		}
		response["status"] = "success";
		response["results"] = searchResults;
	}
	//添加商品到购物车
    void addToShoppingTrolley(const json& request, json& response)const {
		int id = request["id"];
		string username = request["username"];
        int flg = 0;
        for (auto& user : allUsers) {
            if (user["username"] == username) {
				for (const auto& item : user["shoppingtrolley"]) {
					if (item["id"] == id) {
						response["status"] = "error";
						response["message"] = "Item already in shopping trolley.";
						return;
					}
				}
                for (const auto& merchandise : allMerchandises) {
                    if (merchandise["id"] == id) {
                        flg = 1;
                        user["shoppingtrolley"].push_back(merchandise);
                    }
                }
            }
        }
		if (flg == 1) {
            saveUsers();
			response["status"] = "success";
			response["message"] = "Item added to shopping trolley.";
		}
        else {
            response["status"] = "error";
            response["message"] = "Item not found.";
        }
    }
    //查看购物车
    void viewMyShoppingTrolley(const json& request, json& response)const {
		string username = request["username"].get<string>();
        for (const auto& user : allUsers) {
            if (user["username"] == username) {
                response["status"] = "success";
				for (const auto& item : user["shoppingtrolley"]) {
					int id = item["id"].get<int>();
					for (const auto& merchandise : allMerchandises) {
						if (merchandise["id"] == id) {
							response["shoppingtrolley"].push_back(merchandise);
                            break;
						}
					}
				}
				return;
            }
        }
    }
	//从购物车中删除商品
    void removeFromShoppingTrolley(const json& request, json& response)const {
		string username = request["username"];
		for (auto& user : allUsers) {
			if (user["username"] == username) {
				for (auto it = user["shoppingtrolley"].begin(); it != user["shoppingtrolley"].end(); ++it) {
					if ((*it)["id"] == request["id"]) {
						user["shoppingtrolley"].erase(it);
						response["status"] = "success";
                        saveUsers();
						return;
					}
				}
				response["status"] = "error";
				response["message"] = "Item not found in shopping trolley.";
				return;
			}
		}
    }
	//清空购物车
    void clearShoppingTrolley(const json& request, json& response)const {
        string username = request["username"];
        for (auto& user : allUsers) {
            if (user["username"] == username) {
                user["shoppingtrolley"].clear();
                response["status"] = "success";
                saveUsers();
                return;
            }
        }
    }
	// 生成订单
    void generateOrder(const json& request, json& response)const {
        string username = request["username"];
        for (auto& user : allUsers) {
            if (user["username"] == username) {
                user["orders"].push_back(request["orders"]);
                user["shoppingtrolley"].clear();
				user["balance"] = user["balance"].get<double>() - request["totalPrice"].get<double>();
                response["status"] = "success";
                response["message"] = "Order generated successfully.";
				for (const auto& item : request["orders"]) {
					int id = item["id"].get<int>();
					for (auto& merchandise : allMerchandises) {
						if (merchandise["id"] == id) {
							merchandise["stock"] = merchandise["stock"].get<int>() - item["count"].get<int>();
							break;
						}
					}
				}
				saveMerchandises();
                saveUsers();
				return;
            }
        }
    }
	// 查看我的订单
	void viewMyOrders(const json& request, json& response)const {
		string username = request["username"];
		for (const auto& user : allUsers) {
			if (user["username"] == username) {
				response["status"] = "success";
				response["orders"] = user["orders"];
				return;
			}
		}
		response["status"] = "error";
		response["message"] = "User not found.";
	}
	// 支付订单
	void payForTheOrder(const json& request, json& response)const {
		string username = request["username"];
		for (auto& user : allUsers) {
			if (user["username"] == username) {
				for (const auto& order : user["orders"]) {
					for (const auto& item : order) {
						int ownerID = item["ownerid"].get<int>();
                        for (auto& merchant : allUsers) {
							if (merchant["id"] == ownerID) {
								merchant["balance"] = merchant["balance"].get<double>() + item["price"].get<double>() ;
								saveUsers();
                                break;
							}
                        }
					}
				}
				user["orders"].clear();
				response["status"] = "success";
				response["message"] = "Payment successful.";
				saveUsers();
				return;
			}
		}
	}
	// 取消我的订单
	void denyMyOrders(const json& request, json& response)const {
		string username = request["username"];
		for (auto& user : allUsers) {
			if (user["username"] == username) {
				for (const auto& order : user["orders"]) {
					for (const auto& item : order) {
						user["balance"] = user["balance"].get<double>() + item["price"].get<double>();
						for (auto& merchandise : allMerchandises) {
							if (merchandise["id"] == item["id"]) {
								merchandise["stock"] = merchandise["stock"].get<int>() + item["count"].get<int>();
								break;
							}
						}
					}
				}
				user["orders"].clear();
				saveMerchandises();
				saveUsers();
				response["status"] = "success";
				response["message"] = "Order cancelled successfully.";
				return;
			}
		}
	}
	// 添加商品
	void addMerchandise(const json& request, json& response)const {
		json newMerchandise;
		newMerchandise["category"] = request["category"];
		newMerchandise["description"] = request["description"];
		newMerchandise["id"] = merchandiseID["total"].get<int>() + 1;
		merchandiseID["total"] = merchandiseID["total"].get<int>() + 1;
		newMerchandise["name"] = request["name"];
		newMerchandise["ownerid"] = request["ownerid"];
		newMerchandise["price"] = request["price"];
		newMerchandise["stock"] = request["stock"];
		allMerchandises.push_back(newMerchandise);
		saveMerchandises();
		response["status"] = "success";
		response["message"] = "Merchandise added successfully.";
	}
	// 删除商品
	void removeMerchandise(const json& request, json& response)const {
		int id = request["ownerid"];
		string name = request["name"];
		for (auto it = allMerchandises.begin(); it != allMerchandises.end(); ++it) {
			if ((*it)["name"] == name && (*it)["ownerid"]==id ) {
				allMerchandises.erase(it);
				response["status"] = "success";
				response["message"] = "Merchandise removed successfully.";
				saveMerchandises();
				return;
			}
		}
		response["status"] = "error";
		response["message"] = "Merchandise not found.";
	}
	// 修改商品价格
	void modifyMerchandisePrice(const json& request, json& response)const {
		int id = request["ownerid"];
		double newPrice = request["price"];
		string name = request["name"];
		for (auto& merchandise : allMerchandises) {
			if (merchandise["name"] == name && merchandise["ownerid"] == id) {
				merchandise["price"] = newPrice;
				response["status"] = "success";
				response["message"] = "Price updated successfully.";
				saveMerchandises();
				return;
			}
		}
		response["status"] = "error";
		response["message"] = "Merchandise not found.";
	}
	// 补货
	void stockpile(const json& request, json& response)const {
		int id = request["ownerid"];
		string name = request["name"];
		for (auto& merchandise : allMerchandises) {
			if (merchandise["name"] == name && merchandise["ownerid"]==id ) {
				merchandise["stock"] = merchandise["stock"].get<int>() + request["stock"].get<int>();
				response["status"] = "success";
				response["message"] = "Stock updated successfully.";
				saveMerchandises();
				return;
			}
		}
		response["status"] = "error";
		response["message"] = "Merchandise not found.";
	}
	// 按类别打折
	void discountByCategory(const json& request, json& response)const {
		string category = request["category"];
		double discount = request["discount"];
		int id = request["ownerid"];
		for (auto& merchandise : allMerchandises) {
			if (merchandise["category"] == category && merchandise["ownerid"]==id ) {
				merchandise["price"] = merchandise["price"].get<double>() * discount;
			}
		}
		response["status"] = "success";
		response["message"] = "Discount applied successfully.";
		saveMerchandises();
	}
	// 查看我的商品
	void viewMyMerchandise(const json& request, json& response)const {
		int id = request["ownerid"];
		for (const auto& merchandise : allMerchandises) {
			if (merchandise["ownerid"] == id) {
				response["status"] = "success";
				response["merchandises"].push_back(merchandise);
			}
		}
	}
	// 发送响应给客户端
    void sendResponse(SOCKET clientSocket, const json& response) const {
        string responseStr = response.dump();
        int sendSize = send(clientSocket, responseStr.c_str(), responseStr.length(), 0);
        if (sendSize == SOCKET_ERROR) {
            cerr << "send failed: " << WSAGetLastError() << endl;
        }
    }
    // 处理客户端请求
    void handleRequest(SOCKET clientSocket) {
        try {
			//加入循环确保能够一直处理客户端请求
            while (true) {
                char buffer[1024];
                int recvSize = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                if (recvSize == SOCKET_ERROR) {
                    cerr << "recv failed: " << WSAGetLastError() << endl;
                    break;
                }
                if (recvSize == 0) {
                    // 客户端关闭连接
                    break;
                }
                buffer[recvSize] = '\0';
                json request = json::parse(buffer);
                string action = request.value("action", "");
                json response;
                if (action == "register") {
                    registerAccount(request, response);
                }
                else if (action == "login") {
                    loginAccount(request, response);
                }
                else if (action == "viewMerchandise") {
                    viewMerchandise(response);
                }
                else if (action == "changePassword") {
					changePassword(request, response);
                }
                else if (action == "balanceInquiry") {
                    balanceInquiry(request, response);
                }
                else if (action == "recharge") {
                    recharge(request, response);
                }
                else if (action == "searchForMerchandise") {
                    searchForMerchandise(request, response);
                }
				else if (action == "addToShoppingTrolley") {
					addToShoppingTrolley(request, response);
				}
				else if (action == "viewMyShoppingTrolley") {
					viewMyShoppingTrolley(request, response);
				}
                else if (action == "removeFromShoppingTrolley") {
                    removeFromShoppingTrolley(request, response);
                }
				else if (action == "clearShoppingTrolley") {
					clearShoppingTrolley(request, response);
				}
                else if (action == "generateOrder") {
                    generateOrder(request, response);
                }
				else if (action == "viewMyOrders") {
					viewMyOrders(request, response);
				}
                else if (action == "payForTheOrder") {
                    payForTheOrder(request, response);
                }
				else if (action == "denyMyOrders") {
					denyMyOrders(request, response);
				}
				else if (action == "addMerchandise") {
					addMerchandise(request, response);
				}
				else if (action == "removeMerchandise") {
					removeMerchandise(request, response);
				}
				else if (action == "modifyMerchandisePrice") {
					modifyMerchandisePrice(request, response);
				}
				else if (action == "stockpile") {
					stockpile(request, response);
				}
				else if (action == "discountByCategory") {
					discountByCategory(request, response);
				}
				else if (action == "viewMyMerchandise") {
					viewMyMerchandise(request, response);
				}
                else {
                    response["status"] = "error";
                    response["message"] = "Unknown action type!";
                }
                sendResponse(clientSocket, response);
            }
        }
        catch (const json::parse_error& e) {
            json response;
            response["status"] = "error";
            response["message"] = "Invalid JSON data!";
            sendResponse(clientSocket, response);
            cerr << "JSON parse error: " << e.what() << endl;
        }
        catch (const exception& e) {
            json response;
            response["status"] = "error";
            response["message"] = "Internal server error!";
            sendResponse(clientSocket, response);
            cerr << "Unexpected error: " << e.what() << endl;
        }
		saveMerchandises();
		saveUsers();
        closesocket(clientSocket);
    }
};
int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	//初始化Winsock
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "WSAStartup failed!\n" ;
		return 1;
	}
	//创建套接字
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		cout << "Socket creation failed!\n";
		WSACleanup();
		return 1;
	}
	//绑定地址和端口
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(8888);
	if (::bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cout << "Bind failed!\n";
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	//监听连接
	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		cout << "Listen failed!\n";
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	cout << "Server started on port 8888\n";

	basicOperations bo;
	bo.loadUsers();
	bo.loadMerchandises();
    while (true) {
        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Accept failed: " << WSAGetLastError() << endl;
            continue;
        }
        // 使用多线程处理客户端请求
        thread(&basicOperations::handleRequest, &bo, clientSocket).detach();
    }
    // 关闭服务器套接字
    closesocket(serverSocket);
    WSACleanup();
    bo.saveUsers();
    bo.saveMerchandises();

	return 0;
}