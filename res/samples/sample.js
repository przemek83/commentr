// AI generated sample

class Customer {
    constructor(name, address) {
        this.name = name;
        this.address = address;
    }

    getName() {
        return this.name;
    }
}

class Account {
    static accountNumberCounter = 1000;

    constructor(customer, initialDeposit) {
        this.customer = customer;
        this.balance = initialDeposit;
        this.accountNumber = Account.accountNumberCounter++;
    }

    deposit(amount) {
        this.balance += amount;
        console.log(`Deposited ${amount}. New balance: ${this.balance}`);
    }

    withdraw(amount) {
        if (amount > this.balance) {
            console.log("Insufficient funds.");
        } else {
            this.balance -= amount;
            console.log(`Withdrew ${amount}. New balance: ${this.balance}`);
        }
    }

    displayDetails() {
        console.log(`Account Number: ${this.accountNumber}`);
        console.log(`Customer Name: ${this.customer.getName()}`);
        console.log(`Account Balance: ${this.balance}`);
    }
}

class Bank {
    constructor(name) {
        this.name = name;
        this.accounts = new Map();
    }

    createAccount(customer, initialDeposit) {
        const account = new Account(customer, initialDeposit);
        this.accounts.set(account.accountNumber, account);
        return account.accountNumber;
    }

    deposit(accountNumber, amount) {
        const account = this.accounts.get(accountNumber);
        if (account) {
            account.deposit(amount);
        } else {
            console.log("Account not found.");
        }
    }

    withdraw(accountNumber, amount) {
        const account = this.accounts.get(accountNumber);
        if (account) {
            account.withdraw(amount);
        } else {
            console.log("Account not found.");
        }
    }

    displayAccountDetails(accountNumber) {
        const account = this.accounts.get(accountNumber);
        if (account) {
            account.displayDetails();
        } else {
            console.log("Account not found.");
        }
    }
}

// Example usage:
const bank = new Bank("Copilot Bank");

// Create customers
const john = new Customer("John Doe", "123 Elm Street");
const jane = new Customer("Jane Doe", "456 Oak Street");

// Create accounts
const johnsAccount = bank.createAccount(john, 500);
const janesAccount = bank.createAccount(jane, 1000);

// Make some transactions
bank.deposit(johnsAccount, 300);
bank.withdraw(johnsAccount, 200);
bank.deposit(janesAccount, 500);
bank.withdraw(janesAccount, 1500);

// Display account details
bank.displayAccountDetails(johnsAccount);
bank.displayAccountDetails(janesAccount);
