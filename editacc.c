#include "header.h"
account accToEdit;
int indexOfAccToEdit;
int ind;

void editIBAN(){
    extern user currentUser;
    WINDOW * win = newwin(12, 60 , 0  , 0);
    box(win,0,0);
    echo();
    mvwprintw(win,0,10, "EDIT IBAN");
    mvwprintw(win, 1,1, "Current IBAN: %s", currentUser.curUserAccounts[indexOfAccToEdit].IBAN);
    mvwprintw(win,2,1,"Insert new IBAN: ");

    char temp[25];
    wgetstr(win,temp);
    strcpy(currentUser.curUserAccounts[indexOfAccToEdit].IBAN, temp);
    noecho();
    account accounts[100];
    int n=0;
    readFile(accounts, &n);
    FILE *g = fopen("accounts.txt", "w");
    for(int i=0; i<n; i++)
        fprintf(g, "%s %s %s %s %d\n", accounts[i].IBAN, accounts[i].name, accounts[i].lastName, accounts[i].moneda, accounts[i].valuta ); 
    for(int i=0; i<ind; i++)
        fprintf(g, "%s %s %s %s %d\n", currentUser.curUserAccounts[i].IBAN, currentUser.curUserAccounts[i].name,
                currentUser.curUserAccounts[i].lastName,currentUser.curUserAccounts[i].moneda, currentUser.curUserAccounts[i].valuta );
    fclose(g);
    wrefresh(win);
    mainMenu();
    
}

void editCurrency(){
    extern user currentUser;
    WINDOW * win = newwin(12, 60 , 0  , 0);
    box(win,0,0);
    echo();
    mvwprintw(win,0,10, "EDIT CURRENCY TYPE");
    mvwprintw(win, 1,1, "Current currency type: %s", currentUser.curUserAccounts[indexOfAccToEdit].moneda);
    char oldmoneda[4];
    strcpy(oldmoneda, currentUser.curUserAccounts[indexOfAccToEdit].moneda);
    do{
    mvwprintw(win,2,1,"Enter new currency type: ");
    wgetstr(win,currentUser.curUserAccounts[indexOfAccToEdit].moneda);
    mvwprintw(win,2,1,"                                   ");
    mvwprintw(win,9,1,"Enter a valid currency type (USD, EUR, RON)");
    for(int i=0;i<strlen(currentUser.curUserAccounts[indexOfAccToEdit].moneda);i++)
        currentUser.curUserAccounts[indexOfAccToEdit].moneda[i] = toupper(currentUser.curUserAccounts[indexOfAccToEdit].moneda[i]);
    } while(strcmp(currentUser.curUserAccounts[indexOfAccToEdit].moneda, "USD")!=0 &&    // verif moneda e valida
            strcmp(currentUser.curUserAccounts[indexOfAccToEdit].moneda, "EUR")!=0 &&
            strcmp(currentUser.curUserAccounts[indexOfAccToEdit].moneda, "RON")!=0 );

    // conversie valuta
    if(!strcmp(oldmoneda, "RON")){
        if(strcmp(currentUser.curUserAccounts[indexOfAccToEdit].moneda, "RON"))
            currentUser.curUserAccounts[indexOfAccToEdit].valuta /= 5;
    }
    if(!strcmp(oldmoneda, "USD")){
        if(!strcmp(currentUser.curUserAccounts[indexOfAccToEdit].moneda, "RON"))
            currentUser.curUserAccounts[indexOfAccToEdit].valuta *= 5;
    }
    if(!strcmp(oldmoneda, "EUR")){
        if(!strcmp(currentUser.curUserAccounts[indexOfAccToEdit].moneda, "RON"))
            currentUser.curUserAccounts[indexOfAccToEdit].valuta *= 5;
    }
    noecho();
    account accounts[100];
    int n=0;
    readFile(accounts, &n);
    FILE *g = fopen("accounts.txt", "w");
    for(int i=0; i<n; i++)
        fprintf(g, "%s %s %s %s %d\n", accounts[i].IBAN, accounts[i].name, accounts[i].lastName, accounts[i].moneda, accounts[i].valuta ); 
    for(int i=0; i<ind; i++)
        fprintf(g, "%s %s %s %s %d\n", currentUser.curUserAccounts[i].IBAN, currentUser.curUserAccounts[i].name,
                currentUser.curUserAccounts[i].lastName,currentUser.curUserAccounts[i].moneda, currentUser.curUserAccounts[i].valuta );
    fclose(g);
    wrefresh(win);
    mainMenu();
}

void editBalance(){
    extern user currentUser;
    WINDOW * win = newwin(12, 60 , 0  , 0);
    box(win,0,0);
    echo();
    mvwprintw(win,0,10, "EDIT CURRENCY");
    mvwprintw(win, 1,1, "Current currency: %d", currentUser.curUserAccounts[indexOfAccToEdit].valuta);
    mvwprintw(win,2,1,"Insert new currency: ");

    char tempstr[15];
    int tempint=0;
    wgetstr(win, tempstr);
    for(int i=0; i< strlen(tempstr); i++){
        tempint*=10;
        tempint+= tempstr[i] - '0';
    }
    currentUser.curUserAccounts[indexOfAccToEdit].valuta = tempint;

    noecho();
    account accounts[100];
    int n=0;
    readFile(accounts, &n);
    FILE *g = fopen("accounts.txt", "w");
    for(int i=0; i<n; i++)
        fprintf(g, "%s %s %s %s %d\n", accounts[i].IBAN, accounts[i].name, accounts[i].lastName, accounts[i].moneda, accounts[i].valuta ); 
    for(int i=0; i<ind; i++)
        fprintf(g, "%s %s %s %s %d\n", currentUser.curUserAccounts[i].IBAN, currentUser.curUserAccounts[i].name,
                currentUser.curUserAccounts[i].lastName,currentUser.curUserAccounts[i].moneda, currentUser.curUserAccounts[i].valuta );
    fclose(g);
    wrefresh(win);
    mainMenu();
}

void selectWhatToEdit(){
    extern user currentUser;
    WINDOW * win = newwin(12, 60 , 0  , 0);
    box(win,0,0);
    mvwprintw(win, 0,5, "SELECT WHAT TO EDIT");

    menuChoices choices[3]= {{"Edit IBAN", editIBAN},
                             {"Edit balance", editBalance},
                             {"Edit currency", editCurrency }};
    menu(choices, 3, win, NULL);    
}

void editAcc(){
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
                choices[ind++].f = selectWhatToEdit;
            }
            else 
                nonUserAccounts[ind2++] = accounts[i];
            i++;

    } 
    fclose(f);

    FILE *g = fopen("accounts.txt", "w");
    for(int i=0; i<ind2; i++)
        fprintf(g, "%s %s %s %s %d\n", nonUserAccounts[i].IBAN, nonUserAccounts[i].name, nonUserAccounts[i].lastName,
                                     nonUserAccounts[i].moneda, nonUserAccounts[i].valuta );
    fclose(g);
    
    menu(choices, ind, win, &indexOfAccToEdit);
         
}