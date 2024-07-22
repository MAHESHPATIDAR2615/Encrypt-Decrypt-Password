# Encrypt-Decrypt-Password
# Simple Login System with MySQL

This project is a simple C++ application for user login and signup, utilizing MySQL for storing user credentials securely. It demonstrates basic encryption and decryption for storing passwords and the use of prepared statements to prevent SQL injection.

## Features

- User Signup: Users can create an account by providing an ID and a strong password.
- User Login: Users can log in by entering their ID and password.
- Password Encryption: Passwords are encrypted before being stored in the database.
- Secure Database Operations: Uses prepared statements to prevent SQL injection.

## Prerequisites

- MySQL Server
- MySQL C++ Connector
- C++17 or later
- Windows (the current implementation uses Windows-specific headers)
- The encrypt function processes each character of the original password, applying a shift according to whether the character is a letter or digit. The resulting encrypted string is formed by appending each shifted character from the original password. Because the encryption process involves only character shifting without adding or removing any characters, the length of the encrypted password string remains identical to that of the original password string.

## Getting Started

### MySQL Setup

1. Install MySQL Server.
2. Create a database named `mydb`.
3. Create a table named `password` with the following structure:

    ```sql
    CREATE TABLE password (
        Id VARCHAR(255) PRIMARY KEY,
        PW VARCHAR(255)
    );
    ```

### Project Setup

1. Clone the repository:

    ```bash
    git clone https://github.com/yourusername/simple-login-system.git
    cd simple-login-system
    ```

2. Update the MySQL credentials in the `main.cpp` file:

    ```cpp
    const char* HOST = "localhost";
    const char* USER = "root";
    const char* PW = "yourpassword"; 
    const char* DB = "mydb";
    ```

3. Compile the program:

    Ensure you have the MySQL C++ Connector installed and linked during compilation.

    ```bash
    g++ -o login_system main.cpp -lmysqlcppconn
    ```

4. Run the program:

    ```bash
    ./login_system
    ```

## Usage

When you run the program, you will see the following menu:
1.Signup.
2.Login.
3.Exit.
Enter Your Choice:

- Select `1` to create a new account.
- Select `2` to log in with an existing account.
- Select `0` to exit the program.

## Contributing

Contributions are welcome! Please create a pull request with a detailed description of your changes.


