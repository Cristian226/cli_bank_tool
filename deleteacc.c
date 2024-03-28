#include "header.h"
extern user currentUser;


void deleteAcc(){
    WINDOW * win = newwin(12, 60 , 0  , 0);
    box(win,0,0);
    echo();
    mvwprintw(win, 0,20, "DELETE ACCOUNT?");
    mvwprintw(win, 1,1, "Deleting your account is permanent !!!");
    mvwprintw(win, 2,1, "Press 'Y' to confirm: ");
    mvwprintw(win,9,1,"Press any other button to return");
    mvwprintw(win, 10,1, "Press Control+C to exit");
    char confirm = wgetch(win);

    if(confirm=='Y'){
        account accounts[100];   // citire conturi din fisier
        int n=0;
        readFile(accounts, &n);

        FILE *f = fopen("accounts.txt", "w");  // stergere conturi
        for(int i=0; i<n; i++){
            if(!strcmp( currentUser.name, accounts[i].name) && !strcmp(currentUser.lastName, accounts[i].lastName))
                continue;
            fprintf(f, "%s %s %s %s %d\n", accounts[i].IBAN, accounts[i].name, accounts[i].lastName, accounts[i].moneda, accounts[i].valuta );
        } 
        fclose(f);
        wrefresh(win);
        wgetch(win);
        login_menu();
    }
    else 
        mainMenu();

}