#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
#include <windows.h>
#include <stdexcept>
using namespace std;

const char* HOST = "localhost";
const char* USER = "root";
const char* PW = "your password"; 
const char* DB = "mydb";

class Login {
private:
    string userId, userPW;
public:
    Login() : userId(""), userPW("") {}

    void setId(string id) {
        userId = id;
    }

    void setPW(string pw) {
        userPW = pw;
    }

    string getId() const {
        return userId;
    }

    string getPW() const {
        return userPW;
    }
};

char encryptCh(char ch, int shift) {
    if (isalpha(ch)) {
        char base = isupper(ch) ? 'A' : 'a';
        return (ch - base + shift + 26) % 26 + base;
    }
    if (isdigit(ch)) {
        return (ch - '0' + shift + 10) % 10 + '0';
    }
    return ch;
}

string encrypt(const string& password, int shift) {
    string encrypted = "";
    for (char ch : password) {
        encrypted += encryptCh(ch, shift);
    }
    return encrypted;
}

char decryptCh(char ch, int shift) {
    if (isalpha(ch)) {
        char base = isupper(ch) ? 'A' : 'a';
        return (ch - base - shift + 26) % 26 + base;
    }
    if (isdigit(ch)) {
        return (ch - '0' - shift + 10) % 10 + '0';
    }
    return ch;
}

string decrypt(const string& encrypted, int shift) {
    string decrypted = "";
    for (char ch : encrypted) {
        decrypted += decryptCh(ch, shift);
    }
    return decrypted;
}

string DBpw(MYSQL* conn, const string& id) {
    string encryptedPW;
    string get = "SELECT PW FROM password WHERE Id=?";
    
    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    if (!stmt) {
        throw runtime_error("mysql_stmt_init() failed");
    }
    
    if (mysql_stmt_prepare(stmt, get.c_str(), get.length())) {
        throw runtime_error(mysql_stmt_error(stmt));
    }
    
    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));
    
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (char*)id.c_str();
    bind[0].buffer_length = id.length();
    
    if (mysql_stmt_bind_param(stmt, bind)) {
        throw runtime_error(mysql_stmt_error(stmt));
    }
    
    if (mysql_stmt_execute(stmt)) {
        throw runtime_error(mysql_stmt_error(stmt));
    }
    
    MYSQL_BIND result[1];
    memset(result, 0, sizeof(result));
    char pwBuffer[256];
    result[0].buffer_type = MYSQL_TYPE_STRING;
    result[0].buffer = pwBuffer;
    result[0].buffer_length = sizeof(pwBuffer);
    
    if (mysql_stmt_bind_result(stmt, result)) {
        throw runtime_error(mysql_stmt_error(stmt));
    }
    
    if (mysql_stmt_fetch(stmt) == 0) {
        encryptedPW = pwBuffer;
    }
    
    mysql_stmt_close(stmt);
    return encryptedPW;
}

int main() {
    Login l;

    MYSQL* conn = mysql_init(nullptr);
    if (!mysql_real_connect(conn, HOST, USER, PW, DB, 3306, nullptr, 0)) {
        cout << "Error: " << mysql_error(conn) << endl;
        return 1;
    } else {
        cout << "Logged In Database!" << endl;
    }
    Sleep(3000);

    int shift = 3;
    bool exit = false;

    while (!exit) {
        system("cls");
        cout << "1. Signup." << endl;
        cout << "2. Login." << endl;
        cout << "0. Exit." << endl;
        cout << "Enter Your Choice: ";
        int val;
        cin >> val;

        if (val == 1) {
            system("cls");
            string id, pw;
            cout << "Enter ID For Signup: ";
            cin >> id;
            l.setId(id);
            cout << "Enter A Strong Password: ";
            cin >> pw;
            l.setPW(pw);

            string encryptedPW = encrypt(l.getPW(), shift);

            string Sup = "INSERT INTO password (Id, PW) VALUES (?, ?)";
            MYSQL_STMT* stmt = mysql_stmt_init(conn);
            if (!stmt) {
                cout << "Error: " << mysql_stmt_error(stmt) << endl;
            } else {
                if (mysql_stmt_prepare(stmt, Sup.c_str(), Sup.length())) {
                    cout << "Error: " << mysql_stmt_error(stmt) << endl;
                } else {
                    MYSQL_BIND bind[2];
                    memset(bind, 0, sizeof(bind));
                    
                    bind[0].buffer_type = MYSQL_TYPE_STRING;
                    bind[0].buffer = (char*)id.c_str();
                    bind[0].buffer_length = id.length();
                    
                    bind[1].buffer_type = MYSQL_TYPE_STRING;
                    bind[1].buffer = (char*)encryptedPW.c_str();
                    bind[1].buffer_length = encryptedPW.length();
                    
                    if (mysql_stmt_bind_param(stmt, bind)) {
                        cout << "Error: " << mysql_stmt_error(stmt) << endl;
                    } else {
                        if (mysql_stmt_execute(stmt)) {
                            cout << "Error: " << mysql_stmt_error(stmt) << endl;
                        } else {
                            cout << "Signup Successfully" << endl;
                        }
                    }
                }
                mysql_stmt_close(stmt);
            }
            Sleep(3000);
        } else if (val == 2) {
            system("cls");
            string id, pw;
            cout << "Enter ID: ";
            cin >> id;
            cout << "Enter Your password: ";
            cin >> pw;

            try {
                string getDB = DBpw(conn, id);
                if (!getDB.empty()) {
                    string decryptedPW = decrypt(getDB, shift);
                    if (decryptedPW == pw) {
                        cout << "Welcome" << endl;
                    } else {
                        cout << "Incorrect Password. Try Again!" << endl;
                    }
                } else {
                    cout << "User ID not found. Try Again!" << endl;
                }
            } catch (const exception& ex) {
                cout << "Error: " << ex.what() << endl;
            }
            Sleep(5000);
        } else if (val == 0) {
            exit = true;
            cout << "Bye!" << endl;
        } else {
            cout << "Invalid Input" << endl;
        }
    }

    mysql_close(conn);
    return 0;
}
