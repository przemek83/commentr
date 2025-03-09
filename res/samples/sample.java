// AI generated sample

public class Bank {
    private String name;
    private HashMap<Integer, Account> accounts;

    public Bank(String name) {
        this.name = name;
        this.accounts = new HashMap<>();
    }

    public int createAccount(Customer customer, double initialDeposit) {
        Account account = new Account(customer, initialDeposit);
        accounts.put(account.getAccountNumber(), account);
        return account.getAccountNumber();
    }

    public void deposit(int accountNumber, double amount) {
        Account account = accounts.get(accountNumber);
        if (account != null) {
            account.deposit(amount);
        } else {
            System.out.println("Account not found.");
        }
    }

    public void withdraw(int accountNumber, double amount) {
        Account account = accounts.get(accountNumber);
        if (account != null) {
            account.withdraw(amount);
        } else {
            System.out.println("Account not found.");
        }
    }

    public void displayAccountDetails(int accountNumber) {
        Account account = accounts.get(accountNumber);
        if (account != null) {
            account.displayDetails();
        } else {
            System.out.println("Account not found.");
        }
    }
}

public class Account {
    private static int accountNumberCounter = 1000;
    private Customer customer;
    private double balance;
    private int accountNumber;

    public Account(Customer customer, double initialDeposit) {
        this.customer = customer;
        this.balance = initialDeposit;
        this.accountNumber = accountNumberCounter++;
    }

    public void deposit(double amount) {
        balance += amount;
        System.out.println("Deposited " + amount + ". New balance: " + balance);
    }

    public void withdraw(double amount) {
        if (amount > balance) {
            System.out.println("Insufficient funds.");
        } else {
            balance -= amount;
            System.out.println("Withdrew " + amount + ". New balance: " + balance);
        }
    }

    public void displayDetails() {
        System.out.println("Account Number: " + accountNumber);
        System.out.println("Customer Name: " + customer.getName());
        System.out.println("Account Balance: " + balance);
    }

    public int getAccountNumber() {
        return accountNumber;
    }
}

public class Customer {
    private String name;
    private String address;

    public Customer(String name, String address) {
        this.name = name;
        this.address = address;
    }

    public String getName() {
        return name;
    }
}

// Example usage:
public class Main {
    public static void main(String[] args) {
        Bank bank = new Bank("Copilot Bank");

        // Create customers
        Customer john = new Customer("John Doe", "123 Elm Street");
        Customer jane = new Customer("Jane Doe", "456 Oak Street");

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
    }
}
