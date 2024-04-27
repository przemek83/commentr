// AI generated sample

using System;
using System.Collections.Generic;

public class Bank {
    private string name;
    private Dictionary<int, Account> accounts;

    public Bank(string name) {
        this.name = name;
        this.accounts = new Dictionary<int, Account>();
    }

    public int CreateAccount(Customer customer, decimal initialDeposit) {
        Account account = new Account(customer, initialDeposit);
        accounts.Add(account.AccountNumber, account);
        return account.AccountNumber;
    }

    public void Deposit(int accountNumber, decimal amount) {
        if (accounts.TryGetValue(accountNumber, out Account account)) {
            account.Deposit(amount);
        } else {
            Console.WriteLine("Account not found.");
        }
    }

    public void Withdraw(int accountNumber, decimal amount) {
        if (accounts.TryGetValue(accountNumber, out Account account)) {
            account.Withdraw(amount);
        } else {
            Console.WriteLine("Account not found.");
        }
    }

    public void DisplayAccountDetails(int accountNumber) {
        if (accounts.TryGetValue(accountNumber, out Account account)) {
            account.DisplayDetails();
        } else {
            Console.WriteLine("Account not found.");
        }
    }
}

public class Account {
    private static int accountNumberCounter = 1000;
    public int AccountNumber { get; private set; }
    private Customer customer;
    private decimal balance;

    public Account(Customer customer, decimal initialDeposit) {
        this.customer = customer;
        this.balance = initialDeposit;
        this.AccountNumber = accountNumberCounter++;
    }

    public void Deposit(decimal amount) {
        balance += amount;
        Console.WriteLine($"Deposited {amount}. New balance: {balance}");
    }

    public void Withdraw(decimal amount) {
        if (amount > balance) {
            Console.WriteLine("Insufficient funds.");
        } else {
            balance -= amount;
            Console.WriteLine($"Withdrew {amount}. New balance: {balance}");
        }
    }

    public void DisplayDetails() {
        Console.WriteLine($"Account Number: {AccountNumber}");
        Console.WriteLine($"Customer Name: {customer.Name}");
        Console.WriteLine($"Account Balance: {balance}");
    }
}

public class Customer {
    public string Name { get; private set; }
    private string address;

    public Customer(string name, string address) {
        this.Name = name;
        this.address = address;
    }
}

// Example usage:
public class Program {
    public static void Main() {
        Bank bank = new Bank("Copilot Bank");

        // Create customers
        Customer john = new Customer("John Doe", "123 Elm Street");
        Customer jane = new Customer("Jane Doe", "456 Oak Street");

        // Create accounts
        int johnsAccount = bank.CreateAccount(john, 500);
        int janesAccount = bank.CreateAccount(jane, 1000);

        // Make some transactions
        bank.Deposit(johnsAccount, 300);
        bank.Withdraw(johnsAccount, 200);
        bank.Deposit(janesAccount, 500);
        bank.Withdraw(janesAccount, 1500);

        // Display account details
        bank.DisplayAccountDetails(johnsAccount);
        bank.DisplayAccountDetails(janesAccount);
    }
}
