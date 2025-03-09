# AI generated sample

class Bank:
    def __init__(self, name):
        self.name = name
        self.accounts = {}

    def create_account(self, customer, initial_deposit):
        account = Account(customer, initial_deposit)
        self.accounts[account.account_number] = account
        return account.account_number

    def deposit(self, account_number, amount):
        if account_number in self.accounts:
            self.accounts[account_number].deposit(amount)
        else:
            print("Account not found.")

    def withdraw(self, account_number, amount):
        if account_number in self.accounts:
            self.accounts[account_number].withdraw(amount)
        else:
            print("Account not found.")

    def display_account_details(self, account_number):
        if account_number in self.accounts:
            self.accounts[account_number].display_details()
        else:
            print("Account not found.")


class Account:
    account_number_counter = 1000

    def __init__(self, customer, initial_deposit):
        self.customer = customer
        self.balance = initial_deposit
        self.account_number = Account.account_number_counter
        Account.account_number_counter += 1

    def deposit(self, amount):
        self.balance += amount
        print(f"Deposited {amount}. New balance: {self.balance}")

    def withdraw(self, amount):
        if amount > self.balance:
            print("Insufficient funds.")
        else:
            self.balance -= amount
            print(f"Withdrew {amount}. New balance: {self.balance}")

    def display_details(self):
        print(f"Account Number: {self.account_number}")
        print(f"Customer Name: {self.customer.name}")
        print(f"Account Balance: {self.balance}")


class Customer:
    def __init__(self, name, address):
        self.name = name
        self.address = address


# Example usage:
bank = Bank("Copilot Bank")

# Create customers
john = Customer("John Doe", "123 Elm Street")
jane = Customer("Jane Doe", "456 Oak Street")

# Create accounts
johns_account = bank.create_account(john, 500)
janes_account = bank.create_account(jane, 1000)

# Make some transactions
bank.deposit(johns_account, 300)
bank.withdraw(johns_account, 200)
bank.deposit(janes_account, 500)
bank.withdraw(janes_account, 1500)

# Display account details
bank.display_account_details(johns_account)
bank.display_account_details(janes_account)
