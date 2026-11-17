#include<iostream>
#include<fstream>
#include<vector>
#include<iomanip>
#include<limits>

using namespace std;

const string FileName = "database.txt";

struct stClient {
    string AccountNumber;
    string PINCode;
    string ClientName;
    string Phone;
    double Balance;
    bool MarkToDelete = false;
};

void WaitUntilPress() {
    cout << "\nPress any key to go back to Main menu..." << std::flush;
    //comment the line below if you don't used 'std::cin >>' earlier in your code
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cin.get();
}

vector<string> Split(string Str, string delimiter = " ") {
    string word = "";
    short pos = 0;
    vector<string> tokens;

    while((pos = Str.find(delimiter)) != std::string::npos) {
        word = Str.substr(0, pos);
        if(word != "") {
            tokens.push_back(word);
        }

        Str.erase(0, pos + delimiter.length());
    }

    if(Str != "") {
        tokens.push_back(Str);
    }

    return tokens;
}

bool CheckIfExists(vector<stClient> &vClients, string AccountNumber) {
    
    for(stClient &C : vClients) {
        if(C.AccountNumber == AccountNumber) {
            return true;
        }
    }
    return false;
}


stClient ConvertLineToStruct(string Data, string Seperator = "#//#") {
    vector<string> vData = Split(Data, Seperator);

    stClient Client;

    Client.AccountNumber = vData[0];
    Client.PINCode = vData[1];
    Client.ClientName = vData[2];
    Client.Phone = vData[3];
    Client.Balance = stod(vData[4]);

    return Client;
}

void HandleUserChoose(vector<stClient> &vClients); 

vector<stClient> LoadDataFromFile(string FileName) {
    fstream file;
    file.open(FileName, std::ios::in); // Read mode
    vector<stClient> vClients;

    if(file.is_open()) {
        string Line;
        while(getline(file , Line)) {
            if(Line != "")
                vClients.push_back(ConvertLineToStruct(Line));
        }
        file.close();
    }

    return vClients;
} 

stClient ReadClientData(vector<stClient> &vClients) {
    stClient Client;
    cout << "\n------------------------------------\n";
    cout << "\t\tAdd New Client";
    cout << "\n------------------------------------\n";

    cout << "\nEnter Account Number: ";
    getline(cin >> ws , Client.AccountNumber);

    while(CheckIfExists(vClients, Client.AccountNumber)) {
        cout << "\nThis Account number (" << Client.AccountNumber << ") already used \n";
        cout << "\nEnter Account Number: ";
        getline(cin >> ws , Client.AccountNumber);
    }

    cout << "\nEnter PIN code: ";
    getline(cin >> ws, Client.PINCode);
    cout << "\nEnter Client Name: ";
    getline(cin , Client.ClientName);
    cout << "\nEnter Phone Number: ";
    getline(cin , Client.Phone);
    cout << "\nEnter Client Balance: ";
    cin >> Client.Balance;

    return Client;
}

string ConvertRecordToLine(stClient Client, string Seperator = "#//#") {

    string Line = "";

    Line += Client.AccountNumber + Seperator;
    Line += Client.PINCode + Seperator;
    Line += Client.ClientName + Seperator;
    Line += Client.Phone + Seperator;
    Line += to_string(Client.Balance) + Seperator;

    return Line;
}

void AddOneRecordToFile(stClient Client, string FileName) {
    fstream file;

    file.open(FileName, std::ios::out | std::ios::app);

    if(file.is_open()) {
        file << ConvertRecordToLine(Client) << "\n";
        file.close();
    }
}

bool WriteDataToFile(string FileName, vector<stClient> vClients) {
    fstream file;
    file.open(FileName, std::ios::out);
    
    if(!file.is_open()) {
        return false;
    }

    for(stClient &Cl : vClients) {
        if(!Cl.MarkToDelete) {
            file << ConvertRecordToLine(Cl) << "\n";
        }
    }
    file.close();
    return true;
}

void ShowMainMenu() {

    system("clear");
    cout << "------------------------------------------\n";
    cout << "\t\tMain Menu\n";
    cout << "------------------------------------------\n";
    cout << "\n\t[1] Show clients list.";
    cout << "\n\t[2] Add new client.";
    cout << "\n\t[3] Delete client.";
    cout << "\n\t[4] Update client info.";
    cout << "\n\t[5] Find client.";
    cout << "\n\t[6] Transactions.";
    cout << "\n\t[7] Exit.";
    cout << "\n\n------------------------------------------\n";
}

void AddNewClient(vector<stClient> &vClients) {
    char AddMore = 'y';
    do {
        system("clear");
        AddOneRecordToFile(ReadClientData(vClients), FileName);
        cout << "Do you want to add other client: Y/N ";
        cin >> AddMore;
    } while(AddMore == 'Y' || AddMore == 'y');
    vClients = LoadDataFromFile(FileName);
}

void PrintRow(stClient &Client) {
    cout << "|" << setw(15)  << left << Client.AccountNumber;
    cout << "|" << setw(9)  << left << Client.PINCode;
    cout << "|" << setw(20)  << left << Client.ClientName;
    cout << "|" << setw(15)  << left << Client.Phone;
    cout << "|" << setw(8)  << left << Client.Balance << "|\n";
}

void ShowClientsList(vector<stClient> &vClients) {
    system("clear");
    cout << "\n\t\tClient List (" << vClients.size() << ") client(s)\n\n";
    cout << "\n|-----------------------------------------------------------------------|\n";
    cout << "|" << setw(10) << left << " Account Number";
    cout << "|" << setw(7) << left << " PIN code";
    cout << "|" << setw(20) << left << " Client Name";
    cout << "|" << setw(15) << left  << " Phone";
    cout << "|" << setw(15) << left<< " Balance|";
    cout << "\n|-----------------------------------------------------------------------|\n";
    for(stClient Cl : vClients) {
        PrintRow(Cl);
    }
    cout << "|-----------------------------------------------------------------------|\n";

    WaitUntilPress();
}

void PrintClient(stClient &Client) {
    cout << "\nAccount Number: " << Client.AccountNumber;
    cout << "\nPIN code: " << Client.PINCode;
    cout << "\nClient Name: " << Client.ClientName;
    cout << "\nPhone: " << Client.Phone;
    cout << "\nBalance: " << Client.Balance << "\n";
} 

bool AddDeleteFlag(vector<stClient> &vClients, string AccountNumber) {
    for(stClient &Cl : vClients) {
        if(Cl.AccountNumber == AccountNumber) {
            Cl.MarkToDelete = true;
            return true;
        }
    }
    return false;
}

bool FindClientByAccountNumber(vector<stClient> &vClients, stClient &Client, string AccountNumber) {
    for(stClient &Cl : vClients) {
        if(Cl.AccountNumber == AccountNumber) {
            Client = Cl;
            return true;
        }
    }
    return false;
}

void DeleteClientByAccountNumber(vector<stClient> &vClients) {
    system("clear");
    cout << "---------------------------------\n";
    cout << "\t\tDelete a client\n";
    cout << "---------------------------------\n";

    stClient Client;
    string AccountNumber;
    cout << "\nEnter the account Number: ";
    getline(cin >> ws, AccountNumber);

    if(FindClientByAccountNumber(vClients,Client, AccountNumber )) {
        PrintClient(Client);
        char del;
        cout << "\nAre you sure you want to delete it? Y/N ";
        cin >> del;
        if(del == 'y' || del =='Y') {
                AddDeleteFlag(vClients, AccountNumber);
                WriteDataToFile(FileName ,vClients);
                // refrech the file
                vClients = LoadDataFromFile(FileName);
                cout << "\nClient deleted succusfully.\n";
        }
    } else {
        cout << "\nA client with account number (" << AccountNumber << ") NOT found ";
    }
    WaitUntilPress();
}

stClient ReadNewInfo(string AccountNumber) {
    stClient Client;
    Client.AccountNumber = AccountNumber;
    cout << "\nEnter PIN code: ";
    getline(cin >> ws, Client.PINCode);
    cout << "\nEnter Client Name: ";
    getline(cin, Client.ClientName);
    cout << "\nEnter Phone number: ";
    getline(cin, Client.Phone);
    cout << "\nEnter Client's new balance: ";
    cin >> Client.Balance;
    return Client;
}

bool UpdateValues(vector<stClient> &vClients , string AccountNumber) {
    for(stClient &Cl : vClients) {
        if(Cl.AccountNumber == AccountNumber) {
            Cl = ReadNewInfo(AccountNumber);
            return true;
        }
    }
    return false;
}

void UpdateClientInfo(vector<stClient> &vClients) {
    system("clear");
    cout << "---------------------------------\n";
    cout << "\t\tUpdate client's data\n";
    cout << "---------------------------------\n";
    string AccountNumber;
    stClient Client;

    cout << "\nEnter Account Number: ";
    cin >> AccountNumber;

    if(FindClientByAccountNumber(vClients, Client, AccountNumber)) {
        PrintClient(Client);
        cout << "\nDo you want to update this client's data? Y/N";
        char up;
        cin >> up;
        if(up == 'y' || up == 'Y') {
            UpdateValues(vClients, AccountNumber);
            WriteDataToFile(FileName, vClients);
        }
    } else {
        cout << "\nClient with this Account Number (" << AccountNumber << ") NOT found\n";
    }

    WaitUntilPress();
}

void FindClient(vector<stClient> &vClients) {
    system("clear");
    cout << "---------------------------------\n";
    cout << "\t\tFind clients\n";
    cout << "---------------------------------\n";
    string AccountNumber;
    stClient Client;

    cout << "\nEnter Account Number: ";
    cin >> AccountNumber;

    if((FindClientByAccountNumber(vClients,Client, AccountNumber))) {
        PrintClient(Client);
    } else {
        cout << "\nClient with this Account number (" << AccountNumber << ") not found! \n";
    }
    WaitUntilPress();
}

void ShowEndMessage() {
    system("clear");
    cout << "---------------------------------\n";
    cout << "\t\tProgram Ends :) Good By!!\n";
    cout << "---------------------------------\n";
}

void ChangeBalance(vector<stClient> &vClients , string AccountNumber, double Amount) {
    for(stClient &Cl : vClients) {
            if(Cl.AccountNumber == AccountNumber) {
                char Confirm;
                cout << "\nConfirm Y/N: ";
                cin >> Confirm;
                if(Confirm == 'y'  || Confirm == 'Y') {
                    Cl.Balance += Amount;
                    cout << "\nSuccuss.";
                    WriteDataToFile(FileName, vClients);
                    break;
                } else {
                    cout << "\nCanceled!";
                    break;
                }
            }
    }
}

void AddDeposite(vector<stClient> &vClients) {
    string AccountNumber;
    cout << "\nEnter account number: ";
    cin >> AccountNumber;
    stClient Client;
    if(FindClientByAccountNumber(vClients, Client , AccountNumber)) {
        double Amount = 0;
        PrintClient(Client);
        cout << "\nHow much you want to deposite: ";
        cin >> Amount;
        ChangeBalance(vClients, AccountNumber, Amount);
    } else {
        cout << "\nNo client with this account Number (" << AccountNumber << ")";
    }
}

void ShowDepositScreen(vector<stClient> &vClients) {
    system("clear");
    cout << "---------------------------------\n";
    cout << "\t\tDeposite screen\n";
    cout << "---------------------------------\n";
    AddDeposite(vClients);
    WaitUntilPress();
}

void Withdraw(vector<stClient> &vClients) {
    string AccountNumber;
    cout << "\nEnter account number: ";
    cin >> AccountNumber;
    stClient Client;
    if(FindClientByAccountNumber(vClients, Client , AccountNumber)) {
        PrintClient(Client);
        double Amount = 0;
        cout << "\nEnter withdraw amount: ";
        cin >> Amount;

        while(Client.Balance < Amount) {
            cout << "\nYou can't withdraw up to " << Client.Balance;
            cout << "\nEnter withdraw amount: ";
            cin >> Amount;
        }

        ChangeBalance(vClients, AccountNumber, -Amount);
    } else {
        cout << "\nNo client with this account Number (" << AccountNumber << ")";
    }
}

void ShowWithdrawScreen(vector<stClient> &vClients) {
    system("clear");
    cout << "---------------------------------\n";
    cout << "\t\tWithdraw screen\n";
    cout << "---------------------------------\n";
    Withdraw(vClients);
    WaitUntilPress();
}

double ShowTotalBalance(vector<stClient> &vClients) {
    double TotalBalance = 0.0;

    for(stClient &Cl : vClients) {
        TotalBalance += Cl.Balance;
    }
    return TotalBalance;
}

void ShowTotalBalanceScreen(vector<stClient> &vClients) {
    system("clear");
    cout << "---------------------------------\n";
    cout << "\t\tTotal Balance screen\n";
    cout << "---------------------------------\n";
    cout << "\nTotal Balance is: " << ShowTotalBalance(vClients);
    WaitUntilPress();
}

void HandleUserOption(vector<stClient> &vClients, bool &run) {
    short option;
    cout << "\nWhat do you want to do [1 - 4]? ";
    cin >> option;

    switch (option)
    {
    case 1:
        ShowDepositScreen(vClients);
        break;
    case 2:
        ShowWithdrawScreen(vClients);
        break;
    case 3:
        ShowTotalBalanceScreen(vClients);
        break;
    case 4:
        run = false;
        break;
    default:
        cout << "\nWrong option:(";
        WaitUntilPress();
        break;
    }
}

void ShowTransactionsMenu(vector<stClient> &vClients) {
    bool run = true;
    while(run) {
        system("clear");
        cout << "------------------------------------------\n";
        cout << "\t\tTransactions Menu\n";
        cout << "------------------------------------------\n";
        cout << "\n\t[1] Deposite.";
        cout << "\n\t[2] Withdraw.";
        cout << "\n\t[3] Total balances.";
        cout << "\n\t[4] Main Menu.";
        cout << "\n\n------------------------------------------\n";
        HandleUserOption(vClients ,run);
    }
}

void HandleUserChoose(vector<stClient> &vClients, bool &run) {
    short Option;
    cout << "\n Choose what do you want to do? [1 to 6]? ";
    cin >> Option;

    switch(Option) {
        case 1 :
            ShowClientsList(vClients);
            break;
        case 2 :
            AddNewClient(vClients);
            break;
        case 3 :
            DeleteClientByAccountNumber(vClients);
            break;
        case 4 :
            UpdateClientInfo(vClients);
            break;
        case 5 :
            FindClient(vClients);
            break;
        case 6 :
            ShowTransactionsMenu(vClients);
            return;
        case 7 :
            ShowEndMessage();
            run = false;
            break;
        default:
            system("clear");
            cout << "\nThis is wrong option:) ";
            WaitUntilPress();
            break;
    }
}

void Start() {
    vector<stClient> vClients = LoadDataFromFile(FileName);
    bool run = true;
    while(run) {
        ShowMainMenu();
        HandleUserChoose(vClients, run);
    }
}

int main()  {

    Start();
    return 0;
}
