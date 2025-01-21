# Primite Banking System

---

## Basic commands/menus

### Primary Menu

1. Login (login) - Requires your client id;
2. Create account (create) - Requires name and surname;
3. Show all clients (show) - Prints all the client data;
4. Exit (exit) - Quit the program, global command;

After logging in, you are prompted into the secondary menu

### Secondary Menu

1. Show data (show) - Prompts you into the *Show Data Menu*;
2. Sort transactions (sort) - Sorts transactions *TODO*;
3. Search transaction (search) - Prompts you into another menu (search by receiver's id, sum of money transfered, as well as the date);
4. Send money (transfer) - Requires receiver id and sum;
5. Deposit (deposit) - Requires the amount you want to deposit into your account;
6. Logout (logout) - Logs you out of your account, prompts you back into the *Primary Menu*;

### Show Data Menu

1. Name (name) - Prints the name of the account;
2. Surname (surname) - Prints the surname of the account;
3. Balance (balance) - Prints account balance;
4. ID (id) - Prints account id;
5. Transactions (transactions) - Prints account transactions;
6. All (a) - Prints everything;


### Search Transaction
Looks for the specified criterium and prints all the matched transfers

1. Reciever's ID
2. Emitter's ID
2. Date
3. Sum

<br>
---
<br>
*TODO: Sort transactions;* <br>
*TODO: Check input error conversion (currently some don't work because of "isdigit()" function;*<br>
*TODO: Sorting accounts by multiple parameters;*