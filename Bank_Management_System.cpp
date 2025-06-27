#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

const string ACCOUNT_FILE = "account.dat";
const int MIN_SAVINGS_BALANCE = 500;
const int MIN_CURRENT_BALANCE = 1000;

class Account 
{
    int acno;
    string name;
    int deposit;
    char type;              // 'S' ---> Savings, 'C' ---> Current

public:
    void create_account();
    void show_account() const;
    /* Defining certain functions as const is to signify that it's read-only and will not modify the
    state of the object in any way*/
    void modify();
    void deposit_amount(int);
    void withdraw_amount(int);
    void report() const;
    int get_acno() const;
    int get_balance() const;
    char get_type() const;
};

void Account::create_account()    // create_account is a part of the Account class
{
    cout << "\nEnter Account Number: ";
    cin >> acno;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');   // Used to ignore characters in the input stream --> To fix issues when using cin>> and getline()
    cout << "Enter Account Holder Name: ";
    getline(cin, name);
    cout << "Enter Account Type (S for Savings, C for Current): ";
    cin >> type;
    type = toupper(type);
    while (type != 'S' && type != 'C')  // loop to ensure we get an appropriate type of account
    {
        cout << "Invalid type. Enter S or C: ";
        cin >> type;
        type = toupper(type);
    }
    cout << "Enter Initial Deposit Amount: ";
    cin >> deposit;
    int min_balance = (type == 'S') ? MIN_SAVINGS_BALANCE : MIN_CURRENT_BALANCE;
    while (deposit < min_balance) {
        cout << "Minimum balance for this account type is " << min_balance << ". Please re-enter: ";
        cin >> deposit;
    }
    cout << "\nAccount Created Successfully.\n";
}

void Account::show_account() const 
{
    cout << "\nAccount Number: " << acno << "\nAccount Holder: " << name << "\nAccount Type: " << (type == 'S' ? "Savings" : "Current") << "\nBalance: Rs. " << deposit << "\n";
}

void Account::modify() 
{
    cin.ignore();       // Empties in the input buffer
    cout << "Modify Account Holder Name: ";
    getline(cin, name);
    cout << "Modify Account Type (S/C): ";
    cin >> type;
    type = toupper(type);
    cout << "Modify Balance Amount: ";
    cin >> deposit;
}

void Account::deposit_amount(int amt) 
{
    deposit += amt;
}

void Account::withdraw_amount(int amt) 
{
    if (deposit - amt < ((type == 'S') ? MIN_SAVINGS_BALANCE : MIN_CURRENT_BALANCE)) 
    {
        cout << "\nInsufficient balance as per minimum requirement.";
    } 
    else
    deposit -= amt;
}

void Account::report() const 
{
    cout << left << setw(10) << acno << setw(20) << name
         << setw(10) << (type == 'S' ? "Savings" : "Current")
         << "Rs. " << deposit << endl;
}

int Account::get_acno() const { return acno; }
int Account::get_balance() const { return deposit; }
char Account::get_type() const { return type; }

// File Handling Functions
void write_account();
void display_account(int);
void modify_account(int);
void delete_account(int);
void display_all();
void deposit_withdraw(int, bool);

int main() 
{
    char choice;
    int num;
    do {
        cout << "\n================ BANK MANAGEMENT SYSTEM ================\n";
        cout << "1. Create New Account\n";
        cout << "2. Deposit Amount\n";
        cout << "3. Withdraw Amount\n";
        cout << "4. Display Account\n";
        cout << "5. Display All Accounts\n";
        cout << "6. Delete Account\n";
        cout << "7. Modify Account\n";
        cout << "8. Exit\n";
        cout << "Select Your Option (1-8): ";
        cin >> choice;

        switch (choice) {
            case '1': write_account(); break;
            case '2': cout << "Enter Account Number: "; cin >> num; deposit_withdraw(num, true); break;
            case '3': cout << "Enter Account Number: "; cin >> num; deposit_withdraw(num, false); break;
            case '4': cout << "Enter Account Number: "; cin >> num; display_account(num); break;
            case '5': display_all(); break;
            case '6': cout << "Enter Account Number: "; cin >> num; delete_account(num); break;
            case '7': cout << "Enter Account Number: "; cin >> num; modify_account(num); break;
            case '8': cout << "\nThank you for using the system.\n"; break;
            default: cout << "\nInvalid choice. Try again.\n";
        }
    } while (choice != '8');
    return 0;
}

void write_account() 
{
    Account ac;
    ofstream outFile(ACCOUNT_FILE, ios::binary | ios::app);
    ac.create_account();
    outFile.write(reinterpret_cast <char*>(&ac), sizeof(Account));   // Converts the Account object into a raw byte stream using reinterpret_cast<char*>, then writes the binary data to the file --> allows you to store the entire object as-is, instead of writing individual fields
    outFile.close();  // Closes the file to ensure data is saved and resources are released
}

void display_account(int n) {
    Account ac;
    ifstream inFile(ACCOUNT_FILE, ios::binary);
    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.get_acno() == n) {
            ac.show_account();
            found = true;
            break;
        }
    }
    if (!found) cout << "\nAccount Not Found.\n";
    inFile.close();
}

void modify_account(int n) {
    Account ac;
    fstream File(ACCOUNT_FILE, ios::binary | ios::in | ios::out);
    bool found = false;
    while (!File.eof() && !found) {
        streampos pos = File.tellg();
        File.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.get_acno() == n) {
            ac.show_account();
            cout << "\nEnter New Details:\n";
            ac.modify();
            File.seekp(pos);
            File.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "\nAccount Updated.\n";
            found = true;
        }
    }
    if (!found) cout << "\nAccount Not Found.\n";
    File.close();
}

void delete_account(int n) {
    Account ac;
    ifstream inFile(ACCOUNT_FILE, ios::binary);
    ofstream outFile("Temp.dat", ios::binary);
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.get_acno() != n) {
            outFile.write(reinterpret_cast<char*>(&ac), sizeof(Account));
        } else {
            found = true;
        }
    }
    inFile.close();
    outFile.close();
    remove(ACCOUNT_FILE.c_str());
    rename("Temp.dat", ACCOUNT_FILE.c_str());
    cout << (found ? "\nAccount Deleted.\n" : "\nAccount Not Found.\n");
}

void display_all() {
    Account ac;
    ifstream inFile(ACCOUNT_FILE, ios::binary);
    cout << "\nACCOUNT HOLDER LIST\n";
    cout << "--------------------------------------------------\n";
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        ac.report();
    }
    inFile.close();
}

void deposit_withdraw(int n, bool is_deposit) {
    Account ac;
    fstream File(ACCOUNT_FILE, ios::binary | ios::in | ios::out);
    bool found = false;
    int amt;
    while (!File.eof() && !found) {
        streampos pos = File.tellg();
        File.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.get_acno() == n) {
            ac.show_account();
            cout << (is_deposit ? "\nEnter Deposit Amount: " : "\nEnter Withdrawal Amount: ");
            cin >> amt;
            if (is_deposit) ac.deposit_amount(amt);
            else ac.withdraw_amount(amt);
            File.seekp(pos);
            File.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "\nTransaction Successful.\n";
            found = true;
        }
    }
    if (!found) cout << "\nAccount Not Found.\n";
    File.close();
}
