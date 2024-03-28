#include "header.h"
extern user currentUser;


void viewAcc(){
    WINDOW * win = newwin(12, 60 , 0  , 0);
    box(win,0,0);
    noecho();
    mvwprintw(win, 0,5, "%s %s", currentUser.name, currentUser.lastName);
    int poz=2;
    mvwprintw(win, 1,1, "Your accounts:");
    mvwprintw(win,9,1,"Press any button to return");
    mvwprintw(win, 10,1, "Press Control+C to exit");

    account accounts[100];   // citire conturi din fisier
    int i=0;
    FILE *f = fopen("accounts.txt", "r");
    if (f == NULL)
        return;
    while (fscanf(f, "%s %s %s %s %d", &accounts[i].IBAN, &accounts[i].name, &accounts[i].lastName, 
                        &accounts[i].moneda, &accounts[i].valuta ) == 5){
            
            if( !strcmp( currentUser.name, accounts[i].name) && !strcmp(currentUser.lastName, accounts[i].lastName))
                mvwprintw(win,poz++,1, "%s %s %d", accounts[i].IBAN, accounts[i].moneda, accounts[i].valuta );
            i++;

    } 
    fclose(f);
    
    wrefresh(win);
    wgetch(win);
    mainMenu();

}