<?php

// AI generated sample

class Customer {
    private $name;
    private $address;

    public function __construct($name, $address) {
        $this->name = $name;
        $this->address = $address;
    }

    public function getName() {
        return $this->name;
    }
}

class Account {
    private static $accountNumberCounter = 1000;
    private $accountNumber;
    private $customer;
    private $balance;

    public function __construct($customer, $initialDeposit) {
        $this->customer = $customer;
        $this->balance = $initialDeposit;
        $this->accountNumber = self::$accountNumberCounter++;
    }

    public function deposit($amount) {
        $this->balance += $amount;
        echo "Deposited $amount. New balance: $this->balance\n";
    }

    public function withdraw($amount) {
        if ($amount > $this->balance) {
            echo "Insufficient funds.\n";
        } else {
            $this->balance -= $amount;
            echo "Withdrew $amount. New balance: $this->balance\n";
        }
    }

    public function displayDetails() {
        echo "Account Number: $this->accountNumber\n";
        echo "Customer Name: " . $this->customer->getName() . "\n";
        echo "Account Balance: $this->balance\n";
    }

    public function getAccountNumber() {
        return $this->accountNumber;
    }
}

class Bank {
    private $name;
    private $accounts;

    public function __construct($name) {
        $this->name = $name;
        $this->accounts = array();
    }

    public function createAccount($customer, $initialDeposit) {
        $account = new Account($customer, $initialDeposit);
        $this->accounts[$account->getAccountNumber()] = $account;
        return $account->getAccountNumber();
    }

    public function deposit($accountNumber, $amount) {
        if (isset($this->accounts[$accountNumber])) {
            $this->accounts[$accountNumber]->deposit($amount);
        } else {
            echo "Account not found.\n";
        }
    }

    public function withdraw($accountNumber, $amount) {
        if (isset($this->accounts[$accountNumber])) {
            $this->accounts[$accountNumber]->withdraw($amount);
        } else {
            echo "Account not found.\n";
        }
    }

    public function displayAccountDetails($accountNumber) {
        if (isset($this->accounts[$accountNumber])) {
            $this->accounts[$accountNumber]->displayDetails();
        } else {
            echo "Account not found.\n";
        }
    }
}

// Example usage:
$bank = new Bank("Copilot Bank");

// Create customers
$john = new Customer("John Doe", "123 Elm Street");
$jane = new Customer("Jane Doe", "456 Oak Street");

// Create accounts
$johnsAccount = $bank->createAccount($john, 500);
$janesAccount = $bank->createAccount($jane, 1000);

// Make some transactions
$bank->deposit($johnsAccount, 300);
$bank->withdraw($johnsAccount, 200);
$bank->deposit($janesAccount, 500);
$bank->withdraw($janesAccount, 1500);

// Display account details
$bank->displayAccountDetails($johnsAccount);
$bank->displayAccountDetails($janesAccount);
?>
