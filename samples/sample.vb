' AI generated sample

Public Class Bank
    Private _name As String
    Private _accounts As Dictionary(Of Integer, Account)

    Public Sub New(name As String)
        _name = name
        _accounts = New Dictionary(Of Integer, Account)()
    End Sub

    Public Function CreateAccount(customer As Customer, initialDeposit As Decimal) As Integer
        Dim account As New Account(customer, initialDeposit)
        _accounts.Add(account.AccountNumber, account)
        Return account.AccountNumber
    End Function

    Public Sub Deposit(accountNumber As Integer, amount As Decimal)
        If _accounts.ContainsKey(accountNumber) Then
            _accounts(accountNumber).Deposit(amount)
        Else
            Console.WriteLine("Account not found.")
        End If
    End Sub

    Public Sub Withdraw(accountNumber As Integer, amount As Decimal)
        If _accounts.ContainsKey(accountNumber) Then
            _accounts(accountNumber).Withdraw(amount)
        Else
            Console.WriteLine("Account not found.")
        End If
    End Sub

    Public Sub DisplayAccountDetails(accountNumber As Integer)
        If _accounts.ContainsKey(accountNumber) Then
            _accounts(accountNumber).DisplayDetails()
        Else
            Console.WriteLine("Account not found.")
        End If
    End Sub
End Class

Public Class Account
    Private Shared _accountNumberCounter As Integer = 1000
    Public ReadOnly Property AccountNumber As Integer
    Private _customer As Customer
    Private _balance As Decimal

    Public Sub New(customer As Customer, initialDeposit As Decimal)
        _customer = customer
        _balance = initialDeposit
        AccountNumber = System.Threading.Interlocked.Increment(_accountNumberCounter)
    End Sub

    Public Sub Deposit(amount As Decimal)
        _balance += amount
        Console.WriteLine("Deposited " & amount & ". New balance: " & _balance)
    End Sub

    Public Sub Withdraw(amount As Decimal)
        If amount > _balance Then
            Console.WriteLine("Insufficient funds.")
        Else
            _balance -= amount
            Console.WriteLine("Withdrew " & amount & ". New balance: " & _balance)
        End If
    End Sub

    Public Sub DisplayDetails()
        Console.WriteLine("Account Number: " & AccountNumber)
        Console.WriteLine("Customer Name: " & _customer.Name)
        Console.WriteLine("Account Balance: " & _balance)
    End Sub
End Class

Public Class Customer
    Public ReadOnly Property Name As String
    Private _address As String

    Public Sub New(name As String, address As String)
        Me.Name = name
        _address = address
    End Sub
End Class

' Example usage:
Module Module1
    Sub Main()
        Dim bank As New Bank("Copilot Bank")

        ' Create customers
        Dim john As New Customer("John Doe", "123 Elm Street")
        Dim jane As New Customer("Jane Doe", "456 Oak Street")

        ' Create accounts
        Dim johnsAccount As Integer = bank.CreateAccount(john, 500)
        Dim janesAccount As Integer = bank.CreateAccount(jane, 1000)

        ' Make some transactions
        bank.Deposit(johnsAccount, 300)
        bank.Withdraw(johnsAccount, 200)
        bank.Deposit(janesAccount, 500)
        bank.Withdraw(janesAccount, 1500)

        ' Display account details
        bank.DisplayAccountDetails(johnsAccount)
        bank.DisplayAccountDetails(janesAccount)
    End Sub
End Module
