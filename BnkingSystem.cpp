#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/* ========= BASE ACCOUNT CLASS ========= */

class Account
{
public:
    vector<double> transactionLog;
    int accountCode;
    string ownerName;
    double ledgerBalance;
    unsigned int accessMask;

public:
    Account() {}

    Account(int id, string name, double bal, unsigned int mask)
    {
        accountCode = id;
        ownerName = name;
        ledgerBalance = bal;
        accessMask = mask;
    }

    virtual void deposit(double amt) = 0;
    virtual void withdraw(double amt) = 0;
    virtual string getType() = 0;

    int getID()
    {
        return accountCode;
    }

    void showAccount()
    {
        cout << "\n--- ACCOUNT INFO ---\n";
        cout << "ID: " << accountCode << endl;
        cout << "Name: " << ownerName << endl;
        cout << "Balance: " << ledgerBalance << endl;

        cout << "Transaction History:\n";

        int i = 0;
        while (i < transactionLog.size())
        {
            cout << transactionLog[i] << endl;
            i++;
        }
    }

    virtual void save(ofstream &out)
    {
        out << getType() << endl;
        out << accountCode << " " << ownerName << " " << ledgerBalance << " " << accessMask << endl;

        out << transactionLog.size() << endl;

        for (int i = 0; i < transactionLog.size(); i++)
            out << transactionLog[i] << endl;
    }

    virtual void load(ifstream &in)
    {
        int count;

        in >> accountCode >> ownerName >> ledgerBalance >> accessMask;
        in >> count;

        transactionLog.clear();

        for (int i = 0; i < count; i++)
        {
            double t;
            in >> t;
            transactionLog.push_back(t);
        }
    }

    virtual ~Account() {}
};

/* ========= SAVINGS ACCOUNT ========= */

class SavingsAccount : public Account
{
public:
    SavingsAccount() {}

    SavingsAccount(int id, string name, double bal, unsigned int mask)
        : Account(id, name, bal, mask) {}

    string getType()
    {
        return "SAVINGS";
    }

    void deposit(double amt)
    {
        if (!(accessMask & 2))
        {
            cout << "Deposit permission denied\n";
            return;
        }

        ledgerBalance += amt;
        transactionLog.push_back(amt);

        cout << "Deposit Successful\n";
    }

    void withdraw(double amt)
    {
        if (!(accessMask & 1))
        {
            cout << "Withdraw permission denied\n";
            return;
        }

        if (amt > ledgerBalance)
        {
            cout << "Insufficient Balance\n";
            return;
        }

        ledgerBalance -= amt;
        transactionLog.push_back(-amt);

        cout << "Withdraw Successful\n";
    }
};

/* ========= CURRENT ACCOUNT ========= */

class CurrentAccount : public Account
{
public:
    CurrentAccount() {}

    CurrentAccount(int id, string name, double bal, unsigned int mask)
        : Account(id, name, bal, mask) {}

    string getType()
    {
        return "CURRENT";
    }

    void deposit(double amt)
    {
        if (!(accessMask & 2))
        {
            cout << "Deposit blocked\n";
            return;
        }

        ledgerBalance += amt;
        transactionLog.push_back(amt);

        cout << "Deposit Completed\n";
    }

    void withdraw(double amt)
    {
        if (!(accessMask & 1))
        {
            cout << "Withdraw blocked\n";
            return;
        }

        ledgerBalance -= amt;
        transactionLog.push_back(-amt);

        cout << "Withdraw Completed\n";
    }
};

/* ========= GLOBAL STORAGE ========= */

vector<Account *> bankAccounts;

/* ========= HELPER FUNCTION ========= */

int readAccountID()
{
    int id;
    cout << "Enter Account ID: ";
    cin >> id;
    return id;
}

/* ========= FIND ACCOUNT ========= */

Account *findAccount(int id)
{
    int i = 0;

    while (i < bankAccounts.size())
    {
        if (bankAccounts[i]->getID() == id)
            return bankAccounts[i];

        i++;
    }

    return NULL;
}

/* ========= TRANSFER FEATURE ========= */

void transferMoney(int from, int to, double amount)
{
    Account *A = findAccount(from);
    Account *B = findAccount(to);

    if (A == NULL || B == NULL)
    {
        cout << "Account not found\n";
        return;
    }

    A->withdraw(amount);
    B->deposit(amount);

    cout << "Transfer Completed\n";
}

/* ========= SAVE DATA ========= */

void saveSystem()
{
    ofstream file("ledger_data.txt");

    for (int i = 0; i < bankAccounts.size(); i++)
    {
        bankAccounts[i]->save(file);
        file << "###" << endl;
    }

    file.close();

    cout << "Data Saved Successfully\n";
}

/* ========= LOAD DATA ========= */

void loadSystem()
{
    ifstream file("ledger_data.txt");

    if (!file)
    {
        cout << "File not found\n";
        return;
    }

    string type;

    while (file >> type)
    {
        Account *obj;

        if (type == "SAVINGS")
            obj = new SavingsAccount();
        else
            obj = new CurrentAccount();

        obj->load(file);
        bankAccounts.push_back(obj);

        string skip;
        file >> skip;
    }

    file.close();

    cout << "Data Loaded\n";
}

/* ========= MONTHLY SUMMARY ========= */

void monthlySummary()
{
    double monthlyTotals[50] = {0};

    for (int i = 0; i < bankAccounts.size(); i++)
    {
        int index = 0;

        for (int j = 0; j < bankAccounts[i]->transactionLog.size(); j++)
        {
            double t = bankAccounts[i]->transactionLog[j];

            monthlyTotals[index] += t;

            index++;

            if (index == 50)
                index = 0;
        }
    }

    cout << "\nMonthly Change Report\n";

    for (int i = 0; i < 50; i++)
        cout << "Slot " << i << " : " << monthlyTotals[i] << endl;
}

/* ========= MAIN PROGRAM ========= */

int main()
{
    int choice;

    while (true)
    {
        cout << "\n===== Secure Ledger System =====\n";

        cout << "1 Create Account\n";
        cout << "2 Deposit\n";
        cout << "3 Withdraw\n";
        cout << "4 Show Account\n";
        cout << "5 Transfer Money\n";
        cout << "6 Save Data\n";
        cout << "7 Load Data\n";
        cout << "8 Monthly Summary\n";
        cout << "9 Exit\n";

        cin >> choice;

        if (choice == 1)
        {
            int id, type;
            string name;
            double bal;
            unsigned int perm;

            cout << "Account ID: ";
            cin >> id;
            cout << "Owner Name: ";
            cin >> name;
            cout << "Initial Balance: ";
            cin >> bal;

            cout << "Type (1 Savings 2 Current): ";
            cin >> type;

            cout << "Permissions (1 withdraw,2 deposit,4 transfer): ";
            cin >> perm;

            Account *acc;

            if (type == 1)
                acc = new SavingsAccount(id, name, bal, perm);
            else
                acc = new CurrentAccount(id, name, bal, perm);

            bankAccounts.push_back(acc);

            cout << "Account Created\n";
        }

        else if (choice == 2)
        {
            int id = readAccountID();
            double amt;

            cout << "Enter Amount: ";
            cin >> amt;

            Account *acc = findAccount(id);

            if (acc)
                acc->deposit(amt);
            else
                cout << "Account not found\n";
        }

        else if (choice == 3)
        {
            int id = readAccountID();
            double amt;

            cout << "Enter Amount: ";
            cin >> amt;

            Account *acc = findAccount(id);

            if (acc)
                acc->withdraw(amt);
            else
                cout << "Account not found\n";
        }

        else if (choice == 4)
        {
            int id = readAccountID();

            Account *acc = findAccount(id);

            if (acc)
                acc->showAccount();
            else
                cout << "Account not found\n";
        }

        else if (choice == 5)
        {
            int from, to;
            double amount;

            cout << "From Account ID: ";
            cin >> from;

            cout << "To Account ID: ";
            cin >> to;

            cout << "Amount: ";
            cin >> amount;

            transferMoney(from, to, amount);
        }

        else if (choice == 6)
            saveSystem();

        else if (choice == 7)
            loadSystem();

        else if (choice == 8)
            monthlySummary();

        else if (choice == 9)
            break;
    }

    for (int i = 0; i < bankAccounts.size(); i++)
        delete bankAccounts[i];
}