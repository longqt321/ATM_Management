#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <time.h>
#include <ctime>

using namespace std;

const string USER_ACCOUNT_FILE = "useraccounts.txt";
const string ADMIN_ACCOUNT_FILE = "adminaccounts.txt";
const string TRANSACTION_HISTORY_FILE = "history.txt";
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
void displayInfo(const string &id,const vector<User>& accounts);
bool exceedLimit (const string& id, const vector<Transaction>& transHistory);
void recordTransaction (const string& id, const int& amount, vector<Transaction>& trans);
void saveTransactionHistory (const string& id,const vector<Transaction>& transHistory);
int execution(const string& id,vector<User>& accounts,vector<Transaction>& transHistory,const int& option);
void updateData(vector<User>& accounts);
void printMoney(const int& amount);


int main()
{
    int isOn = 0;
    vector<Transaction>transHistory;
    vector<User>accounts;
    string id,pin;
    string openID,openPin;
    while(1){
        cout << "Nhap id admin: ";
        cin >> id;
        cout << "Nhap pin: ";
        cin >> pin;
        if (isAdmin(id,pin)){
            openID = id;
            openPin = pin;
            accounts = readAccountFromFile(USER_ACCOUNT_FILE);
            cout << "Khoi dong thiet bi thanh cong!\n";
            cout << "Xin moi thuc hien giao dich!\n";
            Sleep(1000);
            system("cls");
            break;
        }
        else{
            cout << "Tai khoan khong hop le\n";
            cout << "Xin moi nhap lai\n";
            Sleep(1000);
            system("cls");
        }
    }
    int wrongTimes = 0;
    while (1){
        cout << "Nhap id: ";
        cin >> id;
        cout << "Nhap pin: ";
        cin >> pin;
        if (isUser(id,pin)){
            cout << "Dang nhap thanh cong!\n";
            cout << "Moi ban thuc hien giao dich\n";
            int isRunning = true;
            int option = 0;
            while (isRunning){
                if (exceedLimit(id,transHistory)){
                    cout << "Ban da thuc hien qua so lan giao dich trong 1 ngay\n";
                    cout << "Xin hay quay lai vao hom sau.\n";
                    Sleep(2500);
                    break;
                }
                cout << "1. Nop tien\n";
                cout << "2. Rut tien\n";
                cout << "3. Kiem tra so du\n";
                cout << "0. Thoat\n";
                cout << "Lua chon cua quy khach la: ";
                cin >> option;
                int result = execution(id,accounts,transHistory,option);
                // result == 0 ->
                if (result == 0)    break;
                else if (result > 0){
                    updateData(accounts);
                    cout << "Ban co muon tiep tuc giao dich? 1. Co   0. Khong\n";
                    cout << "Lua chon cua ban la: ";
                    cin >> isRunning;
                }
                else{
                    cout << "Giao dich khong hop le:\n";
                }
                Sleep(1500);
                system("cls");
            }
            cout << "Ket thuc phien giao dich\n";
            cout << "Cam on quy khach\n";
            Sleep(1000);
        }
        else if (isAdmin(id,pin) && id == openID && pin == openPin){
            cout << "Tat thiet bi\n";
            cout << "Luu lich su giao dich\n";
            saveTransactionHistory(id,transHistory);
            cout << "Hoan tat luu lich su giao dich!\n";
            exit(0);
        }
        else{
            wrongTimes++;
            cout << "Tai khoan khong hop le!\n";
            if (wrongTimes >= wrongLimit){
                cout << "Tai khoan va mat khau sai vuot qua so lan cho phep!\n";
                cout << "Ban da bi khoa tai khoan\n";
            }
            Sleep(1500);
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
    fileName = to_string(t->tm_sec)
    + "_" + to_string(t->tm_min)
    + "_" + to_string(t->tm_hour)
    + "_" + to_string(t->tm_mday)
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
    system("cls");
    int amount = 0;
    switch (option){
        case 1:{
            cout << "Xin moi nhap so tien muon nop: ";
            cin >> amount;
            if (deposit(id,amount,accounts)){
                cout << "Nop tien thanh cong!\n";
                cout << "So tien hien tai cua quy khach la: ";
                int money = getBalance(id,accounts);
                printMoney(money);
                recordTransaction(id,amount,transHistory);
            }
            else{
                cout << "Nop tien khong thanh cong\n";
                cout << "Vui long thu lai!\n";
            }
            return 1;
        }
        case 2:{
            cout << "So du hien tai cua ban la: ";
            int money = getBalance(id,accounts);
            printMoney(money);
            cout << "Xin moi nhap so tien muon rut: ";
            cin >> amount;
            if (withdraw(id,amount,accounts)){
                cout << "Rut tien thanh cong!\n";
                cout << "So tien hien tai cua quy khach la: ";
                int money = getBalance(id,accounts);
                printMoney(money);
                recordTransaction(id,-1*amount,transHistory);
            }
            else{
                cout << "Rut tien khong thanh cong\n";
                cout << "Vui long thu lai!\n";
            }
            return 2;
        }
        case 3:{
            cout << "So du hien tai cua ban la: ";
            int money = getBalance(id,accounts);
            printMoney(money);
            recordTransaction(id,amount,transHistory);
            return 3;
        }
        case 0:{
            cout << "Ket thuc giao dich!\n";
            return 0;
        }
        default:{
            cout << "Giao dich khong hop le!\n";
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
