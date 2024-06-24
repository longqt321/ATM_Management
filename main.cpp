#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include "console.h"

using namespace std;

const string USER_ACCOUNT_FILE = "useraccounts.txt";
const string ADMIN_ACCOUNT_FILE = "adminaccounts.txt";
const int TRANSACTION_LIMIT = 3;
const int WRONG_LIMIT = 3;

struct User{
    string id;
    string pin;
    string name;
    long long balance;

    bool operator<(const User& other) const{
        return id < other.id;
    }
};

struct Admin{
    string id;
    string pin;
};

struct Transaction{
    string id;
    long long amount;
};

vector<User> readAccountFromFile(const string& fileName);
void writeToFile (const string& data, const string& fileName);
int isUser(const string& id,const string& pin,const vector<User>& accounts);
bool isAdmin(const string& id,const string& pin);
bool deposit (const int& idx,const long long& amount,vector<User>& accounts);
bool withdraw (const int& idx,const long long& amount,vector<User>& accounts);
long long getBalance(const int& idx,const vector<User>& accounts);
bool exceedLimit (const string& id, const vector<Transaction>& transHistory);
void recordTransaction (const string& id, const long long& amount, vector<Transaction>& trans);
void saveTransactionHistory (const vector<Transaction>& transHistory);
int processTransaction(const int& idx,vector<User>& accounts,vector<Transaction>& transHistory,const int& option);
void updateData(vector<User>& accounts);
void printBalance(const long long& amount);
bool shutDown(const vector<Transaction>& transHistory);
void quicksort(vector<User>& accounts,int l,int r);
void initConsole();
void header();
bool turnOn();
void working();


int main()
{
    initConsole();
    if (turnOn() == false){
        exit(0);
    }
    working();
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
    long long balance;
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
int isUser(const string& id,const string& pin,const vector<User>& accounts){
    int left = 0, right = accounts.size()-1;
    while (left <= right){
        int mid = (left+right)/2;
        User acnt = accounts[mid];
        if (acnt.id == id && acnt.pin == pin){
            return mid;
        }
        else if (id < acnt.id){
            right = mid-1;
        }
        else{
            left = mid+1;
        }
    }
    return -1;
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
bool deposit (const int& idx,const long long& amount,vector<User>& accounts){
    if (amount <= 0)    return false;
    if (amount % 50000 != 0){
        cout << "Vui lòng nhập số tiền là bội số của 50.000\n";
        return false;
    }
    // index is valid -> there is a user with index idx
    if (idx != -1){
        accounts[idx].balance += amount;
        return true;
    }
    return false;
}
bool withdraw (const int& idx,const long long& amount,vector<User>& accounts){
    if (amount <= 0)    return false;
    if (amount % 50000 != 0){
        cout << "Vui lòng nhập số tiền là bội số của 50.000\n";
        return false;
    }
    if (amount > accounts[idx].balance){
        cout << "Số dư tài khoản không đủ\n";
        return false;
    }
    if (idx != -1){
        accounts[idx].balance -= amount;
        return true;
    }
    return false;
}
long long getBalance(const int& idx,const vector<User>& accounts){
    if (idx != -1){
        return accounts[idx].balance;
    }
    return -1;
}
bool exceedLimit (const string& id, const vector<Transaction>& transHistory){
    int cnt = 0;
    for (Transaction trans : transHistory){
        if (trans.id == id && trans.amount != 0){
            ++cnt;
        }
    }
    return cnt >= TRANSACTION_LIMIT;
}
void recordTransaction (const string& id, const long long& amount, vector<Transaction>& transHistory){
    transHistory.push_back({id,amount});
}
void saveTransactionHistory (const vector<Transaction>& transHistory){
    string fileName = "";
    time_t now = time(0);
    tm* t = localtime(&now);
    fileName = to_string(t->tm_mday)
    + "_" + to_string(t->tm_mon+1)
    + "_" + to_string(t->tm_year+1900)
    + ".txt";

    ofstream outputFile(fileName.c_str());
    if (outputFile.is_open()){
        for (Transaction trans : transHistory){
            outputFile << trans.id << " " << trans.amount << "\n";
        }
    }
    outputFile.close();
}
int processTransaction(const int& idx,vector<User>& accounts,vector<Transaction>& transHistory,const int& option){
    Sleep(1000);
    header();
    long long amount = 0;
    switch (option){
        case 1:{
            cout << "Nhập số tiền muốn nộp: ";
            cin >> amount;
            if (deposit(idx,amount,accounts)){
                cout << "Nộp tiền thành công!\n";
                long long money = getBalance(idx,accounts);
                printBalance(money);
                recordTransaction(accounts[idx].id,amount,transHistory);
            }
            else{
                cout << "Nộp tiền không thành công\n";
                cout << "Vui lòng thử lại!\n";
            }
            return 1;
        }
        case 2:{
            long long money = getBalance(idx,accounts);
            printBalance(money);
            cout << "Nhập số tiền muốn rút: ";
            cin >> amount;
            if (withdraw(idx,amount,accounts)){
                cout << "Rút tiền thành công!\n";
                long long money = getBalance(idx,accounts);
                printBalance(money);
                recordTransaction(accounts[idx].id,-1*amount,transHistory);
            }
            else{
                cout << "Rút tiền không thành công\n";
                cout << "Vui lòng thử lại!\n";
            }
            return 2;
        }
        case 3:{
            long long money = getBalance(idx,accounts);
            printBalance(money);
            recordTransaction(accounts[idx].id,amount,transHistory);
            return 3;
        }
        case 0:{
            cout << "Kết thúc giao dịch!\n";
            return 0;
        }
        default:{
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
void printBalance(const long long& amount){
    string s = to_string(amount);
    long long p = 0;
    string r = "";
    for (int i = s.size()-1;i >= 0;--i){
        r += s[i];
        p++;
        if (p % 3 == 0 && i > 0) r += ".";
    }
    reverse(r.begin(),r.end());
    cout << "Số dư hiện tại là: " << r << " VNĐ\n";
}
bool turnOn(){
    string id,pin;
    int wrongTimes = 0;
    while(1){
        header();
        cout << "      ĐĂNG NHẬP BẰNG TÀI KHOẢN ADMIN ĐỂ KHỞI ĐỘNG THIẾT BỊ\n";
        cout << "Nhập ID Admin: ";
        cin >> id;
        cout << "Nhập mã PIN: ";
        cin >> pin;
        if (isAdmin(id,pin)){
            cout << "================================================================" << '\n';
            cout << "                           THÔNG BÁO!\n";
            cout << "                 KHỞI ĐỘNG THIẾT BỊ THÀNH CÔNG!\n";
            Sleep(1500);
            system("cls");
            return true;
        }
        else{
            cout << "Tài khoản không hợp lệ\n";
            cout << "Xin mời nhập lại\n";
            wrongTimes++;
            if (wrongTimes >= WRONG_LIMIT){
                system("cls");
                header();
                cout << "                           THÔNG BÁO!\n";
                cout << "Tài khoản và mật khẩu sai vượt quá số lần cho phép!\n";
                cout << "Vì lý do bảo mật, thiết bị sẽ tự động thoát sau 3 giây nữa\n";
                Sleep(3000);
                return false;
            }
            Sleep(1500);
        }
    }
}
void working(){
    vector<Transaction>transHistory;
    vector<User>accounts = readAccountFromFile(USER_ACCOUNT_FILE);
    int n = accounts.size()-1;
    quicksort(accounts,0,n);
    string id,pin;
    while (1){
        int wrongTimes = 0;
        header();
        cout << "Nhập ID: ";
        cin >> id;
        cout << "Nhập mã PIN: ";
        cin >> pin;
        if (isUser(id,pin,accounts) != -1){
            header();
            int idx = isUser(id,pin,accounts);
            cout << "Đăng nhập thành công!\n";
            cout << "Mời bạn " << accounts[idx].name << " thực hiện giao dịch\n";
            Sleep(1500);
            header();
            int isRunning = true;
            int option = 0;
            while (isRunning){
                if (exceedLimit(id,transHistory)){
                    header();
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
                int result = processTransaction(idx,accounts,transHistory,option);
                if (result == 0)    break;
                else if (result > 0){
                    updateData(accounts);
                    cout << "Bạn có muốn tiếp tục giao dịch? 1. Có   0. Không\n";
                    cout << "Lựa chọn của bạn là: ";
                    cin >> isRunning;
                }
                else{
                    cout << "Lựa chọn không hợp lệ:\n";
                }
                Sleep(1500);
                system("cls");
            }
            header();
            cout << "Kết thúc phiên giao dịch\n";
            cout << "Cảm ơn quý khách. Chúc quý khách một ngày tốt lành\n";
            Sleep(1000);
        }
        else if (isAdmin(id,pin)){
            shutDown(transHistory);
        }
        else{
            wrongTimes++;
            cout << "Tài khoản không hợp lệ\n";
            cout << "Xin mời nhập lại\n";
            if (wrongTimes >= WRONG_LIMIT){
                cout << "                           THÔNG BÁO!\n";
                cout << "Tài khoản và mật khẩu sai vượt quá số lần cho phép!\n";
                cout << "Vì lý do bảo mật, thiết bị sẽ tự động thoát sau 3 giây nữa\n";
                saveTransactionHistory(transHistory);
                Sleep(3000);
                exit(0);
            }
            Sleep(1500);
        }
    }
}
bool shutDown(const vector<Transaction>& transHistory){
    cout << "Đang lưu lịch sử giao dịch...\n";
    saveTransactionHistory(transHistory);
    Sleep(2000);
    cout << "Lưu lịch sử giao dịch hoàn tất!\n";
    cout << "Tắt thiết bị\n";
    Sleep(1000);
    exit(0);
}
void initConsole(){
    SetConsoleOutputCP(65001);
    SetConsoleTitle(TEXT("ATM Management"));
}
void header() {
    system("cls");
    cout << " ______________________________________________________________ " << '\n';
    cout << "|                                                              |" << '\n';
    cout << "|            ĐỒ ÁN PBL1: DỰ ÁN LẬP TRÌNH TÍNH TOÁN             |" << '\n';
    cout << "|                                                              |" << '\n';
    cout << "|                ĐỀ TÀI: XÂY DỰNG ỨNG DỤNG                     |" << '\n';
    cout << "|              THỰC HIỆN GIAO DỊCH TRÊN MÁY ATM                |" << '\n';
    cout << "|                                                              |" << '\n';
    cout << "|        GVHD: Trần Hồ Thủy Tiên                               |" << '\n';
    cout << "|        Nhóm thực hiện:                                       |" << '\n';
    cout << "|              Trần Đức Long     (23T_Nhat1)  MSSV: 102230027  |" << '\n';
    cout << "|              Hoàng Vũ Tấn Phát (19TCLC_DT4) MSSV: 102190182  |" << '\n';
    cout << "|              Nguyễn Gia Khánh  (19TCLC_DT3) MSSV: 102190120  |" << '\n';
    cout << "|                                                              |" << '\n';
    cout << "|______________________________________________________________|" << '\n';
}
void quicksort(vector<User>& accounts,int l,int r){
    if (l > r) return;
    int left = l;
    int right = r;
    int mid = (left+right)/2;
    string pivot = accounts[mid].id;
    while (left <= right){
        while (vt[left].id < pivot) left++;
        while (vt[right].id > pivot) right--;
        if (left > right)  break;
        swap(accounts[left],accounts[right]);
        left++;
        right--;
    }
    if (left < r) quicksort(accounts,left,r);
    if (l < right)  quicksort(accounts,l,right);
}


