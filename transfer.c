#include "header.h"
extern account accToEdit;
extern int indexOfAccToEdit;
extern int ind;

void transfer2(){
    extern user currentUser;
    WINDOW * win = newwin(12, 60 , 0  , 0);
    box(win,0,0);
    echo();
    mvwprintw(win, 0,5, "TRANSFER");
    mvwprintw(win,1,1,"Ammount you want to transfer: ");
    
    char tempstr[15];
    int tempint=0;
    wgetstr(win, tempstr);
    for(int i=0; i< strlen(tempstr); i++){
        tempint*=10;
        tempint+= tempstr[i] - '0';
    }
    if(tempint > currentUser.curUserAccounts[indexOfAccToEdit].valuta){
        refresh();  // nu stiu dc dar imi dispare meniul in functia asta, asa ca il fac iar
        WINDOW * win = newwin(12, 60 , 0  , 0);
        box(win,0,0);
        echo();
        mvwprintw(win, 0,5, "TRANSFER");
        mvwprintw(win,1,1,"Ammount you want to transfer: %d", tempint);
        
        mvwprintw(win,9,1,"Not enough funds");
        wrefresh(win);
        getch();
        mainMenu();
    }
    
    char tempIBAN[20];
    mvwprintw(win,2,1,"IBAN: ");
    wgetstr(win, tempIBAN);

    int found=0;
    account accounts[100];
    int n=0;
    readFile(accounts, &n);
    for(int i=0; i<n; i++)
        if( !strcmp(tempIBAN, accounts[i].IBAN) ) { // cautare IBAN
            // conversie valuta
            found=1;
            if(!strcmp(currentUser.curUserAccounts[indexOfAccToEdit].moneda, "RON")){
                if(strcmp(accounts[i].moneda, "RON")){
                    accounts[i].valuta += tempint/5;
                    currentUser.curUserAccounts[indexOfAccToEdit].valuta-= tempint;
                }
                else{
                    accounts[i].valuta += tempint;
                    currentUser.curUserAccounts[indexOfAccToEdit].valuta-= tempint;
                }
            }
            if(!strcmp(currentUser.curUserAccounts[indexOfAccToEdit].moneda, "USD")){
                if(!strcmp(accounts[i].moneda, "RON")){
                    accounts[i].valuta += tempint*5;
                    currentUser.curUserAccounts[indexOfAccToEdit].valuta-= tempint;
                }
                else{
                    accounts[i].valuta += tempint;
                    currentUser.curUserAccounts[indexOfAccToEdit].valuta-= tempint;
                }
            }
            if(!strcmp(currentUser.curUserAccounts[indexOfAccToEdit].moneda, "EUR")){
                if(!strcmp(accounts[i].moneda, "RON")){
                    accounts[i].valuta += tempint*5;
                    currentUser.curUserAccounts[indexOfAccToEdit].valuta-= tempint;
                }
                else{
                    accounts[i].valuta += tempint;
                    currentUser.curUserAccounts[indexOfAccToEdit].valuta-= tempint;
                }
            }
        }
    
    for(int i=0; i<n;i++)
        if(!strcmp(accounts[i].IBAN, currentUser.curUserAccounts[indexOfAccToEdit].IBAN))
            accounts[i].valuta = currentUser.curUserAccounts[indexOfAccToEdit].valuta;
    FILE *g = fopen("accounts.txt", "w");
    for(int i=0; i<n; i++)
        fprintf(g, "%s %s %s %s %d\n", accounts[i].IBAN, accounts[i].name, accounts[i].lastName, accounts[i].moneda, accounts[i].valuta ); 
    fclose(g);

    if(found==0){
        mvwprintw(win,9,1,"IBAN not found");
        wrefresh(win);
        getch();
        mainMenu();
    }
    wrefresh(win);
    mainMenu();
}

void transfer(){
    extern user currentUser;
    WINDOW * win = newwin(12, 60 , 0  , 0);
    box(win,0,0);
    mvwprintw(win, 0,5, "SELECT AN ACCOUNT");

    ind=0;
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
                choices[ind++].f = transfer2;
            }
            i++;

    } 
    fclose(f);
    
    menu(choices, ind, win, &indexOfAccToEdit);
}