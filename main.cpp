#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <ctime>
#include "console.h"

using namespace std;

const string USER_ACCOUNT_FILE = "useraccounts.txt";
const string ADMIN_ACCOUNT_FILE = "adminaccounts.txt";
const int TRANSACTION_LIMIT = 3;
const int wrongLimit = 3;

struct User{
    string id;
    string pin;
    string name;
    int balance;
};

struct Admin{
    string id;
    string pin;
    string name;
};

struct Transaction{
    string id;
    int amount;
};

vector<User> readAccountFromFile(const string& fileName);
void writeToFile (const string& data, const string& fileName);
bool isUser(const string& id,const string& pin);
bool isAdmin(const string& id,const string& pin);
bool deposit (const string& id,const int& amount,vector<User>& accounts);
bool withdraw (const string& id,const int& amount,vector<User>& accounts);
int getBalance(const string& id,const vector<User>& accounts);
bool exceedLimit (const string& id, const vector<Transaction>& transHistory);
void recordTransaction (const string& id, const int& amount, vector<Transaction>& trans);
void saveTransactionHistory (const string& id,const vector<Transaction>& transHistory);
int execution(const string& id,vector<User>& accounts,vector<Transaction>& transHistory,const int& option);
void updateData(vector<User>& accounts);
void printMoney(const int& amount);



int main()
{
    initConsole();
    int isOn = 0;
    vector<Transaction>transHistory;
    vector<User>accounts;
    string id,pin;
    string openID,openPin;
    int wrongTimes = 0;
    while(1){
        header();
        cout << "      ĐĂNG NHẬP BẰNG TÀI KHOẢN ADMIN ĐỂ KHỞI ĐỘNG THIẾT BỊ\n";
        cout << "Nhập ID Admin: ";
        cin >> id;
        cout << "Nhập mã PIN: ";
        cin >> pin;
        if (isAdmin(id,pin)){
            openID = id;
            openPin = pin;
            accounts = readAccountFromFile(USER_ACCOUNT_FILE);
            cout << " ===============================================================" << '\n';
            cout << "                           THÔNG BÁO!\n";
            cout << "                 KHỞI ĐỘNG THIẾT BỊ THÀNH CÔNG!\n";
            Sleep(2000);
            system("cls");
            break;
        }
        else{
            cout << "Tài khoản không hợp lệ\n";
            cout << "Xin mời nhập lại\n";
            wrongTimes++;
            if (wrongTimes >= wrongLimit){
                Sleep(1000);
                system("cls");
                header();
                cout << "                           THÔNG BÁO!\n";
                cout << "Tài khoản và mật khẩu sai vượt quá số lần cho phép!\n";
                cout << "Vì lý do bảo mật, thiết bị sẽ tự động thoát sau 3 giây nữa\n";
                saveTransactionHistory(id,transHistory);
                Sleep(3000);
                exit(0);
            }
            Sleep(1500);
        }
    }
    wrongTimes = 0;
    while (1){
        header();
        cout << "Nhập ID: ";
        cin >> id;
        cout << "Nhập mã PIN: ";
        cin >> pin;
        if (isUser(id,pin)){
            header();
            cout << "Đăng nhập thành công!\n";
            cout << "Mời bạn thực hiện giao dịch\n";
            Sleep(1500);
            header();
            int isRunning = true;
            int option = 0;
            while (isRunning){
                if (exceedLimit(id,transHistory)){
                    cout << "Bạn đã thực hiện quá số lần giao dịch trong 1 ngày\n";
                    cout << "Xin hãy quay lại vào hôm sau\n";
                    Sleep(2500);
                    break;
                }
                header();
                cout << "Xin mời bạn nhập lựa chọn giao dịch\n";
                cout << "1. Nộp tiền\n";
                cout << "2. Rút tiền\n";
                cout << "3. Kiểm tra số dư\n";
                cout << "0. Thoát\n";
                cout << "Lựa chọn của quý khách là: ";
                cin >> option;
                int result = execution(id,accounts,transHistory,option);
                if (result == 0)    break;
                else if (result > 0){
                    updateData(accounts);
                    cout << "Bạn có muốn tiếp tục giao dịch? 1. Có   0. Không\n";
                    cout << "Lựa chọn của bạn là: ";
                    cin >> isRunning;
                }
                else{
                    cout << "Giao dịch không hợp lệ:\n";
                }
                Sleep(1500);
                system("cls");
            }
            header();
            cout << "Kết thúc phiên giao dịch\n";
            cout << "Cảm ơn quý khách. Chúc quý khách một ngày tốt lành\n";
            Sleep(1000);
        }
        else if (isAdmin(id,pin) && id == openID && pin == openPin){
            cout << "Đang lưu lịch sử giao dịch...\n";
            saveTransactionHistory(id,transHistory);
            Sleep(3000);
            cout << "Lưu lịch sử giao dịch hoàn tất!\n";
            cout << "Tắt thiết bị\n";
            Sleep(1000);
            exit(0);
        }
        else{
            wrongTimes++;
            cout << "Tài khoản không hợp lệ!\n";
            if (wrongTimes >= wrongLimit){
                cout << "Tài khoản và mật khẩu sai vượt quá số lần cho phép!\n";
                cout << "Vì lý do bảo mật, thiết bị sẽ tự động thoát sau 3 giây nữa\n";
                saveTransactionHistory(id,transHistory);
            }
            Sleep(3000);
        }
        if (wrongTimes >= wrongLimit)   exit(0);
        system("cls");
    }
    return 0;
}

vector<User> readAccountFromFile(const string& fileName){
    ifstream inputFile(fileName.c_str());
    if (!inputFile.is_open()){
        cerr << "User's accounts file not found!";
        exit(1);
    }
    vector<User>accounts;
    string id,pin,name;
    int balance;
    while(inputFile >> id >> pin >> name >> balance){
        accounts.push_back({id,pin,name,balance});
    }
    inputFile.close();
    return accounts;
}
void writeToFile (const string& data,const string& fileName){
    ofstream outputFile(fileName.c_str());
    if (!outputFile.is_open()){
        cerr << "File not found!";
        exit(1);
    }
    outputFile << data;
    outputFile.close();
}
bool isUser(const string& id,const string& pin){
    vector<User>accounts = readAccountFromFile(USER_ACCOUNT_FILE);
    for (User account : accounts){
        if (account.id == id && account.pin == pin){
            return true;
        }
    }
    return false;
}
bool isAdmin(const string& id,const string& pin){
    ifstream inputFile(ADMIN_ACCOUNT_FILE.c_str());
    if (!inputFile.is_open()){
        cerr << "Admin's account file not found";
        exit(1);
    }
    string _id,_pin;
    while (inputFile >> _id >> _pin){
        if (id == _id && pin == _pin){
            return true;
        }
    }
    return false;
}
bool deposit (const string& id,const int& amount,vector<User>& accounts){
    if (amount <= 0)    return false;
    if (amount % 50000 != 0){
        cout << "Vui long nhap so tien la boi so cua 50000\n";
        return false;
    }
    for (User& account: accounts){
        if (account.id == id){
            account.balance += amount;
            return true;
        }
    }
    return false;
}
bool withdraw (const string& id,const int& amount,vector<User>& accounts){
    if (amount <= 0)    return false;
    if (amount % 50000 != 0){
        cout << "Vui long nhap so tien la boi so cua 50000\n";
        return false;
    }
    for (User& account: accounts){
        if (account.id == id && account.balance >= amount){
            account.balance -= amount;
            return true;
        }
    }
    return false;
}
int getBalance(const string& id,const vector<User>& accounts){
    for (const User& account : accounts){
        if (account.id == id){
            return account.balance;
        }
    }
    // Neu khong tim thay account
    return -1;
}
bool exceedLimit (const string& id, const vector<Transaction>& transHistory){
    int cnt = 1;
    for (Transaction trans : transHistory){
        if (trans.id == id && trans.amount != 0){
            ++cnt;
        }
    }
    return cnt > TRANSACTION_LIMIT;
}
void recordTransaction (const string& id, const int& amount, vector<Transaction>& trans){
    trans.push_back({id,amount});
}
void saveTransactionHistory (const string& id,const vector<Transaction>& transHistory){
    string fileName = "";
    time_t now = time(0);
    tm* t = localtime(&now);
    fileName = to_string(t->tm_mday)
    + "_" + to_string(t->tm_mon+1)
    + "_" + to_string(t->tm_year+1900)
    + "_" + id
    + ".txt";

    ofstream outputFile(fileName.c_str());
    if (outputFile.is_open()){
        for (Transaction trans : transHistory){
            outputFile << trans.id << " " << trans.amount << "\n";
        }
    }
    outputFile.close();
}
int execution(const string& id,vector<User>& accounts,vector<Transaction>& transHistory,const int& option){
    Sleep(1000);
    header();
    int amount = 0;
    switch (option){
        case 1:{
            cout << "Nhập số tiền muốn nộp: ";
            cin >> amount;
            if (deposit(id,amount,accounts)){
                cout << "Nộp tiền thành công!\n";
                cout << "Số tiền hiện tại: ";
                int money = getBalance(id,accounts);
                printMoney(money);
                recordTransaction(id,amount,transHistory);
            }
            else{
                cout << "Nộp tiền không thành công\n";
                cout << "Vui lòng thử lại!\n";
            }
            return 1;
        }
        case 2:{
            cout << "Số dư hiện tại: ";
            int money = getBalance(id,accounts);
            printMoney(money);
            cout << "Nhập số tiền muốn rút: ";
            cin >> amount;
            if (withdraw(id,amount,accounts)){
                cout << "Rút tiền thành công!\n";
                cout << "Số dư hiện tại: ";
                int money = getBalance(id,accounts);
                printMoney(money);
                recordTransaction(id,-1*amount,transHistory);
            }
            else{
                cout << "Rút tiền không thành công\n";
                cout << "Vui lòng thử lại!\n";
            }
            return 2;
        }
        case 3:{
            cout << "Số dư hiện tại: ";
            int money = getBalance(id,accounts);
            printMoney(money);
            recordTransaction(id,amount,transHistory);
            return 3;
        }
        case 0:{
            cout << "Kết thúc giao dịch!\n";
            return 0;
        }
        default:{
            cout << "Lựa chọn không hợp lệ!\n";
            return -1;
        }
    }
}
void updateData(vector<User>& accounts){
    string data = "";
    for (User x : accounts){
    data = data + x.id + " " + x.pin + " " + x.name + " " + to_string(x.balance) + "\n";
        }
    writeToFile(data,USER_ACCOUNT_FILE);
}
void printMoney(const int& amount){
    string s = to_string(amount);
    int p = 0;
    string r = "";
    for (int i = s.size()-1;i >= 0;--i){
        r += s[i];
        p++;
        if (p % 3 == 0 && i > 0) r += ".";
    }
    reverse(r.begin(),r.end());
    cout << r << '\n';
}
