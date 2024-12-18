// ATM-System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
#include<string>
#include<fstream>
#include <iomanip>
using namespace std;
void ShowMainMenue();
void Login();
void ShowQuickWithdrawScreen();
void ShowNormalWithDrawScreen();
void ShowDepositScreen();

vector<string> SplitString(string sentence, string delim = "#//#") {
    short pos;
    string sWord = "";
    vector<string>vSentence;
    //use find() function to get the position of delimiter
    while ((pos = sentence.find(delim)) != string::npos) {
        sWord = sentence.substr(0, pos);
        if (sWord != delim) {
            vSentence.push_back(sWord);
        }
        //erase() until position and move to the next word
        sentence.erase(0, pos + delim.length());
    }
    if (sentence != delim) {
        vSentence.push_back(sentence);
    }
    return vSentence;
}
const string FileName = "MyFile.txt";
struct stClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    double totalBalances;
    bool MarkDelete = false;
    bool Add = false;
};

stClient CurrentClient;

enum eATM {

    QuickWithdraw = 1,
    NormalWithdraw,
    Deposit,
    CheckBalance,
    Logout
};
stClient ConvertLineToRecord(string Line, string seperator = "#//#") {
    stClient Client;
    vector<string>vSentence;
    vSentence = SplitString(Line, seperator);
    Client.AccountNumber = vSentence[0];
    Client.PinCode = vSentence[1];
    Client.Name = vSentence[2];
    Client.Phone = vSentence[3];
    Client.AccountBalance = stod(vSentence[4]);
    return Client;
}
string  ConvertRecordToLine(stClient Client, string seperator = "#//#") {
    string stClientRecord = "";
    vector<stClient>vClient;
    stClientRecord += Client.AccountNumber + seperator;
    stClientRecord += Client.PinCode + seperator;
    stClientRecord += Client.Name + seperator;
    stClientRecord += Client.Phone + seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}
vector<stClient> LoadClientDataFromFile(string FileName) {
    vector <stClient> vClientRecords;
    fstream MyFile;
    short counter = 0;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open()) {
        string Line;
        stClient Client;
        while (getline(MyFile, Line)) {
            Client = ConvertLineToRecord(Line);
            vClientRecords.push_back(Client);
        }
        MyFile.close();
    }
    return vClientRecords;
}
bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient& Client)
{
    vector <stClient> vClient = LoadClientDataFromFile(FileName);
    for (stClient C : vClient)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }
    return false;
}
vector <stClient> SaveClientToFile(string FileName, vector<stClient> vClient) {
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    string DataLine;
    if (MyFile.is_open()) {
        for (stClient c : vClient) {
            DataLine = ConvertRecordToLine(c);
            MyFile << DataLine << endl;
        }
        MyFile.close();
    }
    return vClient;
}
bool DepositBalanceToClientByAccountNumber(string AccountNumber,double Amount, vector <stClient>& vClients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction?y / n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        for (stClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientToFile(FileName, vClients);
                cout << "\n\nDone Successfully. New balance is: "<< C.AccountBalance;
                return true;
            }
        }
        return false;
    }
}
short ReadQuickWithdrawOption()
{
    short Choice = 0;
    while (Choice < 1 || Choice>9)
    {
        cout << "\nChoose what to do from [1] to [9] ? ";
        cin >> Choice;
    }
    return Choice;
}
short getQuickWithdrawAmount(short QuickWithDrawOption) {
    switch (QuickWithDrawOption)
    {
        case 1:
            return 20;
        case 2:
            return 50;
        case 3:
            return 100;
        case 4:
            return 200;
        case 5:
            return 400;
        case 6:
            return 600;
        case 7:
            return 800;
        case 8:
            return 1000;
        default:
            return 0;
    }
}
void PerfromQuickWithdrawOption(short QuickWithDrawOption)
{
    if (QuickWithDrawOption == 9)//exit
        return;
    short WithDrawBalance =getQuickWithdrawAmount(QuickWithDrawOption);
    if (WithDrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make anotherchoice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    }
    vector <stClient> vClients =LoadClientDataFromFile(FileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber,WithDrawBalance * -1, vClients);
    CurrentClient.AccountBalance -= WithDrawBalance;
}
double ReadDepositAmount()
{
    double Amount;
    cout << "\nEnter a positive Deposit Amount? ";
    cin >> Amount;
    while (Amount <= 0)
    {
        cout << "\nEnter a positive Deposit Amount? ";
        cin >> Amount;
    }
    return Amount;
}
void PerfromDepositOption()
{
    double DepositAmount = ReadDepositAmount();
    vector <stClient> vClients =LoadClientDataFromFile(FileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber,DepositAmount, vClients);
    CurrentClient.AccountBalance += DepositAmount;
}
void ShowDepositScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tDeposit Screen\n";
    cout << "===========================================\n";
    PerfromDepositOption();
}
void ShowCheckBalanceScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tCheck Balance Screen\n";
    cout << "===========================================\n";
    cout << "Your Balance is " << CurrentClient.AccountBalance <<"\n";
}
int ReadWithDrawAmont()
{
    int Amount;
    cout << "\nEnter an amount multiple of 5's ? ";
    cin >> Amount;
    while (Amount % 5 != 0)
    {
        cout << "\nEnter an amount multiple of 5's ? ";
        cin >> Amount;
    }
    return Amount;
}
void PerfromNormalWithdrawOption()
{
    int WithDrawBalance = ReadWithDrawAmont();
    if (WithDrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");
        ShowNormalWithDrawScreen();
        return;
    }
    vector <stClient> vClients =LoadClientDataFromFile(FileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber,WithDrawBalance * -1, vClients);
    CurrentClient.AccountBalance -= WithDrawBalance;
}
void ShowNormalWithDrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tNormal Withdraw Screen\n";
    cout << "===========================================\n";
    PerfromNormalWithdrawOption();
}
void ShowQuickWithdrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tQucik Withdraw\n";
    cout << "===========================================\n";
    cout << "\t[1] 20\t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "===========================================\n";
    cout << "Your Balance is " << CurrentClient.AccountBalance;
    PerfromQuickWithdrawOption(ReadQuickWithdrawOption());
}


void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}
short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 5]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}
bool  LoadClientInfo(string AccountNumber, string PinCode)
{
    if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;
}

void PerfromMainMenueOption(short Operation) {
    stClient Client;
    switch (Operation)
    {
    case eATM::QuickWithdraw:
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToMainMenue();
        break;
    case eATM::NormalWithdraw:
        system("cls");
        ShowNormalWithDrawScreen();
        GoBackToMainMenue();
        break;
    case eATM::Deposit:
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenue();
        break;
    case eATM::CheckBalance:
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToMainMenue();
        break;
    case eATM::Logout:
        system("cls");
        Login();
        break;
    }
}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((eATM)ReadMainMenueOption());
}
void Login() {
    bool LoginFaild = false;
    string AccountNumber, PinCode;
    do
    {
        system("cls");
        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";
        if (LoginFaild)
        {
            cout << "Invlaid AccountNumber/PinCode!\n";
        }
        cout << "Enter AccountNumber? ";
        cin >> AccountNumber;
        cout << "Enter PinCode? ";
        cin >> PinCode;
        LoginFaild = !LoadClientInfo(AccountNumber, PinCode);
    } while (LoginFaild);
    ShowMainMenue();
}
int main()
{
    Login();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
