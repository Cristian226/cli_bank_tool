#include "header.h"

//gcc -o main main.c mainmenu.c editacc.c transfer.c viewacc.c deleteacc.c -lncurses -DNCURSES_STATIC
// ./main
// METODA GENERARE FISIER:
// IBAN PRENUME NUME MONEDA VALUTA
//  %s %s %s %s %d

user currentUser;
int indexOfAccToEdit;
int ind;
 
void newAcc(){
    WINDOW * win = newwin(12, 60, 0, 0);
    box(win, 0, 0);
    echo();
    mvwprintw(win, 0, 27, "NEW ACCOUNT");
    mvwprintw(win, 10, 1, "Press Control+C to exit");

    account temp;
    strcpy(temp.IBAN,"");

    // scriere nume , prenume si moneda
    mvwprintw(win,1,1,"Enter first name: ");
    wgetstr(win,temp.name);
    //mvwprintw(win,6,1,"%s",temp.name);
    mvwprintw(win,2,1,"Enter last name: ");
    wgetstr(win,temp.lastName);
    //mvwprintw(win,7,1,"%s", temp.lastName);
    for(int i=0; i<=strlen(temp.name); i++)  /// transformare nume, prenume: prima litera -> mare, restul -> mici
        temp.name[i]= tolower(temp.name[i]);
    temp.name[0]=toupper(temp.name[0]);
    for(int i=0; i<=strlen(temp.lastName); i++)
        temp.lastName[i]= tolower(temp.lastName[i]);
    temp.lastName[0]=toupper(temp.lastName[0]);

    // scriere moneda
    do {
        mvwprintw(win,3,1,"Enter currency: ");
        wgetstr(win,temp.moneda);
        //mvwprintw(win,8,1,"%s",temp.moneda);
        mvwprintw(win,3,1,"                                   ");
        mvwprintw(win,9,1,"Enter a valid currency (USD, EUR, RON)");
        for(int i=0;i<strlen(temp.moneda);i++)
            temp.moneda[i] = toupper(temp.moneda[i]);
    } while(strcmp(temp.moneda, "USD")!=0 &&    // verif moneda e valida
            strcmp(temp.moneda, "EUR")!=0 &&
            strcmp(temp.moneda, "RON")!=0 );

    // generare iban + valuta
    strcpy(temp.IBAN, temp.moneda);
    strcat(temp.IBAN, "00");
    srand ( time(NULL) );
    for (int i = strlen(temp.IBAN); i < 14; i++) {
        temp.IBAN[i] = '0' + (rand() % 10);
    }
    temp.valuta = 0;

    strcpy(currentUser.name, temp.name);   // set current user
    strcpy(currentUser.lastName, temp.lastName);
    mvwprintw(win, 9, 1, "                                                       ");
    mvwprintw(win, 8, 1, "Iban generat: %s",temp.IBAN);
    mvwprintw(win, 9, 1, "Welcome %s %s", currentUser.name, currentUser.lastName);

    // scriere in fisier
    account accounts[100];
    int n = 0;
    readFile(accounts, &n);
    accounts[n]=temp;
    n++;
    writeFile(accounts, n);

    wrefresh(win);
    wgetch(win);
    mainMenu();   
}

void loginAcc(){
    WINDOW * win = newwin(12, 60 , 0  , 0);
    box(win,0,0);
    echo(); // sa fie vizibil in consola ce scriem
    mvwprintw(win, 0,27, "LOGIN");
    mvwprintw(win, 10,1, "Press Control+C to exit");

    account temp;
    strcpy(temp.IBAN,"");

    // scriere nume , prenume si moneda
    mvwprintw(win,1,1,"Enter first name: ");
    wgetstr(win,temp.name);
    //mvwprintw(win,6,1,"%s",temp.name);
    mvwprintw(win,2,1,"Enter last name: ");
    wgetstr(win,temp.lastName);
    //mvwprintw(win,7,1,"%s", temp.lastName);
    for(int i=0; i<=strlen(temp.name); i++)  /// transformare nume, prenume: prima litera -> mare, restul -> mici
        temp.name[i]= tolower(temp.name[i]);
    temp.name[0]=toupper(temp.name[0]);
    for(int i=0; i<=strlen(temp.lastName); i++)
        temp.lastName[i]= tolower(temp.lastName[i]);
    temp.lastName[0]=toupper(temp.lastName[0]);

    int isInFile=0;
    account accounts[100];   // citire conturi din fisier
    int n=0;
    readFile(accounts, &n);
    for(int i=0; i<n; i++)
        if( !strcmp(temp.name, accounts[i].name) && !strcmp(temp.lastName, accounts[i].lastName) )
            isInFile = 1;
    
    if (isInFile){
        strcpy(currentUser.name, temp.name);
        strcpy(currentUser.lastName, temp.lastName);
        mvwprintw(win, 9,1, "Welcome %s %s", currentUser.name, currentUser.lastName);
        wgetch(win);
        mainMenu();
    } else {
        mvwprintw(win, 9,1, "Account not found");
        wgetch(win);
        login_menu();
    }
}

void login_menu(){
    WINDOW * win = newwin(12, 60 , 0  , 0);
    box(win,0,0);
    mvwprintw(win, 0,27, "LOGIN MENU");
    mvwprintw(win, 5,1, "----------------------------------------------------------");
    mvwprintw(win, 6, 1, "TIPS:");
    mvwprintw(win, 7,1, "Press ENTER to continue");
    mvwprintw(win, 8,1, "Use up/down arrow to navigate");
    mvwprintw(win, 9,1, "----------------------------------------------------------");
    mvwprintw(win, 10,1, "Press Control+C to exit");

    menuChoices choices[2]= {{"New account", newAcc}, {"Login", loginAcc}};
    menu(choices, 2, win, NULL);
    refresh();
    wrefresh(win);
}

int main(int argc, char** argv){

    initscr();

    // verificarre argv
    if(argv[1] ){
        account accounts[100];
        FILE *f = fopen("accounts.txt", "r");
        if (f == NULL)
            login_menu();
        int i=0;
        while (fscanf(f, "%s %s %s %s %d", 
            &accounts[i].IBAN, 
            &accounts[i].name, 
            &accounts[i].lastName, 
            &accounts[i].moneda, 
            &accounts[i].valuta ) == 5){
                // verificam daca este cont al current user
                if( !strcmp(argv[1], accounts[i].name) && 
                    !strcmp(argv[2], accounts[i].lastName)){
                            strcpy(currentUser.name, argv[1]);
                            strcpy(currentUser.lastName, argv[2]);
                            mainMenu();
                }
                i++;
        }
        login_menu();
    }
    login_menu();
}
