# BankAccount Class

A class representing accounts in a banking system

## Data Dictionary

| Attribute     | Data Type     | Description                    |
|---------------|---------------|--------------------------------|
| `accountNumber`       | `std::string` | Account's Number              |
| `accountHolderName`      | `std::string` | Owner of the account             |
| `balance`        | `double` | Amount of money in account        |

## Methods List

| Method Signature             | Return Type   | Description               |
|------------------------------|---------------|---------------------------|
| `BankAccount()`                     | (Constructor) | Default constructor.      |
| `BankAccount(accNum, holderName, bal)`  | (Constructor) | Parameterized constructor.|
| `GetAccNum() const`           | `std::string` | Gets the account's number.    |
| `GetAccHolder() const`          | `std::string` | Gets the account's owner.   |
| `GetBalance() const`        | `double`        | Returns the balance total.      |
| `SetAccHolder(string holderName)`                 | `void`        | Changes the name of account owner.      |
| `Deposit(double depositAmount)`               | `void`        | Adds money to your balance.         |
| `Withdraw(double withdrawAmount)`        | `void`     | Takes money from your balance.