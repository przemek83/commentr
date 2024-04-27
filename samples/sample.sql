-- AI generated sample

-- Create Customer Table
CREATE TABLE Customers (
    CustomerID INT PRIMARY KEY IDENTITY,
    Name NVARCHAR(100),
    Address NVARCHAR(255)
);

-- Create Account Table
CREATE TABLE Accounts (
    AccountNumber INT PRIMARY KEY IDENTITY(1000,1),
    CustomerID INT FOREIGN KEY REFERENCES Customers(CustomerID),
    Balance DECIMAL(18,2)
);

-- Insert Customers
INSERT INTO Customers (Name, Address) VALUES ('John Doe', '123 Elm Street');
INSERT INTO Customers (Name, Address) VALUES ('Jane Doe', '456 Oak Street');

-- Stored Procedure to Create Account
CREATE PROCEDURE CreateAccount
    @CustomerID INT,
    @InitialDeposit DECIMAL(18,2)
AS
BEGIN
    INSERT INTO Accounts (CustomerID, Balance) VALUES (@CustomerID, @InitialDeposit);
END

-- Stored Procedure to Deposit
CREATE PROCEDURE Deposit
    @AccountNumber INT,
    @Amount DECIMAL(18,2)
AS
BEGIN
    UPDATE Accounts SET Balance = Balance + @Amount WHERE AccountNumber = @AccountNumber;
END

-- Stored Procedure to Withdraw
CREATE PROCEDURE Withdraw
    @AccountNumber INT,
    @Amount DECIMAL(18,2)
AS
BEGIN
    UPDATE Accounts SET Balance = Balance - @Amount WHERE AccountNumber = @AccountNumber AND Balance >= @Amount;
END

-- Stored Procedure to Display Account Details
CREATE PROCEDURE DisplayAccountDetails
    @AccountNumber INT
AS
BEGIN
    SELECT a.AccountNumber, c.Name, a.Balance
    FROM Accounts a
    JOIN Customers c ON a.CustomerID = c.CustomerID
    WHERE a.AccountNumber = @AccountNumber;
END

-- Example usage:
DECLARE @JohnsAccount INT;
DECLARE @JanesAccount INT;

-- Create accounts
EXEC CreateAccount 1, 500; -- Assuming John's CustomerID is 1
EXEC CreateAccount 2, 1000; -- Assuming Jane's CustomerID is 2

-- Get the account numbers
SELECT @JohnsAccount = SCOPE_IDENTITY();
SELECT @JanesAccount = SCOPE_IDENTITY();

-- Make some transactions
EXEC Deposit @JohnsAccount, 300;
EXEC Withdraw @JohnsAccount, 200;
EXEC Deposit @JanesAccount, 500;
EXEC Withdraw @JanesAccount, 1500;

-- Display account details
EXEC DisplayAccountDetails @JohnsAccount;
EXEC DisplayAccountDetails @JanesAccount;
