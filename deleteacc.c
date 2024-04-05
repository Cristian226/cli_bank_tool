#include "header.h"
extern user currentUser;
extern int indexOfAccToEdit;
extern int ind; // nr of user accounts


void delete(){
    WINDOW * win = newwin(12, 60 , 0  , 0);
    box(win,0,0);
    echo();
    mvwprintw(win, 0,20, "DELETE ACCOUNT?");
    mvwprintw(win, 1,1, "Deleting your account is permanent !!!");
    mvwprintw(win,9,1,"Press any other button to return");
    mvwprintw(win, 10,1, "Press Control+C to exit");
    mvwprintw(win, 2,1, "Press 'Y' to confirm: ");
    char confirm = wgetch(win);

    if(confirm=='Y' || confirm == 'y'){
        account accounts[100];   // citire conturi din fisier
        int n=0;
        readFile(accounts, &n);

        FILE *f = fopen("accounts.txt", "w");  // stergere conturi
        for(int i=0; i<n; i++){
            if(!strcmp( currentUser.curUserAccounts[indexOfAccToEdit].IBAN, accounts[i].IBAN) && 
               !strcmp( currentUser.curUserAccounts[indexOfAccToEdit].IBAN, accounts[i].IBAN))
                continue;
            fprintf(f, "%s %s %s %s %d\n", accounts[i].IBAN, accounts[i].name, accounts[i].lastName, accounts[i].moneda, accounts[i].valuta );
        } 
        fclose(f);
        wrefresh(win);
        wgetch(win);
        if(ind<=1)
            login_menu();
        else mainMenu();
    }
    else 
        mainMenu();

}

void deleteAcc(){
    extern user currentUser;
    WINDOW * win = newwin(12, 60 , 0  , 0);
    box(win,0,0);
    mvwprintw(win, 0, 5, "SELECT AN ACCOUNT");

    ind=0;
    account nonUserAccounts[100];
    int ind2=0;
    account accounts[100];   // citire conturi din fisier
    int i=0;
    menuChoices choices[10];
    FILE *f = fopen("accounts.txt", "r");
    if (f == NULL)
        return;
    while (fscanf(f, "%s %s %s %s %d", &accounts[i].IBAN, &accounts[i].name, &accounts[i].lastName, 
                        &accounts[i].moneda, &accounts[i].valuta ) == 5){
            // verificam daca este cont al current user
            if( !strcmp( currentUser.name, accounts[i].name) && !strcmp(currentUser.lastName, accounts[i].lastName)){
                currentUser.curUserAccounts[ind]= accounts[i];
                char temp[100];
                strcpy(temp, accounts[i].IBAN); // trecere iban, moneda si valtua in meniul cu optiuni
                strcat(temp, " ");
                strcat(temp, accounts[i].moneda);
                strcat(temp, " ");
                char tempValuta[10], tempValuta2[10];
                int ind3=0;
                int valcopy=accounts[i].valuta;
                while(valcopy){
                    tempValuta[ind3++]= '0' + (valcopy%10);
                    valcopy/=10;
                }
                for(int i=0; i< ind3; i++)
                    tempValuta2[ind3-i-1]=tempValuta[i];
                if(ind3==0)
                    strcpy(tempValuta2, "0");
                strcat(temp, tempValuta2);
                strcpy(choices[ind].name, temp);
                choices[ind++].f = delete;
            }
            else 
                nonUserAccounts[ind2++] = accounts[i];
            i++;

    } 
    fclose(f);
    menu(choices, ind, win, &indexOfAccToEdit);
}