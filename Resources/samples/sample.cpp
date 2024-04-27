// AI generated sample

#include <iostream>
#include <string>
#include <map>

class Customer {
private:
    std::string name;
    std::string address;

public:
    Customer(const std::string& name, const std::string& address) : name(name), address(address) {}

    std::string getName() const {
        return name;
    }
};

class Account {
private:
    static int accountNumberCounter;
    int accountNumber;
    Customer customer;
    double balance;

public:
    Account(const Customer& customer, double initialDeposit) : customer(customer), balance(initialDeposit) {
        accountNumber = ++accountNumberCounter;
    }

    void deposit(double amount) {
        balance += amount;
        std::cout << "Deposited " << amount << ". New balance: " << balance << std::endl;
    }

    void withdraw(double amount) {
        if (amount > balance) {
            std::cout << "Insufficient funds." << std::endl;
        } else {
            balance -= amount;
            std::cout << "Withdrew " << amount << ". New balance: " << balance << std::endl;
        }
    }

    void displayDetails() const {
        std::cout << "Account Number: " << accountNumber << std::endl;
        std::cout << "Customer Name: " << customer.getName() << std::endl;
        std::cout << "Account Balance: " << balance << std::endl;
    }

    int getAccountNumber() const {
        return accountNumber;
    }
};

int Account::accountNumberCounter = 1000;

class Bank {
private:
    std::string name;
    std::map<int, Account> accounts;

public:
    Bank(const std::string& name) : name(name) {}

    int createAccount(const Customer& customer, double initialDeposit) {
        Account account(customer, initialDeposit);
        accounts[account.getAccountNumber()] = account;
        return account.getAccountNumber();
    }

    void deposit(int accountNumber, double amount) {
        if (accounts.find(accountNumber) != accounts.end()) {
            accounts[accountNumber].deposit(amount);
        } else {
            std::cout << "Account not found." << std::endl;
        }
    }

    void withdraw(int accountNumber, double amount) {
        if (accounts.find(accountNumber) != accounts.end()) {
            accounts[accountNumber].withdraw(amount);
        } else {
            std::cout << "Account not found." << std::endl;
        }
    }

    void displayAccountDetails(int accountNumber) {
        if (accounts.find(accountNumber) != accounts.end()) {
            accounts[accountNumber].displayDetails();
        } else {
            std::cout << "Account not found." << std::endl;
        }
    }
};

// Example usage:
int main() {
    Bank bank("Copilot Bank");

    // Create customers
    Customer john("John Doe", "123 Elm Street");
    Customer jane("Jane Doe", "456 Oak Street");

    // Create accounts
    int johnsAccount = bank.createAccount(john, 500);
    int janesAccount = bank.createAccount(jane, 1000);

    // Make some transactions
    bank.deposit(johnsAccount, 300);
    bank.withdraw(johnsAccount, 200);
    bank.deposit(janesAccount, 500);
    bank.withdraw(janesAccount, 1500);

    // Display account details
    bank.displayAccountDetails(johnsAccount);
    bank.displayAccountDetails(janesAccount);

    return 0;
}
