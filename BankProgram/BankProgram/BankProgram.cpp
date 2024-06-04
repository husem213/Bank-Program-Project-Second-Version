#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>

using namespace std;
static string ClientsData = "ClientsData.txt";
static string UsersFile = "UsersData.txt";
void MainMenuScreen();
void Login();
void ManageUsersMenu();
void TransactionsMenuScreen();

enum enOptions
{
    ClientsList = 1, AddClient = 2, DeleteClient = 3, UpdateClient = 4, FindClient = 5,
    Transactions = 6, ManageUsers = 7, Logout = 8
};

enum enManagingUsers
{
    UsersList = 1, AddANewUser = 2, DeleteUser = 3, UpdateUser = 4, FindUser = 5, Back2MainMenu = 6
};

enum enTransactions
{
    Deposit = 1, Withdraw = 2, TotalBalances = 3, MainMenu = 4
};

enum enPermissions
{
    pAll = -1, pShowClientsList = 1, pAddNewClient = 2, pDeleteClient = 4, pUpdateClient = 8, pFindClient = 16,
    pTransactions = 32, pManangeUsers = 64
};

struct stClientData
{
    string AccountNumber, PinCode, Name, Phone;
    float AccountBalance = 0;
    bool MarkedForDeletion = false;
};

struct stUser 
{
    string UserName, Password;
    short Permissions = 0;
    bool MarkedForDeletion = false;
};

stUser CurrentUser;

string ReadString(string Message)
{
    string Data;

    cout << Message;
    getline(cin >> ws, Data);

    return Data;
}

char ReadCharacter(string Message)
{
    char Letter;

    cout << Message;
    cin >> Letter;

    return Letter;
}

short ReadOptionNumber()
{
    short Operation;
    
    do
    {
        cout << "\nChoose what do you want to do [1 to 8]: ";
        cin >> Operation;
    } while (Operation < 1 || Operation > 8);

    return Operation;
}

void BackToMainMenu()
{
    cout << "\nPress any key to go back to main menu...";
    system("pause>0");
    MainMenuScreen();
}

vector<string> SplitString(string S, string Delimeter)
{
    vector <string> vWords;
    short DelimPosition;
    string Word;

    while ((DelimPosition = S.find(Delimeter)) != string::npos)
    {
        Word = S.substr(0, DelimPosition);
        if (Word != "")
            vWords.push_back(Word);

        S.erase(0, DelimPosition + Delimeter.length());
    }

    if (S != "")
        vWords.push_back(S);

    return vWords;
}

stClientData ConvertLineToRecord(string LineOfData, string Dilemeter = "#//#")
{
    stClientData Client;
    vector <string> vClientData = SplitString(LineOfData, Dilemeter);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

vector <stClientData> LoadAllClientsinVector()
{
    fstream MyFile;
    vector <stClientData> vClientData;
    string ClientDataLine;
    MyFile.open(ClientsData, ios::in);

    if (MyFile.is_open())
    {
        while (getline(MyFile, ClientDataLine))
            vClientData.push_back(ConvertLineToRecord(ClientDataLine));

        MyFile.close();
    }
    return vClientData;
}

void ShowTableHeader(short ClientsNumber)
{
    cout << "\n\t\t\t\Clients list (" << ClientsNumber << ") client(s)\n";
    cout << "________________________________________________________________________________________\n\n";
}

void TableFirstRow()
{
    cout << "| " << left << setw(20) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(25) << "Client Name";
    cout << "| " << left << setw(14) << "Phone Number ";
    cout << "| " << left << setw(9) << "Balance\n";
    cout << "_______________________________________________________________________________________\n\n";
}

void PrintOneClientData(stClientData Client)
{
    cout << "| " << left << setw(20) << Client.AccountNumber;
    cout << "| " << left << setw(10) << Client.PinCode;
    cout << "| " << left << setw(25) << Client.Name;
    cout << "| " << left << setw(14) << Client.Phone;
    cout << "| " << left << setw(9) << Client.AccountBalance;
}

void PrintAllClientsData(vector <stClientData>& vClients)
{
    for (stClientData& Data : vClients)
    {
        PrintOneClientData(Data);
        cout << endl;
    }
    cout << "________________________________________________________________________________________\n";
}

bool CheckAccessPermission(enPermissions Permission)
{
    return (CurrentUser.Permissions == -1) ? true : (CurrentUser.Permissions & Permission) == Permission ? true : false;
}

void AccessDeniedScreen()
{
    system("cls");
    cout << "\n------------------------------------------\n";
    cout << "Access Denied,\n";
    cout << "You Don't Have Permission To Do This,\n";
    cout << "Please Contact Your Admin.";
    cout << "\n------------------------------------------\n";
}

void ShowClientsList()
{
    if (!CheckAccessPermission(enPermissions::pShowClientsList))
    {
        AccessDeniedScreen();
        return;
    }
    system("cls");
    vector <stClientData> vClients = LoadAllClientsinVector();

    ShowTableHeader(vClients.size());
    TableFirstRow();
    PrintAllClientsData(vClients);
}

void AddNewClientScreen()
{
    cout << "\n------------------------------------------\n";
    cout << "         Add New Client Screen\n";
    cout << "------------------------------------------\n";
    cout << "Adding a new client:\n\n";
}

string CheckAccNumberExistence(vector <stClientData>& vClients)
{
    string AccNumber = ReadString("Enter account number: ");
    bool Exists = true;

    while(Exists)
    {
        Exists = false;
        for (stClientData& C : vClients)
        {
            if (C.AccountNumber == AccNumber)
            {
                cout << "\n\nClient with [" << AccNumber << "] already exists, ";
                AccNumber = ReadString("Enter another account number: ");
                Exists = true;
                break;
            }
        }
    }
    return AccNumber;
}

stClientData ReadNewClient(vector <stClientData>& vClients)
{
    stClientData Client;

    cout << "\nPlease enter new client data:\n\n";

    Client.AccountNumber = CheckAccNumberExistence(vClients);

    cout << "Enter Pin Code: ";
    getline(cin, Client.PinCode);

    cout << "Enter Name: ";
    getline(cin, Client.Name);

    cout << "Enter Phone Number: ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance: ";
    cin >> Client.AccountBalance;

    return Client;
}

void AddDataLineToFile(string DataLine, string File)
{
    fstream MyFile;
    MyFile.open(File, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << DataLine << endl;
        MyFile.close();
    }
}

string ConvertRecordToLine(stClientData Client, string Dilemeter = "#//#")
{
    string ClientData = "";

    ClientData += Client.AccountNumber + Dilemeter;
    ClientData += Client.PinCode + Dilemeter;
    ClientData += Client.Name + Dilemeter;
    ClientData += Client.Phone + Dilemeter;
    ClientData += to_string(Client.AccountBalance);

    return ClientData;
}

void AddNewClient(vector <stClientData>& vClients)
{
    stClientData Client = ReadNewClient(vClients);
    AddDataLineToFile(ConvertRecordToLine(Client), ClientsData);
}

void AddClients()
{
    if (!CheckAccessPermission(enPermissions::pAddNewClient))
    {
        AccessDeniedScreen();
        return;
    }

    char AddMore = 'Y';
    vector <stClientData> vClients = LoadAllClientsinVector();

    do
    {
        system("cls");
        AddNewClientScreen();
        AddNewClient(vClients);

        cout << "Client added successfully! Do you want to add more clients? [y/n]: ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

void DeleteClientScreen()
{
    system("cls");
    cout << "\n------------------------------------------------\n";
    cout << "              Delete Client Screen\n";
    cout << "------------------------------------------------\n";
}

bool FindClientDataUsingAccountNumber(string AccountNumber, vector <stClientData>& vClients, stClientData& ClientData)
{
    for (stClientData& Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            ClientData = Client;
            return true;
        }
    }
    return false;
}

void MarkClientForDeletion(vector <stClientData>& vClients, string AccNumber)
{
    for (stClientData& Client : vClients)
    {
        if (Client.AccountNumber == AccNumber)
        {
            Client.MarkedForDeletion = true;
            break;
        }
    }
}

void PrintClientCard(stClientData Client)
{
    cout << "\nThe following are the client's details:";
    cout << "\n---------------------------------------";
    cout << "\nAccount Number : " << Client.AccountNumber;
    cout << "\nPin Code       : " << Client.PinCode;
    cout << "\nName           : " << Client.Name;
    cout << "\nPhone Number   : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n---------------------------------------";
}

void SaveClientsVectorToFile(vector <stClientData>& vClients)
{
    fstream MyFile;
    MyFile.open(ClientsData, ios::out);

    if (MyFile.is_open())
    {
        for (stClientData& C : vClients)
        {
            if (!C.MarkedForDeletion)
                MyFile << ConvertRecordToLine(C) << endl;
        }
        MyFile.close();
    }
}

void DeleteClientByAccNumber()
{
    if (!CheckAccessPermission(enPermissions::pDeleteClient))
    {
        AccessDeniedScreen();
        return;
    }

    stClientData Client;
    char AskForDeletion = 'n';
    string AccNumber;
    vector <stClientData> vClients = LoadAllClientsinVector();

    DeleteClientScreen();
    AccNumber = ReadString("\nPlease enter Account Number: ");

    if (FindClientDataUsingAccountNumber(AccNumber, vClients, Client))
    {
        PrintClientCard(Client);

        AskForDeletion = ReadCharacter("\n\nAre you sure you want to delete this client? [y/n]: ");
        if (toupper(AskForDeletion) == 'Y')
        {
            MarkClientForDeletion(vClients, AccNumber);
            SaveClientsVectorToFile(vClients);

            vClients = LoadAllClientsinVector();

            cout << "\n\nClient deleted successfully!";
        }
    }
    else
    {
        cout << "\nClient with account number (" << AccNumber << ") is not found";
    }
}

void UpdateClientInfoScreen()
{
    system("cls");
    cout << "\n---------------------------------------------\n";
    cout << "           Update Client Info Screen\n";
    cout << "---------------------------------------------\n";
}

stClientData UpdateClientData(string AccNumber)
{
    stClientData Client;

    Client.AccountNumber = AccNumber;

    cout << "\nEnter Pin Code     : ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name         : ";
    getline(cin, Client.Name);

    cout << "Enter Phone Number : ";
    getline(cin, Client.Phone);

    cout << "Enter Balance      : ";
    cin >> Client.AccountBalance;

    return Client;
}

void UpdateClientDataUsingAccNumber()
{
    if (!CheckAccessPermission(enPermissions::pUpdateClient))
    {
        AccessDeniedScreen();
        return;
    }

    char AskForUpdate = 'n';
    stClientData Client;
    string AccNumber;
    vector <stClientData> vClients = LoadAllClientsinVector();

    UpdateClientInfoScreen();
    AccNumber = ReadString("\nPlease enter Account Number: ");

    if (FindClientDataUsingAccountNumber(AccNumber, vClients, Client))
    {
        PrintClientCard(Client);

        AskForUpdate = ReadCharacter("\n\nAre you sure you want to update this client's data? [y/n]: ");

        if (toupper(AskForUpdate) == 'Y')
        {
            for (stClientData& C : vClients)
            {
                if (C.AccountNumber == AccNumber)
                {
                    C = UpdateClientData(AccNumber);
                    break;
                }
            }
            SaveClientsVectorToFile(vClients);

            cout << "\n\nClient's data is updated successfully!";
        }
    }
    else
    {
        cout << "\nClient with account number (" << AccNumber << ") is not found";
    }
}

void FindClientScreen()
{
    system("cls");
    cout << "\n---------------------------------------------\n";
    cout << "           Find Client Screen\n";
    cout << "---------------------------------------------\n";
}

void FindClientByAccNumber()
{
    if (!CheckAccessPermission(enPermissions::pFindClient))
    {
        AccessDeniedScreen();
        return;
    }

    string AccNumber;
    stClientData Client;
    vector <stClientData> vClients = LoadAllClientsinVector();

    FindClientScreen();
    AccNumber = ReadString("\nPlease enter Account Number: ");

    if (FindClientDataUsingAccountNumber(AccNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with account number [" << AccNumber << "] is not found!";
}

short ReadTransactionNumber()
{
    short Transaction;

    cout << "Choose what do you want to do [1 to 4]: ";
    cin >> Transaction;

    return Transaction;
}

void DepositScreen()
{
    system("cls");
    cout << "\n------------------------------------------------\n";
    cout << "\t\tDeposit Screen\n";
    cout << "------------------------------------------------\n";
}

void DepositAmountUsingAccNumber(vector <stClientData>& vClients, string AccNumber, double Amount)
{
    char Perform;

    Perform = ReadCharacter("\nAre you sure you want to perform this transaction? [y/n]: ");

    if (toupper(Perform) == 'Y')
    {
        for (stClientData& C : vClients)
        {
            if (C.AccountNumber == AccNumber)
            {
                C.AccountBalance += Amount;
                cout << "\nDone Successfully! New balance is " << C.AccountBalance;
                break;
            }
        }
    }
    SaveClientsVectorToFile(vClients);
}

void DepositPerform()
{
    string AccNumber;
    vector <stClientData> vClients = LoadAllClientsinVector();
    stClientData Client;
    double DepositAmount = 0;

    DepositScreen();
    AccNumber = ReadString("\nPlease enter account number: ");

    while (!FindClientDataUsingAccountNumber(AccNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccNumber << "] is not found, Please enter another account number: ";
        cin >> AccNumber;
    }

    PrintClientCard(Client);

    cout << "\nPlease enter deposit amount: ";
    cin >> DepositAmount;

    DepositAmountUsingAccNumber(vClients, AccNumber, DepositAmount);
}

void BackToTransactionsMenu()
{
    cout << "\nPress any key to back to tansactions menu...";
    system("pause>0");
    TransactionsMenuScreen();
}

void WithdrawScreen()
{
    system("cls");
    cout << "\n-----------------------------------------------\n";
    cout << "\t\tWithdraw Screen\n";
    cout << "-----------------------------------------------\n";
}

void WithdrawPerform()
{
    string AccNumber;
    vector <stClientData> vClients = LoadAllClientsinVector();
    stClientData Client;
    float WithdrawAmount = 0;

    WithdrawScreen();
    AccNumber = ReadString("\nPlease enter account number: ");

    while (!FindClientDataUsingAccountNumber(AccNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccNumber << "] is not found, Please enter another account number: ";
        cin >> AccNumber;
    }

    PrintClientCard(Client);

    cout << "\nPlease enter withdraw amount: ";
    cin >> WithdrawAmount;

    while (WithdrawAmount > Client.AccountBalance)
    {
        cout << "\nAmount exceeds the balance, you can withdraw up to: " << Client.AccountBalance;
        cout << "\nPlease enter another amount: ";
        cin >> WithdrawAmount;
    }
    DepositAmountUsingAccNumber(vClients, AccNumber, WithdrawAmount * (-1));
}

void TotalBalancesScreen(short ClientsNumber)
{
    system("cls");
    cout << "\n\t\tBalances List (" << ClientsNumber << ") Client(s)";
    cout << "\n____________________________________________________________________\n";

    cout << "\n| " << left << setw(15) << "Account Number";
    cout << " | " << left << setw(25) << "Client Name";
    cout << " | " << left << setw(10) << "Balance";
    cout << "\n____________________________________________________________________";
}

void PrintOneClientBalance(stClientData Client)
{
    cout << "\n| " << left << setw(15) << Client.AccountNumber;
    cout << " | " << left << setw(25) << Client.Name;
    cout << " | " << left << setw(10) << Client.AccountBalance;
}

void PrintTotalBalances()
{
    vector <stClientData> vClients = LoadAllClientsinVector();
    double Total = 0;

    TotalBalancesScreen(vClients.size());
    for (stClientData& C : vClients)
    {
        PrintOneClientBalance(C);
        Total += C.AccountBalance;
    }
    cout << "\n____________________________________________________________________\n";
    cout << "\n\t\tTotal Balances = " << Total << endl;
}

void PerformTransaction(enTransactions Transaction)
{
    switch (Transaction)
    {
    case Deposit:
        DepositPerform();
        BackToTransactionsMenu();
        break;
    case Withdraw:
        WithdrawPerform();
        BackToTransactionsMenu();
        break;
    case TotalBalances:
        PrintTotalBalances();
        BackToTransactionsMenu();
        break;
    case MainMenu:
        MainMenuScreen();
        break;
    }
}

void TransactionsMenuScreen()
{
    if (!CheckAccessPermission(enPermissions::pTransactions))
    {
        AccessDeniedScreen();
        return;
    }

    system("cls");
    cout << "========================================================\n";
    cout << "\t\tTransactions Menu Screen\n";
    cout << "========================================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menu.\n";
    cout << "========================================================\n";
    PerformTransaction((enTransactions)ReadTransactionNumber());
}

short ReadManagingUsersOption()
{
    short Option;

    do
    {
        cout << "\nChoose what you want to do [1 to 6]: ";
        cin >> Option;
    } while (Option < 1 || Option > 6);

    return Option;
}

stUser ConvertLineToUserRecord(string LineOfData, string Dilemeter = "#//#")
{
    stUser User;
    vector <string> vUserData = SplitString(LineOfData, Dilemeter);

    User.UserName = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);

    return User;
}

vector <stUser> LoadUsersinVector()
{
    fstream MyFile;
    vector <stUser> vUserData;
    string UserDataLine;
    MyFile.open(UsersFile, ios::in);

    if (MyFile.is_open())
    {
        while (getline(MyFile, UserDataLine))
            vUserData.push_back(ConvertLineToUserRecord(UserDataLine));

        MyFile.close();
    }
    return vUserData;
}

void ShowUsersListHeader(short UsersNumber)
{
    cout << "\n\t\t\t\tUsers list (" << UsersNumber << ") user(s)\n";
    cout << "________________________________________________________________________________________\n\n";
}

void UsersListFirstRow()
{
    cout << "| " << left << setw(20) << "UserName";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(25) << "Permissions";
    cout << "\n_______________________________________________________________________________________\n\n";
}

void PrintOneUserData(stUser User)
{
    cout << "| " << left << setw(20) << User.UserName;
    cout << "| " << left << setw(10) << User.Password;
    cout << "| " << left << setw(25) << User.Permissions;
}

void PrintAllUsersData(vector <stUser>& vUsers)
{
    for (stUser& Data : vUsers)
    {
        PrintOneUserData(Data);
        cout << endl;
    }
    cout << "________________________________________________________________________________________\n";
}

void ShowUserListScreen()
{
    system("cls");
    vector <stUser> vUsers = LoadUsersinVector();

    ShowUsersListHeader(vUsers.size());
    UsersListFirstRow();
    PrintAllUsersData(vUsers);

    cout << "Press any key to back to Manage Users Menu...";
    system("pause>0");
    ManageUsersMenu();
}

stUser ReadUser()
{
    stUser User;

    cout << "\nEnter username: ";
    getline(cin >> ws, User.UserName);

    cout << "\nEnter Password: ";
    getline(cin, User.Password);

    return User;
}

string ConvertUserRecordToLine(stUser User, string Dilemeter = "#//#")
{
    string UserData = "";

    UserData += User.UserName + Dilemeter;
    UserData += User.Password + Dilemeter;
    UserData += to_string(User.Permissions);

    return UserData;
}

bool FindUserByUsernameAndPassword(stUser User1, stUser& User2)
{
    vector <stUser> vUsers = LoadUsersinVector();

    for (stUser& U : vUsers)
    {
        if (U.UserName == User1.UserName && U.Password == User1.Password)
        {
            User2 = U;
            return true;
        }
    }
    return false;
}

void AddNewUserScreen()
{
    system("cls");
    cout << "================================================\n";
    cout << "               Add New User Screen\n";
    cout << "================================================\n";
    cout << "Adding new user:\n";
}

string CheckUsernameExistence(vector <stUser>& vUsers)
{
    string Username = ReadString("Enter Username: ");
    bool Exists = true;

    while (Exists)
    {
        Exists = false;
        for (stUser& U : vUsers)
        {
            if (U.UserName == Username)
            {
                cout << "\n\nUser with [" << Username << "] already exists, ";
                Username = ReadString("Enter another account number: ");
                Exists = true;
                break;
            }
        }
    }
    return Username;
}

short ReadPermissions()
{
    char FullAccess = ReadCharacter("\nDo you want to give full access? [y/n]: ");

    if (toupper(FullAccess) == 'Y')
        return -1;

    short Options = 0;
    char Permission;
    cout << "\nDo you want to give access to:\n";

    cout << "\nShow clients list? [y/n]: ";
    cin >> Permission;
    Options = (toupper(Permission) == 'Y') ? 1 : 0;

    cout << "\nAdd new client? [y/n]: ";
    cin >> Permission;
    (toupper(Permission) == 'Y') ? Options += 2 : Options += 0;

    cout << "\nDelete client? [y/n]: ";
    cin >> Permission;
    (toupper(Permission) == 'Y') ? Options += 4 : Options += 0;

    cout << "\nUpdate client? [y/n]: ";
    cin >> Permission;
    (toupper(Permission) == 'Y') ? Options += 8 : Options += 0;

    cout << "\nFind client? [y/n]: ";
    cin >> Permission;
    (toupper(Permission) == 'Y') ? Options += 16 : Options += 0;

    cout << "\nTransactions? [y/n]: ";
    cin >> Permission;
    (toupper(Permission) == 'Y') ? Options += 32 : Options += 0;

    cout << "\nManage users? [y/n]: ";
    cin >> Permission;
    (toupper(Permission) == 'Y') ? Options += 64 : Options += 0;

    return (Options == 127) ? -1 : Options;
}

stUser ReadNewUser(vector <stUser>& vUsers)
{
    stUser User;

    cout << "\nPlease enter new user data:\n\n";

    User.UserName = CheckUsernameExistence(vUsers);

    cout << "Enter Password: ";
    getline(cin, User.Password);

    User.Permissions = ReadPermissions();

    return User;
}

void AddNewUser(vector <stUser>& vUsers)
{
    stUser User = ReadNewUser(vUsers);
    AddDataLineToFile(ConvertUserRecordToLine(User), UsersFile);
}

void AddUsers()
{
    char AddMore = 'Y';
    vector <stUser> vUsers = LoadUsersinVector();

    while(toupper(AddMore) == 'Y')
    {
        system("cls");
        AddNewUserScreen();
        AddNewUser(vUsers);

        cout << "User added successfully! Do you want to add more clients? [y/n]: ";
        cin >> AddMore;
    }
}

void DeleteUserScreen()
{
    system("cls");
    cout << "\n------------------------------------------------\n";
    cout << "              Delete User Screen\n";
    cout << "------------------------------------------------\n";
}

bool FindUserDataUsingUsername(string Username, vector <stUser>& vUsers, stUser& User)
{
    for (stUser& U : vUsers)
    {
        if (U.UserName == Username)
        {
            User = U;
            return true;
        }
    }
    return false;
}

void PrintUserCard(stUser User)
{
    cout << "\nThe following are the user's details:";
    cout << "\n---------------------------------------";
    cout << "\nUsername   : " << User.UserName;
    cout << "\nPassword   : " << User.Password;
    cout << "\nPermissions: " << User.Permissions;
    cout << "\n---------------------------------------";
}

void MarkUserForDeletion(vector <stUser>& vUsers, string Username)
{
    for (stUser& User : vUsers)
    {
        if (User.UserName == Username)
        {
            User.MarkedForDeletion = true;
            break;
        }
    }
}

void SaveUsersVectorToFile(vector <stUser>& vUsers)
{
    fstream MyFile;
    MyFile.open(UsersFile, ios::out);

    if (MyFile.is_open())
    {
        for (stUser& U : vUsers)
        {
            if (!U.MarkedForDeletion)
                MyFile << ConvertUserRecordToLine(U) << endl;
        }
        MyFile.close();
    }
}

void DeleteUserByUsername()
{
    stUser User;
    char AskForDeletion = 'n';
    string Username;
    vector <stUser> vUsers = LoadUsersinVector();

    DeleteUserScreen();
    Username = ReadString("\nPlease enter username: ");

    if (Username == "Admin")
    {
        cout << "You cannot delete this user!";
        return;
    }

    if (FindUserDataUsingUsername(Username, vUsers, User))
    {
        if (Username == "Admin")
            cout << "\nYou cannot delete this user.";
        else
        {
            PrintUserCard(User);

            AskForDeletion = ReadCharacter("\n\nAre you sure you want to delete this user? [y/n]: ");
            if (toupper(AskForDeletion) == 'Y')
            {
                MarkUserForDeletion(vUsers, Username);
                SaveUsersVectorToFile(vUsers);

                vUsers = LoadUsersinVector();

                cout << "\n\nUser deleted successfully!";
            }
        }
    }
    else
    {
        cout << "\nUser with username (" << Username << ") is not found";
    }
}

void FindUserScreen()
{
    system("cls");
    cout << "\n---------------------------------------------\n";
    cout << "           Find User Screen\n";
    cout << "---------------------------------------------\n";
}

void FindUserByUsername()
{
    string Username;
    stUser User;
    vector <stUser> vUsers = LoadUsersinVector();

    FindUserScreen();
    Username = ReadString("\nPlease enter username: ");

    if (FindUserDataUsingUsername(Username, vUsers, User))
        PrintUserCard(User);
    else
        cout << "\nUser with username [" << Username << "] is not found!";
}

void BackToManageUsersScreen()
{
    cout << "\nPress any key to go back to manage users screen...";
    system("pause>0");
    ManageUsersMenu();
}

void UpdateUserInfoScreen()
{
    system("cls");
    cout << "\n---------------------------------------------\n";
    cout << "           Update User Info Screen\n";
    cout << "---------------------------------------------\n";
}

stUser UpdateUserData(string Username)
{
    stUser User;

    User.UserName = Username;

    cout << "\nEnter Password     : ";
    getline(cin >> ws, User.Password);

    User.Permissions = ReadPermissions();

    return User;
}

void UpdateUserDataUsingUsername()
{
    char AskForUpdate = 'n';
    stUser User;
    string Username;
    vector <stUser> vUsers = LoadUsersinVector();

    UpdateUserInfoScreen();
    Username = ReadString("\nPlease enter username: ");

    if (Username == "Admin")
    {
        cout << "\nYou cannot update this user's information!";
        return;
    }

    if (FindUserDataUsingUsername(Username, vUsers, User))
    {
        PrintUserCard(User);

        AskForUpdate = ReadCharacter("\n\nAre you sure you want to update this User's data? [y/n]: ");

        if (toupper(AskForUpdate) == 'Y')
        {
            for (stUser& U : vUsers)
            {
                if (U.UserName == Username)
                {
                    U = UpdateUserData(Username);
                    break;
                }
            }
            SaveUsersVectorToFile(vUsers);

            cout << "\n\nUser's data is updated successfully!";
        }
    }
    else
    {
        cout << "\nClient with account number (" << Username << ") is not found";
    }
}

void PerformManagingUsersOption(enManagingUsers Option)
{
    switch (Option)
    {
    case UsersList:
        ShowUserListScreen();
        BackToManageUsersScreen();
        break;
    case AddANewUser:
        AddUsers();
        BackToManageUsersScreen();
        break;
    case DeleteUser:
        DeleteUserByUsername();
        BackToManageUsersScreen();
        break;
    case UpdateUser:
        UpdateUserDataUsingUsername();
        BackToManageUsersScreen();
        break;
    case FindUser:
        FindUserByUsername();
        BackToManageUsersScreen();
        break;
    case Back2MainMenu:
        MainMenuScreen();
        break;
    }
}

void ManageUsersMenu()
{
    if (!CheckAccessPermission(enPermissions::pManangeUsers))
    {
        AccessDeniedScreen();
        return;
    }

    system("cls");
    cout << "================================================\n";
    cout << "               Manage Users Screen              \n";
    cout << "================================================\n";
    cout << "     [1] Users List.\n";
    cout << "     [2] Add A New User.\n";
    cout << "     [3] Delete A User.\n";
    cout << "     [4] Update A User's Info.\n";
    cout << "     [5] Find A User.\n";
    cout << "     [6] Main Menu.\n";
    cout << "================================================\n";
    PerformManagingUsersOption((enManagingUsers)ReadManagingUsersOption());
}

void PerformOption(enOptions Option)
{
    switch (Option)
    {
    case ClientsList:
        ShowClientsList();
        BackToMainMenu();
        break;
    case AddClient:
        AddClients();
        BackToMainMenu();
        break;
    case DeleteClient:
        DeleteClientByAccNumber();
        BackToMainMenu();
        break;
    case UpdateClient:
        UpdateClientDataUsingAccNumber();
        BackToMainMenu();
        break;
    case FindClient:
        FindClientByAccNumber();
        BackToMainMenu();
        break;
    case Transactions:
        TransactionsMenuScreen();
        BackToMainMenu();
        break;
    case ManageUsers:
        ManageUsersMenu();
        BackToMainMenu();
        break;
    case Logout:
        Login();
        break;
    }
}

void MainMenuScreen()
{
    system("cls");
    cout << "================================================\n";
    cout << "               Main Menu Screen                 \n";
    cout << "================================================\n";
    cout << "     [1] Show Clients List.\n";
    cout << "     [2] Add A New Client.\n";
    cout << "     [3] Delete A Client.\n";
    cout << "     [4] Update A Client's Info.\n";
    cout << "     [5] Find A Client.\n";
    cout << "     [6] Transactions.\n";
    cout << "     [7] Manage Users.\n";
    cout << "     [8] Logout.\n";
    cout << "================================================\n";
    PerformOption((enOptions)ReadOptionNumber());
}

void LoginScreen()
{
    system("cls");
    cout << "\n================================================\n";
    cout << "               Login Screen                 \n";
    cout << "================================================\n";
}

void Login()
{
    stUser User;

    LoginScreen();
    User = ReadUser();

    while (!FindUserByUsernameAndPassword(User, CurrentUser))
    {
        LoginScreen();
        cout << "\nInvalid Username/Password!";
        User = ReadUser();
    }
    MainMenuScreen();
}

int main()
{
    Login();
    return 0;
}