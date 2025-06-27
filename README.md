# Bank Management System (C++)

This is a console-based **Bank Management System** built in C++ that allows users to perform standard banking operations like creating accounts, depositing/withdrawing money, modifying or deleting records, and displaying account information — all backed by binary file handling for persistent storage.

---

## Features

- Create **Savings** or **Current** accounts
- Enforce **minimum balance requirements** (₹500 for Savings, ₹1000 for Current)
- Deposit and withdraw amounts with proper validations
- Modify account details
- Delete accounts safely
- View details of specific or all accounts
- Persistent data storage using **binary files**
- Input validation and user-friendly prompts

---

## Account Rules

| Account Type | Minimum Balance |
|--------------|------------------|
| Savings (S)  | ₹500             |
| Current (C)  | ₹1000            |

Withdrawals are only allowed if they do not reduce the balance below the required minimum.

---

## Tech Stack

- **Language:** C++
- **File Handling:** `fstream`, binary I/O
- **Interface:** Command-line menu-driven

---

## File Structure

Bank_Management_System_Modified.cpp
account.dat   # Binary file auto-generated to store account data

---
