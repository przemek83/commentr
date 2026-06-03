// AI generated sample

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100
#define MAX_ADDR_LEN 200
#define MAX_ACCOUNTS 100

typedef struct {
    char name[MAX_NAME_LEN];
    char address[MAX_ADDR_LEN];
} Customer;

typedef struct {
    int accountNumber;
    Customer customer;
    double balance;
} Account;

typedef struct {
    char name[MAX_NAME_LEN];
    Account accounts[MAX_ACCOUNTS];
    int accountCount;
    int accountNumberCounter;
} Bank;

// ------------------------- Customer -------------------------

Customer createCustomer(const char* name, const char* address) {
    Customer c;
    strncpy(c.name, name, MAX_NAME_LEN);
    strncpy(c.address, address, MAX_ADDR_LEN);
    return c;
}

// ------------------------- Account -------------------------

Account createAccount(Customer customer, double initialDeposit, int accountNumber) {
    Account a;
    a.accountNumber = accountNumber;
    a.customer = customer;
    a.balance = initialDeposit;
    return a;
}

void deposit(Account* account, double amount) {
    account->balance += amount;
    printf("Deposited %.2f. New balance: %.2f\n", amount, account->balance);
}

void withdraw(Account* account, double amount) {
    if (amount > account->balance) {
        printf("Insufficient funds.\n");
    } else {
        account->balance -= amount;
        printf("Withdrew %.2f. New balance: %.2f\n", amount, account->balance);
    }
}

void displayAccountDetails(const Account* account) {
    printf("Account Number: %d\n", account->accountNumber);
    printf("Customer Name: %s\n", account->customer.name);
    printf("Account Balance: %.2f\n", account->balance);
}

// ------------------------- Bank -------------------------

Bank createBank(const char* name) {
    Bank b;
    strncpy(b.name, name, MAX_NAME_LEN);
    b.accountCount = 0;
    b.accountNumberCounter = 1000;
    return b;
}

int bankCreateAccount(Bank* bank, Customer customer, double initialDeposit) {
    if (bank->accountCount >= MAX_ACCOUNTS) {
        printf("Bank account limit reached.\n");
        return -1;
    }

    int newAccountNumber = ++bank->accountNumberCounter;
    bank->accounts[bank->accountCount] =
        createAccount(customer, initialDeposit, newAccountNumber);

    bank->accountCount++;
    return newAccountNumber;
}

Account* findAccount(Bank* bank, int accountNumber) {
    for (int i = 0; i < bank->accountCount; i++) {
        if (bank->accounts[i].accountNumber == accountNumber) {
            return &bank->accounts[i];
        }
    }
    return NULL;
}

void bankDeposit(Bank* bank, int accountNumber, double amount) {
    Account* acc = findAccount(bank, accountNumber);
    if (acc) deposit(acc, amount);
    else printf("Account not found.\n");
}

void bankWithdraw(Bank* bank, int accountNumber, double amount) {
    Account* acc = findAccount(bank, accountNumber);
    if (acc) withdraw(acc, amount);
    else printf("Account not found.\n");
}

void bankDisplayAccountDetails(Bank* bank, int accountNumber) {
    Account* acc = findAccount(bank, accountNumber);
    if (acc) displayAccountDetails(acc);
    else printf("Account not found.\n");
}

// ------------------------- Main -------------------------

int main() {
    Bank bank = createBank("Copilot Bank");

    Customer john = createCustomer("John Doe", "123 Elm Street");
    Customer jane = createCustomer("Jane Doe", "456 Oak Street");

    int johnAcc = bankCreateAccount(&bank, john, 500);
    int janeAcc = bankCreateAccount(&bank, jane, 1000);

    bankDeposit(&bank, johnAcc, 300);
    bankWithdraw(&bank, johnAcc, 200);
    bankDeposit(&bank, janeAcc, 500);
    bankWithdraw(&bank, janeAcc, 1500);

    bankDisplayAccountDetails(&bank, johnAcc);
    bankDisplayAccountDetails(&bank, janeAcc);

    return 0;
}
