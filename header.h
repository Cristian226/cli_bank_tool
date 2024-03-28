#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses/ncurses.h>
#include <time.h>
#include <ctype.h>

typedef struct{
    char name[20];
    void (*f)(); // Functie asociata optiunii
} menuChoices;

typedef struct{
    char IBAN[15];
    char lastName[15];
    char name[15];
    char moneda[4];
    int valuta;
} account;

typedef struct{
    char name[15];
    char lastName[15];
    account curUserAccounts[5];
} user;

//user currentUser;

void menu(menuChoices choices[], int choicesCount, WINDOW* win,
          int *highlightedOut);
void mainMenu();
void login_menu();

void editAcc();
void viewAcc();
void deleteAcc();
void transfer();

void readFile(account* accounts, int *i);
void writeFile(account* accounts, int i);

