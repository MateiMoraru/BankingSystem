/*!
    TODO: Maybe use "cin.getline" instead of "cin >> " to prevent buffer overflow?
    TODO: Color codes?
*/

#include <iostream>
#include <ctime>
#include <chrono>
#include <cstring>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cctype>
using namespace std;

const char SAVE_FILE_NAME[20] = "utilizatori.in";

struct date
{
    int day, month, year;
};

struct transaction
{
    int emitter_id;
    int receiver_id;
    int sum;
    date transaction_date;
};

struct client
{
    char name[99];
    char surname[99];
    int _id;
    float balance;
    transaction transactions[100];
    int length_transactions;
};

void print_client_transaction(transaction *transfer)
{

    cout << "FROM: " << transfer -> emitter_id << endl
            << "TO: " << transfer -> receiver_id << endl
            << "SUM: " << transfer -> sum << endl
            << "DATE:\t" << transfer -> transaction_date.day << "/"
                         << transfer -> transaction_date.month << "/"
                         << transfer -> transaction_date.year << endl << endl;
}

void print_client_transactions(client &current_client)
{
    transaction current_transaction;
    for(int i = 0; i < current_client.length_transactions; i++)
    {
        current_transaction = current_client.transactions[i];
        cout << "transaction " << i + 1 << ": " << endl;
        print_client_transaction(&current_transaction);
    }
}

void print_client_data(client &current_client)
{
    cout << "Nume: " << current_client.name << endl;
    cout << "Prenume: " << current_client.surname << endl;
    cout << "ID: " << current_client._id << endl;
    cout << "Sold: " << current_client.balance << endl;
    cout << endl;
    print_client_transactions(current_client);
    cout << endl;
}

void first_menu()
{
    cout << endl;
    cout << "Login into account (login)" << endl;
    cout << "Create account (create)" << endl;
    cout << "Show accounts (show)" << endl;
    cout << "exit program (EXIT)" << endl << endl;
}

void secondary_menu()
{
    cout << endl;
    cout << "Show data (show)" << endl;
    cout << "Sort transactions (sort)" << endl;
    cout << "Search transaction (search)" << endl;
    cout << "Send money (transfer)" << endl;
    cout << "Deposit money (deposit)" << endl;
    cout << "Logout (logout)" << endl << endl;

}

void show_data_menu()
{
    cout << endl;
    cout << "\tSee client data" << endl;
    cout << "Name (name)" << endl;
    cout << "Surname (surname)" << endl;
    cout << "Balance (balance)" << endl;
    cout << "ID (id)" << endl;
    cout << "Transactions (transactions)" << endl;
    cout << "ALL (a)" << endl << endl;
}

void sort_transactions_menu()
{
    cout << "\tSort transactions" << endl;
    cout << "Sort by date (date)" << endl;
    cout << "Sort by sum (sum)" << endl;
}

void search_transactions_menu()
{
    cout << endl;
    cout << "\tSearch Transactions" << endl;
    cout << "Receiver id (receiver)" << endl;
    cout << "Emitter id (emitter)" << endl;
    cout << "Sum of money (sum)" << endl;
    cout << "Date (date)" << endl << endl;
}
void except_error(char *message, int code)
{
    cout << "\t" << message << "!" << endl;
    exit(code);
}

int cin_fail_int() // If program waited for int and got string
{
    int value;
    while (true)
    {
        cin >> value;

        if (!cin.fail()) // Checks if got wrong input
            return value; // If not, return int

        cin.clear(); // Clears the state of the cin
        cin.ignore(10000, '\n'); // Clears the data in cin
        cout << "Please enter a valid int, or type -1 to exit: ";
    }
    return value;
}

bool is_char_in_array(char* arr, char target) {
    for (int i = 0; arr[i] != '\0'; i++) {
        if (arr[i] == target) {
            return true;
        }
    }
    return false;
}

int size_array(char* arr)
{
    int s = 0;
    while(arr[s] != '\0')
        s++;
    return s + 1;
}

float str_to_float(char *str)
{
    float result = 0.0f;
    float decimal_mult = 0.1f;
    bool is_negative = false;
    bool is_decimal = false;

    int i = 0;

    if (str[i] == '-') {
        is_negative = true;
        ++i;
    } else if (str[i] == '+') {
        ++i;
    }

    for (; i < strlen(str); ++i) {
        char ch = str[i];

        if (ch == '.') {
            if (is_decimal) {
                except_error("Invalid input: multiple decimal points", -1);
                return 0.0f;
            }
            is_decimal = true;
            continue;
        }

        if (!isdigit(ch)) {
            char msg[999] = "Invalid input: contains non-numeric characters : ";
            msg[strlen(msg)] = ch;
            except_error(msg, -1);
            return 0.0f;
        }

        int digit = ch - '0';

        if (is_decimal) {
            result += digit * decimal_mult;
            decimal_mult *= 0.1f;
        } else {
            result = result * 10 + digit;
        }
    }

    if (is_negative) {
        result = -result;
    }

    return result;
}

char* float_to_char(float number) {
    static char str[50];  // Buffer to hold the resulting string

    // Check if the number is a whole number (no decimal part)
    if (number == (int)number) {
        // Convert the integer part to string manually
        int int_part = (int)number;
        int index = 0;

        // Handle the integer part manually (like 'itoa')
        do {
            str[index++] = '0' + (int_part % 10);
            int_part /= 10;
        } while (int_part > 0);

        // Reverse the digits
        for (int i = 0; i < index / 2; i++) {
            char temp = str[i];
            str[i] = str[index - 1 - i];
            str[index - 1 - i] = temp;
        }
        str[index] = '\0';  // Null terminates the string
    } else {
        // Handle float formatting (two decimal places)
        int int_part = (int)number;
        int decimal_part = (number - int_part) * 100; // Get two decimal places
        int index = 0;

        // Handle the integer part
        int temp_int = int_part;
        do {
            str[index++] = '0' + (temp_int % 10);
            temp_int /= 10;
        } while (temp_int > 0);

        // Reverse the digits of the integer part
        for (int i = 0; i < index / 2; i++) {
            char temp = str[i];
            str[i] = str[index - 1 - i];
            str[index - 1 - i] = temp;
        }

        // Add decimal point
        str[index++] = '.';

        // Handle the decimal part
        int decimal_temp = decimal_part;
        str[index++] = '0' + (decimal_temp / 10);  // Tens place
        str[index++] = '0' + (decimal_temp % 10);  // Ones place
        str[index] = '\0';  // Null terminate the string
    }

    return str;
}

bool is_file_empty(const char* filename) {
    ifstream fin(filename);
    if(!fin.is_open()) return true;

    char line[10000];
    char ch;

    while (fin.getline(line, 10000)) {
        // Check if the line contains at least one non-whitespace character
        bool is_non_whitespace = false;

        for (int i = 0; line[i] != '\0'; i++) {
            ch = line[i];
            if (ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n') {
                is_non_whitespace = true;
                break;
            }
        }
        if (is_non_whitespace) {
            return false; // Found non-whitespace content, so file is not empty
        }
    }
    return true; // Only blank lines were found, so the file is empty
}

int read_clients(client *clients)
{
    int length_clients = 0;

    ifstream fin(SAVE_FILE_NAME);

    if (!fin.is_open()) {
        cout << "Error: Could not open the file \"" << SAVE_FILE_NAME << "\". Creating a new file." << endl;

        ofstream fout(SAVE_FILE_NAME);

        if (!fout.is_open()) {
            cout << "Error: Failed to create the file." << endl;
            return 1;
        }

        cout << "A new file \"" << SAVE_FILE_NAME << "\" has been created." << endl;

        fout.close();

        return 0;
    }

    if(is_file_empty(SAVE_FILE_NAME))
    {
        cout << "The file is empty. Try creating some accounts and exiting using the proper command." << endl;
        return 0;
    }


    client current_client;
    current_client.length_transactions = 0;
    date transaction_date = {0, 0, 0}; // 3h waste of time because i didnt init this
    transaction current_transaction;

    char str[9999] = "";

    while(fin >> str)
    {
        // Basic user data
        // Use of strncpy and '\0' at the end in order not to get a buffer overflow (srncpy checks for buffer overflow, while strcpy does not)
        strncpy(current_client.name, str, size_array(str) - 1);
        current_client.name[size_array(str) - 1] = '\0';

        fin >> str;
        strncpy(current_client.surname, str, size_array(str) - 1);
        current_client.surname[size_array(str) - 1] = '\0';

        fin >> str;
        current_client._id = str_to_float(str);

        fin >> str;
        current_client.balance = str_to_float(str);

        fin >> str;

        // Transactions
        if(str[0] == '|')
        {
            while(str[0] != '}')
            {
                fin >> str;
                if(str[0] != '}')
                {
                    current_transaction.emitter_id = str_to_float(str);
                    fin >> str;
                    current_transaction.receiver_id = str_to_float(str);
                    fin >> str;
                    current_transaction.sum = str_to_float(str);
                    fin >> str;

                    transaction_date.day = str_to_float(str);
                    fin >> str;
                    transaction_date.month = str_to_float(str);
                    fin >> str;
                    transaction_date.year = str_to_float(str);

                    current_transaction.transaction_date = transaction_date;
                    current_client.transactions[current_client.length_transactions] = current_transaction;
                    current_client.length_transactions++;

                    fin >> str;
                }
            }

        }

        clients[length_clients] = current_client;
        length_clients++;
        memset(&current_client, 0, sizeof(current_client)); // Resetting the user data (for transactions especially)

    }
    fin.close();
    return length_clients;
}

client* login(client clients[], int length_clients)
{
    int id;
    cout << "Input your id: ";
    id = cin_fail_int();

    if(id == -1)
        return nullptr;

    for(int i = 0; i < length_clients; i++)
    {
        if(id == clients[i]._id)
        {
            return &clients[i];
        }
    }
    cout << "Couldn't find account with that certain id. Try again, or try creating an account!" << endl << endl;
    return nullptr;
}

void unknown_command_err()
{
    cout << "Unknown command! Use the commands from the parantheses!" << endl << endl;
}

client create_account(int clients_length)
{
    int id = clients_length + 1000 + 1;
    int balance = 0;
    char name[999], surname[999];

    cout << "\tCreate account" << endl;
    cout << "Enter your name: ";
    cin >> name;
    cout << endl << "Enter your surname (if you have two/more surnames, use \"-\" between them): ";
    cin >> surname;
    cout << endl;

    client user;
    strcpy(user.name, name);
    strcpy(user.surname, surname);
    user.balance = balance;
    user._id = id;
    user.length_transactions = 0;

    return user;
}

void transfer(client *clients, client *selected_client, date current_date, int length_clients)
{
    int emitter = selected_client -> _id;
    int receiver;
    int sum;

    cout << "\tTransfer money" << endl;
    cout << "To whom do you want to send money to? ";
    cin >> receiver;

    int ok = 0;
    while((receiver == emitter || receiver <= 1000 || receiver >= length_clients || cin.fail()) && ok == 0)
    {
        if(receiver == -1)
            ok = 1;
        else
        {
            if(cin.fail())
            {
                cin.clear();
                cin.ignore(10000, '\n');
            }
            cout << "You must input a valid id! (id>1000, can't be your own account, must be a digit!). Enter -1 if you want to exit this loop." << endl << "Input receiver id: ";
            cin >> receiver;
            cout << endl;
        }
    }
    cout << endl << "How much do you want to send? ";
    cin >> sum;

    ok = 0;
    while((sum > selected_client -> balance || sum <= 0 || cin.fail()) && ok == 0)
    {
        if(sum == -1)
            ok = 1;
        else
        {
            if(cin.fail())
            {
                cin.clear();
                cin.ignore(10000, '\n');
            }
            cout << "Please input a valid number! (either inputted number is not a digit, is negative/null, or it is greater than your funds!). Enter -1 if you want to exit this loop." << endl
                 <<"Input the sum you want to transfer: ";
            cin >> sum;
            cout << endl;
        }
    }

    transaction new_transaction;
    new_transaction.emitter_id = emitter;
    new_transaction.receiver_id = receiver;
    new_transaction.sum = sum;
    new_transaction.transaction_date = current_date;

    client *r_client = &clients[receiver - 1001];
    r_client -> transactions[r_client ->length_transactions] = new_transaction;
    r_client -> length_transactions++;
    r_client -> balance += sum;
    selected_client -> transactions[selected_client ->length_transactions] = new_transaction;
    selected_client ->length_transactions++;
    selected_client -> balance -= sum;
}

// Search transaction

// If there are multiple transfers that match the criteria, show all of them
// Do not save them, just print them numbered (maybe)

void search_transaction_receiver_id(client *current_client, int receiver_id)
{
    int length = current_client -> length_transactions;
    int transaction_idx = 1;

    for(int i = 0; i < length; i++)
    {
        if(current_client -> transactions[i].receiver_id == receiver_id)
        {
            cout << transaction_idx++ << ".";
            print_client_transaction(&current_client -> transactions[i]);
        }
    }
}

void search_transaction_emitter_id(client *current_client, int emitter_id)
{
    int length = current_client -> length_transactions;
    int transaction_idx = 1;

    for(int i = 0; i < length; i++)
    {
        if(current_client -> transactions[i].emitter_id == emitter_id)
        {
            cout << transaction_idx++ << ".";
            print_client_transaction(&current_client -> transactions[i]);
        }
    }
}

void search_transaction_sum(client *current_client, int sum)
{
    int length = current_client -> length_transactions;
    int transaction_idx = 1;

    for(int i = 0; i < length; i++)
    {
        if(current_client -> transactions[i].sum == sum)
        {
            cout << transaction_idx++ << ".";
            print_client_transaction(&current_client -> transactions[i]);
        }
    }
}

void search_transaction_date(client *current_client, int day, int month, int year)
{
    int length = current_client -> length_transactions;
    int transaction_idx = 1;

    for(int i = 0; i < length; i++)
    {
        if(current_client -> transactions[i].transaction_date.day == day &&
           current_client -> transactions[i].transaction_date.month == month &&
           current_client -> transactions[i].transaction_date.year == year)
        {
            cout << transaction_idx++ << ".";
            print_client_transaction(&current_client -> transactions[i]);
        }
    }
}

int compare_dates(date date1, date date2)
{
    if(date1.year < date2.year) return 1;  // Older date 1
    if(date1.year > date2.year) return 0;  // Newer date 0

    if(date1.month < date2.month) return 1;
    if(date1.month > date2.month) return 0;

    if(date1.day < date2.day) return 1;
    if(date1.day > date2.day) return 0;

    return 0; // Equal dates
}


void sort_transactions_sum(client *current_client, int greatest)
{
    int length = current_client -> length_transactions;
    transaction transactions[length];
    for(int i = 0; i < length; i++)
    {
        transactions[i] = current_client -> transactions[i];
    }

    for(int i = 0; i < length - 1; i++)
    {
        int idx_min = i;
        for(int j = i + 1; j < length; j++)
        {
            if(transactions[j].sum < transactions[idx_min].sum)
                idx_min = j;
        }

        transaction aux = transactions[i];
        transactions[i] = transactions[idx_min];
        transactions[idx_min] = aux;
    }

    if(greatest == 0)
        for(int i = 0; i < length; i++)
            print_client_transaction(&transactions[i]);
    else
        for(int i = length - 1; i >= 0; i--)
            print_client_transaction(&transactions[i]);
}

void sort_transactions_date(client *current_client, int recent)
{
    //recent == 1 -> print from recent
    int length = current_client -> length_transactions;
    transaction transactions[length];
    for(int i = 0; i < length; i++)
    {
        transactions[i] = current_client -> transactions[i];

    }

    for(int i = 0; i < length - 1; i++)
    {
        int idx_min = i;
        for(int j = i + 1; j < length; j++)
        {
            if(compare_dates(transactions[i].transaction_date, transactions[idx_min].transaction_date) == 0)
            {
                idx_min = j;
            }
        }

        transaction aux = transactions[i];
        transactions[i] = transactions[idx_min];
        transactions[idx_min] = aux;
    }

    if(recent == 0)
        for(int i = 0; i < length; i++)
            print_client_transaction(&transactions[i]);
    else
        for(int i = length - 1; i >= 0; i--)
            print_client_transaction(&transactions[i]);
}

void client_to_text(client *user, char *str)
{
    strcat(str, user -> name);
    strcat(str, " ");
    strcat(str, user -> surname);
    strcat(str, " ");
    strcat(str, float_to_char(user -> _id));
    strcat(str, " ");
    strcat(str, float_to_char(user -> balance));
    strcat(str, " | ");

    for(int i = 0; i < user -> length_transactions; i++)
    {
        strcat(str, float_to_char(user -> transactions[i].emitter_id));
        strcat(str, " ");
        strcat(str, float_to_char(user -> transactions[i].receiver_id));
        strcat(str, " ");
        strcat(str, float_to_char(user -> transactions[i].sum));
        strcat(str, " ");
        strcat(str, float_to_char(user -> transactions[i].transaction_date.day));
        strcat(str, " ");
        strcat(str, float_to_char(user -> transactions[i].transaction_date.month));
        strcat(str, " ");
        strcat(str, float_to_char(user -> transactions[i].transaction_date.year));
        strcat(str, " ");
        strcat(str, "; ");

    }

    strcat(str, " }\n");
}

int main()
{
    // Initializing current date for automating future transactions
    date current_date;
    time_t t = time(nullptr);
    tm *const current_time = localtime(&t);
    current_date.year = 1900 + current_time -> tm_year;
    current_date.month = 1 + current_time -> tm_mon;
    current_date.day = current_time -> tm_mday;

    int length_clients = 0;
    client clients[100];
    client *selected_client;
    bool logged_in = false;

    cout << "---ADMINISTRAREA UNEI APLICATII BANCARE---" << endl;
    cout << "Date: " << current_date.day << '/' << current_date.month << '/' << current_date.year << endl;

    cout << "Pentru a folosi programul, tasteaza comanda din paranteza!" << endl;
    cout << "Pentru a sterge ecranul, tasteaza \"clear\"!" << endl;
    cout << "Pentru a salva modificarile facute in useri, tastati \"exit\" pentru a termina programul!" << endl << endl;

    length_clients = read_clients(clients);

    char command[99] = "";
    char subcommand[99] = "";

    bool running = strcmp(command, "EXIT") != 0 && strcmp(command, "exit") != 0;
    while(running)
    {
        if(!logged_in)
            first_menu();
        else
            secondary_menu();

        cout << "Input: ";
        cin >> command;

        running = strcmp(command, "EXIT") != 0 && strcmp(command, "exit") != 0;

        if (cin.fail()) {
            cin.clear();  // Clears the error state
            cin.ignore(10000, '\n');  // Ignores the rest of the input
            cout << "Invalid input, please try again!" << endl;
            continue; // Goes back to the start of the while loop, not running the following code :)
        }

        cout << endl;

        if(strcmp(command, "clear") == 0)
            system("cls");
        else if(running)
        {
            if(!logged_in)
            {
                if(strcmp(command, "login") == 0)
                {
                    cout << "\tLogin into account" << endl;
                    selected_client = login(clients, length_clients);
                    if(selected_client != nullptr)
                    {
                        cout << "\tSuccessfully logged in, " << selected_client->name << "!" << endl;
                        logged_in = true;
                    }
                }
                else if(strcmp(command, "create") == 0)
                {
                    clients[length_clients] = create_account(length_clients);
                    print_client_data(clients[length_clients]);
                    length_clients++;
                }
                else if(strcmp(command, "show") == 0)
                {
                    if(length_clients == 0)
                    {
                        cout << "No accounts found! Try creating some and exit the program correctly in order to save!" << endl;
                    }
                    else
                        for(int i = 0; i < length_clients; i++)
                        {
                            print_client_data(clients[i]);
                        }
                }
                else
                {
                    unknown_command_err();
                }
            }
            else // Logged in
            {
                if(strcmp(command, "show") == 0)
                {
                    show_data_menu();
                    cin >> subcommand;
                    if(strcmp(subcommand, "a") == 0)
                        print_client_data(*selected_client);
                    else if(strcmp(subcommand, "name") == 0)
                        cout << "\tName: " << selected_client -> name << endl;
                    else if(strcmp(subcommand, "surname") == 0)
                        cout << "\tSurame: " << selected_client -> surname << endl;
                    else if(strcmp(subcommand, "balance") == 0)
                        cout << "\tBalance: " << selected_client -> balance << endl;
                    else if(strcmp(subcommand, "id") == 0)
                        cout << "\tID: " << selected_client -> _id << endl;
                    else if(strcmp(subcommand, "transactions") == 0)
                        print_client_transactions(*selected_client);
                    else
                        unknown_command_err();

                }
                else if(strcmp(command, "search") == 0)
                {
                    if(selected_client -> length_transactions == 0)
                        cout << "No transactions!" << endl;
                    else
                    {
                        search_transactions_menu();
                        cin >> subcommand;
                        if(strcmp(subcommand, "receiver") == 0)
                        {
                            int id;
                            cout << "Specify the id: ";
                            id = cin_fail_int();
                            if(id != -1)
                                search_transaction_receiver_id(selected_client, id);
                        }
                        else if(strcmp(subcommand, "emitter") == 0)
                        {
                            int id;
                            cout << "Specify the id: ";
                            id = cin_fail_int();
                            cout << endl;
                            if(id != -1)
                                search_transaction_emitter_id(selected_client, id);
                        }
                        else if(strcmp(subcommand, "sum") == 0)
                        {
                            int sum;
                            cout << "Specify the id: ";
                            sum = cin_fail_int();
                            cout << endl;
                            if(sum != -1)
                                search_transaction_sum(selected_client, sum);
                        }
                        else if(strcmp(subcommand, "date") == 0)
                        {
                            cout << "Specify the day, month and year (use a space between them): ";
                            int day, month, year;
                            cin >> day >> month >> year;
                            cout << endl;
                            // Here it would be annoying to look for fail in every case :)
                            if(!cin.fail())
                                search_transaction_date(selected_client, day, month, year);
                            else
                            {
                                cout << "Please input valid numbers!!!" << endl;
                                cin.clear();
                                cin.ignore(10000, '\n');
                            }

                        }
                        else
                            unknown_command_err();
                    }
                }
                else if(strcmp(command, "sort") == 0)
                {
                    if(selected_client -> length_transactions == 0)
                        cout << "No transactions!" << endl;
                    else
                    {
                        sort_transactions_menu();
                        cin >> subcommand;

                        if(strcmp(subcommand, "date") == 0)
                        {
                            cout << "Sort from most recent to oldest (type recent, else oldest):" << endl;

                            char recent[100];
                            int recent_int;

                            recent = cin_fail_int();
                            cout << endl;

                            if(strcmp(recent, "oldest") == 0)
                            {
                                cout << "Sorting from most recent to oldest" << endl << endl;
                                recent_int = 0;
                            }
                            else if(strcmp(recent, "recent") == 0)
                            {
                                cout << "Sorting from oldest to most recent" << endl << endl;
                                recent_int = 1;
                            }
                            else
                                recent_int = -1;

                            if(recent_int != -1)
                                sort_transactions_date(selected_client, recent_int);
                            else
                                cout << "Unknown command" << endl;
                        }
                        else if(strcmp(subcommand, "sum") == 0)
                        {
                            cout << "Sort from greatest sum to smallest (type greatest, else smallest):" << endl;

                            char greatest[100];
                            int greatest_int;

                            greatest = cin_fail_int();

                            if(strcmp(greatest, "smallest") == 0)
                            {
                                cout << "Sorting from greatest to smallest" << endl << endl;
                                greatest_int = 0;
                            }
                            else if(strcmp(greatest, "greatest") == 0)
                            {
                                cout << "Sorting from smallest to greatest" << endl << endl;
                                greatest_int = 1;
                            }
                            else
                                greatest_int = -1;

                            if(greatest_int != -1)
                                sort_transactions_sum(selected_client, greatest_int);
                            else
                                cout << "Unknown command" << endl;
                        }
                        else
                            unknown_command_err();
                    }
                }
                else if(strcmp(command, "deposit") == 0)
                {
                    int sum;
                    cout << "How much do you want to deposit: ";
                    sum = cin_fail_int();
                    if(sum != -1)
                    {
                        selected_client -> balance += sum;
                        cout << endl << "Current balance: " << selected_client -> balance << endl;
                    }
                }
                else if(strcmp(command, "transfer") == 0)
                {
                    transfer(clients, selected_client, current_date, length_clients);
                }
                else if(strcmp(command, "logout") == 0)
                {
                    selected_client = nullptr;
                    logged_in = false;
                }
                else
                {
                    unknown_command_err();
                }
            }
        }


    }

    //----
    // Saving in SAVE_FILE_NAME
    char save_content[10000] = "";
    for(int i = 0; i < length_clients; i++)
    {
        client_to_text(&clients[i], save_content);
    }

    ofstream fout(SAVE_FILE_NAME);
    if (!fout.is_open()) {
        cout << "Error: Could not open the file!" << endl;
        return 1;
    }
    fout << save_content;
    fout.close();
    cout << "File saved successfully!" << endl;
    return 0;
}
