Sure! Here's a version with simpler language:

---

# Banking System - C++ Project

## Overview

This is a simple banking system written in C++. It allows users to create accounts, make transactions, deposit money, and transfer money between accounts. The system also lets users view account details, search and sort transactions, and manage funds.

## Main Features

- **Account Management**:
  - Create a new account with a unique ID, first name, and last name.
  - Log in with your account ID to see your account details.
  - View your balance and transaction history.

- **Transaction Management**:
  - Transfer money to other users.
  - Deposit money into your account.
  - Sort and search transactions by date, amount, sender, or receiver.

- **File Management**:
  - Account and transaction data are saved in a file called `utilizatori.in`.
  - The program reads from and writes to this file.

## Core Structures

### `struct date`
Stores the date of a transaction:
```cpp
struct date {
    int day, month, year;
};
```

### `struct transaction`
Represents a transaction, including the sender’s ID, receiver’s ID, amount, and transaction date:
```cpp
struct transaction {
    int emitter_id;
    int receiver_id;
    int sum;
    date transaction_date;
};
```

### `struct client`
Holds a client’s personal details and transaction information:
```cpp
struct client {
    char name[99];
    char surname[99];
    int _id;
    float balance;
    transaction transactions[100];
    int length_transactions;
};
```

## Main Functions

### 1. **Account and Transaction Management**

- **`create_account()`**:
    - Creates a new account with a unique ID and a starting balance of 0.
    - Asks for the user’s name and surname.

- **`login()`**:
    - Lets a user log in with their unique ID.
    - Checks if the ID exists in the system, then shows the user’s account details.

- **`transfer()`**:
    - Lets a user send money to another user.
    - Asks for the receiver’s ID and the amount to send, checks if everything is valid, then updates both accounts.

- **`deposit()`**:
    - Lets a user deposit money into their account.
    - Asks for the amount to deposit and adds it to the user’s balance.

### 2. **Transaction Sorting and Searching**

- **`sort_transactions_menu()`**:
    - Lets the user sort transactions by:
      - **Date**: Sort by when the transaction happened.
      - **Amount**: Sort by the money transferred.

- **`search_transactions_menu()`**:
    - Lets the user search for transactions by:
      - **Receiver ID**: Find transactions where a specific person received money.
      - **Emitter ID**: Find transactions made by a specific person.
      - **Transaction Amount**: Find transactions with a specific amount.
      - **Date**: Find transactions on a specific date.

### 3. **Helper Functions**

- **`is_file_empty()`**:
    - Checks if the file storing user data is empty.
    - Returns `true` if the file is empty, `false` if not.

- **`except_error()`**:
    - Displays an error message and stops the program if something goes wrong.
    - Takes a custom message and an exit code as inputs.

- **`float_to_char()`**:
    - Converts a decimal number into a string.
    - Works with both whole numbers and decimals.

## File Format

The user information and transaction data are saved in the file `utilizatori.in` in the following format:

1. **User Information**:
    - **Name**: The user’s first name.
    - **Surname**: The user’s last name.
    - **ID**: A unique ID for each user.
    - **Balance**: The user’s current account balance.

2. **Transactions** (if any):
    - Each transaction includes:
        - **Emitter ID**: The user sending the money.
        - **Receiver ID**: The user receiving the money.
        - **Amount**: The amount of money sent.
        - **Date**: The date of the transaction in the format `day/month/year`.

### Example Format:
```
Moraru Matei 1001 150.00 | 1001 1002 350.00 22 1 2023 ; 1003 1001 50.00 23 1 2023 }
```
