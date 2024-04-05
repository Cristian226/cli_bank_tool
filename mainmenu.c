#include "header.h"


void readFile(account accounts[100], int *i){
    FILE *f = fopen("accounts.txt", "r");
    if (f == NULL)
        return;
    while (fscanf(f, "%s %s %s %s %d", 
                  &accounts[*i].IBAN, 
                  &accounts[*i].name,
                  &accounts[*i].lastName, 
                  &accounts[*i].moneda,
                  &accounts[*i].valuta ) == 5) 
        (*i)++;
    fclose(f);
}

void writeFile(account* accounts, int n){
    FILE *f = fopen("accounts.txt", "w");
    for(int i=0; i<n; i++)
        fprintf(f, "%s %s %s %s %d\n",
                accounts[i].IBAN, accounts[i].name, accounts[i].lastName,
                accounts[i].moneda, accounts[i].valuta); 
    fclose(f);
}

// functia folosita pentru crearea meniurilor, primeste un struct cu ce sa
// scrie si ce sa apeleze la fiecare optiune(linie) 
void menu(menuChoices choices[], int choicesCount, WINDOW* win,
          int *highlightedOut){
    noecho(); // ascunde stdin input
    curs_set(0); // ascunde cursorul
    keypad(win,true); // permite utilizarea sagetilor 
    int choice = 0;
    int highlight = 0;
    while (true){
        for (int i=0; i<choicesCount; i++){
            if (i == highlight) 
                wattron(win, A_REVERSE);
            mvwprintw(win, i+1, 1, choices[i].name);
            wattroff(win, A_REVERSE);   
        }
        choice = wgetch(win);
        switch(choice){
            case KEY_UP:
                highlight--;
                if(highlight == -1)
                    highlight=choicesCount-1;
                break;
            case KEY_DOWN:
                highlight++;
                if(highlight == choicesCount)
                    highlight = 0;
                break;
            default: 
                break;
        }
        if(choice == 10)
            break;
    }
    if (highlightedOut) {
        *highlightedOut=highlight;
    }
    choices[highlight].f();

    refresh();
    wrefresh(win);
    getch();
    endwin();
}

void mainMenu(){
    WINDOW * win = newwin(12, 60 , 0  , 0);
    box(win,0,0);
    mvwprintw(win, 0,27, "MAIN MENU");
    mvwprintw(win, 10,1, "Press Control+C to exit");
    
    menuChoices choices[5]= {{"Edit account", editAcc},
                             {"Delete account", deleteAcc},
                             {"View account", viewAcc },
                             {"Transactions", transfer},
                             {"Change account", login_menu}};
    menu(choices, 5, win, NULL);
    
}