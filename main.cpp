#include <iostream>
#include <ctime>
#include <chrono>
#include <cstring>
#include <fstream>
#include <cstdlib>
using namespace std;

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

void except_error(char *message, int code)
{
    cout << "\t" << message << "!" << endl;
    exit(code);
}

bool is_char_in_array(char* arr, char target) {
    for (int i = 0; arr[i] != '\0'; i++) {
        if (arr[i] == target) {
            return true;
        }
    }
    return false;
}

float str_to_float(char *str)
{
    float result = 0.0f;
    float decimal_mult = 0.1f;
    bool is_negative = false;
    bool is_decimal = false;

    size_t i = 0;

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


int citire_utilizatori(client *clients)
{
    int length_clients = 0;
    ifstream fin("utilizatori.in");
    if(!fin)
        except_error("Invalid name for client database. FileIO Exception", -1);


    client current_client;
    current_client.length_transactions = 0;
    transaction current_transaction;
    date transaction_date;

    char str[9999];

    while(fin >> str)
    {
        // Basic user data
        strcpy(current_client.name, str);
        fin >> str;
        strcpy(current_client.surname, str);
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

        //print_client_data(current_client);

        clients[length_clients] = current_client;
        length_clients++;
        memset(&current_client, 0, sizeof(current_client)); // Resetting the user data (for transactions especially)

    }

    cout << endl;
    return length_clients;
}

void first_menu()
{
    cout << endl;
    cout << "Login into account (login)" << endl;
    cout << "Create account (create)" << endl;
    cout << "Show accounts (show)" << endl;
    cout << "exit program (EXIT)" << endl << endl;
}

//TODO: transactions
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

client* login(client clients[], int length_clients)
{
    int id;
    cout << "Input your id: ";
    cin >> id;

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

    return user;
}

void transfer(client *clients, client *selected_client, date current_date)
{
    int emitter = selected_client -> _id;
    int receiver;
    int sum;
    cout << "\tTransfer money" << endl;
    cout << "To whom do you want to send money to? ";
    cin >> receiver;
    while(receiver == emitter || receiver <= 1000)
    {
        cout << "You must input a valid id! (id>1000, can't be your own account)" << endl << "Input receiver id: ";
        cin >> receiver;
        cout << endl;
    }
    cout << endl << "How much do you want to send? ";
    cin >> sum;

    while(sum > selected_client -> balance)
    {
        cout << "Not enough funds! Input a smaller amount!" << endl;
        cin >> sum;
        cout << endl;
    }

    while(sum <= 0)
    {
        cout << "Sum must be greater than 0; Try again: ";
        cin >> sum;
        cout << endl;
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

// Search transfers

// If there are multiple transfers that match the criterium, show all of them
// Do not save them, just print them numbered

void search_transfer_receiver_id(client *current_client, int receiver_id)
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

void search_transfer_emitter_id(int emitter_id)
{

}

void search_transfer_date(int day, int month, int year)
{

}

/*
    TODO: Sort transactions
    TODO: Search transaction (menu)
    TODO: Saving client data into utilizatori.out
    TODO: cerr if not found fin
*/
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
    length_clients = citire_utilizatori(clients);
    //print_client_data(clients[0]);
    cout << "---ADMINISTRAREA UNEI APLICATII BANCARE---" << endl;
    cout << "Date: " << current_date.day << '/' << current_date.month << '/' << current_date.year << endl;

    cout << "Pentru a folosi programul, tasteaza comanda din paranteza!" << endl;
    cout << "Pentru a sterge ecranul, tasteaza \"clear\"" << endl;

    char command[99];

    while(strcmp(command, "EXIT") != 0)
    {
        if(!logged_in)
            first_menu();
        else
            secondary_menu();
        cout << "Input: ";
        cin >> command;
        cout << endl;
        if(strcmp(command, "clear") == 0)
            system("cls");
        else
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
                    cin >> command;
                    if(strcmp(command, "a") == 0)
                        print_client_data(*selected_client);
                    else if(strcmp(command, "name") == 0)
                        cout << "\tName: " << selected_client -> name << endl;
                    else if(strcmp(command, "surname") == 0)
                        cout << "\tSurame: " << selected_client -> surname << endl;
                    else if(strcmp(command, "balance") == 0)
                        cout << "\tBalance: " << selected_client -> balance << endl;
                    else if(strcmp(command, "id") == 0)
                        cout << "\tID: " << selected_client -> _id << endl;
                    else if(strcmp(command, "transactions") == 0)
                        print_client_transactions(*selected_client);
                    else
                        unknown_command_err();

                }
                else if(strcmp(command, "search") == 0)
                {
                    cout << "Cautati dupa (id) ";
                    int id;
                    cin >> id;
                    cout << endl;
                    search_transfer_receiver_id(selected_client, id);
                }
                else if(strcmp(command, "deposit") == 0)
                {
                    int sum;
                    cout << "How much do you want to deposit: ";
                    cin >> sum;
                    selected_client -> balance += sum;
                    cout << endl << "Current balance: " << selected_client -> balance << endl;
                }
                else if(strcmp(command, "transfer") == 0)
                {
                    transfer(clients, selected_client, current_date);
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


    return 0;
}
