// Rene Rodriguez 09/26/25  
// Lab Activities: Inheritance
// Challenge: Specialized Bank Accounts

#include<iostream>
#include<vector>
#include<string>
#include<limits>
#include<stdexcept>
#include<cmath>
using namespace std;

// Global variables used for user input
double userDouble;
int userInt;
string userString;

// ================= Utility Functions =================

// Helper function for validated integer input
int getIntInput(){
    int input;
    cin >> input;
    while(input < 0 || cin.fail()){  // keep asking until valid positive integer
        if (cin.fail()) {
            cout << "Invalid input, try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }
        if(input < 0){
            cout << "Enter a positive number" << endl; 
        }
        cin >> input;
    }
    cout << "You entered " << input << endl;
    return input;
}

// ================= BankAccount Class =================
class BankAccount {
    public:
        // Default constructor
        BankAccount(){
            accountNumber = "000000";
            accountHolderName = "Unknown";
            balance = 0.0;
        }

        // Parameterized constructor
        BankAccount(string accNum, string holderName, double bal){
            accountNumber = accNum;
            accountHolderName = holderName;
            balance = bal;
        }

        // Static helper functions
        static BankAccount createAccountFromInput(vector<BankAccount*>& bankVect);
        static void printAccount(const BankAccount& account);

        // Rule of Three (copy constructor, destructor, assignment operator)
        BankAccount(const BankAccount& other);   // copy constructor
        virtual ~BankAccount();                          // destructor
        BankAccount& operator=(const BankAccount& other){  // assignment operator
            if(this != &other){ // prevent self-assignment
                accountNumber = other.accountNumber;
                accountHolderName = other.accountHolderName;
                balance = other.balance;
            }
            return *this;
        }

        // Overloaded operators for deposits and withdrawals
        BankAccount& operator+=(double amount);  // deposit
        BankAccount& operator-=(double amount);  // withdraw

        // Comparison operators
        bool operator==(const BankAccount& other) const;
        bool operator<(const BankAccount& other) const;
        bool operator>(const BankAccount& other) const;

        // Getters and setters
        string GetAccNum() const;
        string GetAccHolder() const;
        double GetBalance() const;
        void SetAccHolder(string holderName);
        void Deposit(double depositAmount);
        virtual bool Withdraw(double withdrawAmount);
    
    protected:
        double balance;
        string accountNumber;
        string accountHolderName;
    private:
};

// ================= BankAccount Definitions =================

// Copy constructor (performs deep copy of members)
BankAccount::BankAccount(const BankAccount& other) 
    : accountHolderName(other.accountHolderName),
      accountNumber(other.accountNumber),
      balance(other.balance) {}

// Destructor (empty since no dynamic memory)
BankAccount::~BankAccount(){}

// Operator overload for += (deposit money)
BankAccount& BankAccount::operator+=(double amount){
    if (amount <= 0) {
        throw invalid_argument("Cannot add a non-positive amount.");
    }
    this->balance += amount;
    return *this;
}

// Operator overload for -= (withdraw money)
BankAccount& BankAccount::operator-=(double amount){
    if (amount <= 0) {
        throw invalid_argument("Cannot enter a non-positive amount.");
    }
    if (this->balance < amount) {
        throw invalid_argument("Insufficient funds.");
    }
    this->balance -= amount;
    return *this;
}

// Print account details
void BankAccount::printAccount(const BankAccount& account){
    cout << "Account Number: " << account.GetAccNum() << endl
         << "Account Holder Name: " << account.GetAccHolder() << endl
         << "Account Balance: " << account.GetBalance() << endl;
}

// Create new account by prompting user for input
BankAccount BankAccount::createAccountFromInput(vector<BankAccount*>& bankVect){
    BankAccount currAccount;
    cout << "Enter bank account number." << endl;
    // Make sure account number is unique
    do {
        userInt = getIntInput();
        for (int i = 0; i < bankVect.size(); ++i){
            currAccount = *bankVect.at(i);
            if (currAccount.GetAccNum() == to_string(userInt)){
                cout << "Account number already exists." << endl
                     << "Enter new number." << endl;
                break;
            }
        }
    } while (currAccount.GetAccNum() == to_string(userInt));

    cout << "Enter Name" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer
    getline(cin, userString);

    // Return new BankAccount with user input
    BankAccount newAccount = BankAccount(to_string(userInt), userString, 0.0);
    return newAccount;
}

// Comparison operators
bool BankAccount::operator==(const BankAccount& other) const {
    return this->accountNumber == other.accountNumber;
}
bool BankAccount::operator<(const BankAccount& other) const {
    return this->balance < other.balance;
}
bool BankAccount::operator>(const BankAccount& other) const {
    return this->balance > other.balance;
}

// Getter and setter definitions
string BankAccount::GetAccNum() const { return accountNumber; }
string BankAccount::GetAccHolder() const { return accountHolderName; }
double BankAccount::GetBalance() const { return balance; }
void BankAccount::SetAccHolder(string holderName){ accountHolderName = holderName; }
void BankAccount::Deposit(double depositAmount){ balance += depositAmount; }
bool BankAccount::Withdraw(double withdrawAmount){
    if (withdrawAmount > balance){
        cout << "Insufficient funds." << endl;
        return true;
    } else {
        balance -= withdrawAmount;
        return false;
    }

}

class CheckingAccount : public BankAccount
{
private:
    double transactionFee;

public:
    CheckingAccount() : BankAccount(), transactionFee(1.0) {}
    CheckingAccount(string accNum, string holderName, double bal) : BankAccount(accNum, holderName, bal), transactionFee(1.0) {}
    ~CheckingAccount() override;
    virtual bool Withdraw(double withdrawAmount);
    double GetFee(){
        return transactionFee;
    }
};


CheckingAccount::~CheckingAccount()
{
}
bool CheckingAccount::Withdraw(double withdrawAmount){
    if ((withdrawAmount + (withdrawAmount * (transactionFee / 100))) > balance){
        cout << "Insufficient funds." << endl;
        return true;
    } else {
        balance -= (floor((withdrawAmount + (withdrawAmount / 100)) * 100.0) / 100.0);
        return false;
    }
}

class SavingsAccount : public BankAccount
{
private:
    double interestRate;
public:
    SavingsAccount() : BankAccount(), interestRate(0.004) {}
    SavingsAccount(string accNum, string holderName, double bal) : BankAccount(accNum, holderName, bal), interestRate(0.004) {}
    ~SavingsAccount() override;
    void calculateInterest();
};

SavingsAccount ::~SavingsAccount ()
{
}
void SavingsAccount ::calculateInterest(){
    balance += (balance * interestRate);
}



// ================= Menu and Account Functions =================

// Show menu options
void promptUser(){
    cout << "What would you like to do?" << endl;
    cout << "\t 1. Create checking account" << endl;
    cout << "\t 2. Create savings account" << endl;
    cout << "\t 3. List all accounts" << endl;
    cout << "\t 4. Change account holder name" << endl;
    cout << "\t 5. Deposit" << endl;
    cout << "\t 6. Withdraw" << endl;
    cout << "\t 7. Create a copy of an account" << endl;
    cout << "\t 8. Delete Account" << endl;
    cout << "\t 9. Compare Accounts" << endl;
    cout << "\t 10. Calculate Interest" << endl;
    cout << "\t 0. Quit" << endl;
}

// Validated double input
double getdoubleInput(){
    double input;
    cin >> input;
    while(input < 0 || cin.fail()){
        if (cin.fail()) {
            cout << "Invalid input, try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }
        if(input < 0){
            cout << "Enter a positive number" << endl; 
        }
        cin >> input;
    }
    cout << "You entered " << input << endl;
    return input;
}

// List all accounts
void listAccounts(vector<BankAccount*>& bankVect){
    if (bankVect.empty()){
        cout << "No accounts" << endl;
    } else {
        for (int i = 0; i < bankVect.size(); ++i){
            BankAccount::printAccount(*bankVect.at(i));
        }
    }
}

// Change account holder name
void changeAccountName(vector<BankAccount*>& bankVect){
    bool notFound = true;
    if (bankVect.empty()){
        cout << "No accounts available." << endl;
    } else {
        while(notFound){
            cout << "Enter Account Number you would like to change name of." << endl;
            userInt = getIntInput();
            for (int i = 0; i < bankVect.size(); ++i){
                if(bankVect[i]->GetAccNum() == to_string(userInt)){
                    notFound = false;
                    cout << "Enter new name" << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, userString);
                    bankVect[i]->SetAccHolder(userString);
                    break;
                }
            }
            if(notFound){
                cout << "Account Number not found. Try Again." << endl;
            }
        }
    }
}

// Deposit money into an account
void depositAmount(vector<BankAccount*>& bankVect){
    bool notFound = true;
    if(bankVect.empty()){
        cout << "No accounts available." << endl;
    } else {
        while(notFound){
            cout << "Enter Account Number you would like to deposit money." << endl;
            userInt = getIntInput();
            for (int i = 0; i < bankVect.size(); ++i){
                if(bankVect[i]->GetAccNum() == to_string(userInt)){
                    notFound = false;
                    cout << "Enter deposit amount" << endl;
                    userDouble = getdoubleInput();
                    *bankVect[i] += userDouble;  // uses overloaded +=
                    break;
                }
            }
            if(notFound){
                cout << "Account Number not found. Try Again." << endl;
            }
        }
    }
}

// Withdraw money from an account
void withdrawAmount(vector<BankAccount*>& bankVect){
    bool notFound = true;
    bool tooMuch = true;
    if(bankVect.empty()){
        cout << "No accounts available." << endl;
    } else {
        while(notFound || tooMuch){
            cout << "Enter Account Number you would like to withdraw money from." << endl;
            userInt = getIntInput();
            for (int i = 0; i < bankVect.size(); ++i){
                if(bankVect[i]->GetAccNum() == to_string(userInt)){
                    notFound = false;
                    cout << "Enter withdrawal amount" << endl;
                    userDouble = getdoubleInput();
                    tooMuch = bankVect[i]->Withdraw(userDouble);
                    break;
                }
            }
            if(notFound){
                cout << "Account Number not found. Try Again." << endl;
            }
        }
    }
}

// Create a duplicate of an existing account
void createCopy(vector<BankAccount*>& bankVect) {
    bool notFound = true;
    if (bankVect.empty()){
        cout << "No accounts" << endl;
    } else {
        while (notFound){
            cout << "Enter account number of account you want to copy." << endl;
            cin >> userString;
            for (int i = 0; i < bankVect.size(); i++){
                if (bankVect[i]->GetAccNum() == userString){
                    notFound = false;
                    BankAccount* newAccount = bankVect[i]; // copy constructor
                    bankVect.push_back(newAccount);
                    cout << "Account copied successfully." << endl;
                    break;
                }
            }
            if (notFound) {
                cout << "Account not found! Try again." << endl;
            }
        }
    }
}

// Delete an existing account
void deleteAccount(vector<BankAccount*>& bankVect){
    bool notFound = true;
    if (bankVect.empty()){
        cout << "No accounts" << endl;
    } else {
        while (notFound){
            cout << "Enter account number of account you want to delete." << endl;
            cin >> userString;
            for (int i = 0; i < bankVect.size(); i++){
                if (bankVect[i]->GetAccNum() == userString){
                    notFound = false;
                    bankVect.erase(bankVect.begin() + i);
                    cout << "Account deleted successfully." << endl;
                    break;
                }
            }
            if (notFound){
                cout << "Account not found! Try again." << endl;
            }
        }
    }
}
// Compare two accounts using overloaded comparison operators
void compareAccounts(vector<BankAccount*>& bankVect) {
    bool notFound = true;
    BankAccount* account1;  // first account to compare
    BankAccount* account2;  // second account to compare

    // Check if there are at least 2 accounts in the system
    if (bankVect.empty() || bankVect.size() < 2){
        if (bankVect.empty()){
            cout << "No accounts" << endl;
        } else {
            cout << "Only 1 account to compare. Try creating another account." << endl;
        }
    } else {
        // ================= Select First Account =================
        while (notFound){
            cout << "Enter account number of first account you want to compare." << endl;
            cin >> userString;
            for (int i = 0; i < bankVect.size(); i++){
                // Search for account number entered by user
                if (bankVect[i]->GetAccNum() == userString){
                    notFound = false;
                    account1 = bankVect[i];   // copy account into account1
                    break;
                }
            }
            if (notFound){
                cout << "Account not found! Try again." << endl;
            }
        }

        // Reset flag for second search
        notFound = true;

        // ================= Select Second Account =================
        while (notFound){
            cout << "Enter account number of second account you want to compare." << endl;
            cin >> userString;
            for (int i = 0; i < bankVect.size(); i++){
                if (bankVect[i]->GetAccNum() == userString){
                    notFound = false;
                    account2 = bankVect[i];   // copy account into account2
                    break;
                }
            }
            if (notFound){
                cout << "Account not found! Try again." << endl;
            }
        }

        // ================= Compare Accounts =================
        // Uses my overloaded operators:
        //   operator> compares balances
        if (*account1 > *account2) {
            cout << "Account Number: " << account1->GetAccNum() 
                 << " has a greater balance than Account Number: " 
                 << account2->GetAccNum() << endl;
        } else if (*account2 > *account1) {
            cout << "Account Number: " << account2->GetAccNum() 
                 << " has a greater balance than Account Number: " 
                 << account1->GetAccNum() << endl;
        } else {
            cout << "Account Number: " << account1->GetAccNum() 
                 << " and Account Number: " << account2->GetAccNum() 
                 << " have equal balances." << endl;
        }
    }
}

// ================= Main Program =================
int main()
{
    vector<BankAccount*> accountList;

    // Make some CheckingAccounts and SavingsAccounts
    CheckingAccount c1("1", "Alice Checking", 500.0);
    CheckingAccount c2("2", "Bob Checking", 1000.0);

    SavingsAccount s1("3", "Charlie Savings", 2000.0);
    SavingsAccount s2("4", "Diana Savings", 3000.0);

    accountList.push_back(&c1);
    accountList.push_back(&c2);
    accountList.push_back(&s1);
    accountList.push_back(&s2);

    vector<BankAccount> accounts;
    BankAccount newAccount;
    CheckingAccount* newCheckingAccount;
    SavingsAccount* newSavingsAccount;

    // Main menu loop
    while (true) {
        promptUser();
        userInt = getIntInput();
        switch (userInt) {
            case 1: // Create checking account
                newAccount = BankAccount::createAccountFromInput(accountList);
                newCheckingAccount = new CheckingAccount(newAccount.GetAccNum(), newAccount.GetAccHolder(), 0.0);
                accountList.push_back(newCheckingAccount);
                break;
            case 2: // Create savings account
                newAccount = BankAccount::createAccountFromInput(accountList);
                newSavingsAccount = new SavingsAccount(newAccount.GetAccNum(), newAccount.GetAccHolder(), 0.0);
                accountList.push_back(newSavingsAccount);
                break;
            case 3: // List accounts
                listAccounts(accountList);
                break;
            case 4: // Change account name
                changeAccountName(accountList);
                break;
            case 5: // Deposit
                depositAmount(accountList);
                break;
            case 6: // Withdraw
                withdrawAmount(accountList);
                break;
            case 7: // Create a copy
                createCopy(accountList);
                break;
            case 8: // Delete accounts
                deleteAccount(accountList);
                break;
            case 9: // Compare accounts
                compareAccounts(accountList);
                break;
            case 10: // Calculate interest
                for (auto acc : accountList) {
                    SavingsAccount* savings = dynamic_cast<SavingsAccount*>(acc);
                    if (savings != nullptr) {
                        savings->calculateInterest();
                        cout << "Interest applied to account " << savings->GetAccNum() << endl;
                    }
                }
                break;
            case 0: // Exit
                cout << "Goodbye!" << endl;
                return 0;
        }
    }
    return 0;
}