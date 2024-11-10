#include<stdio.h>
#include <Windows.h>
#include <unistd.h>
#include <stdbool.h>

char tabletop[4][4] = {
    {'.','A','B','C'},
    {'1','-','-','-'},
    {'2','-','-','-'},
    {'3','-','-','-'}};

void reset_table() {//Resets the table values if the player wants to play again
    for (int i = 1; i < 4; i++) {
        for (int j = 1; j < 4; j++) {
            tabletop[i][j] = '-';
        }
    }
}

void table_print() {//Clear the terminal, print the tabletop and set a 4 sec time sleep
    sleep(1);
    printf("\033"); //Arrow :)
    for (int i = 0; i < 4; i++) {//Print Table
        printf("\n %c %c %c %c", tabletop[i][0],tabletop[i][1],tabletop[i][2],
            tabletop[i][3]);
    }
    printf("\n");
}

int ask_column() {
    char ask;
    int column;
    printf("Enter the column you want to play (A, B or C)\n");
    scanf("%s", &ask);

    while (ask != 'A' && ask != 'B' && ask != 'C' ) {
        printf("Please enter a valid option (A, B or C) \n");
       scanf("%s", &ask);
    }

    switch (ask) {
        case 'A':
            column = 1;
            break;
        case 'B':
            column = 2;
            break;
        case 'C':
            column = 3;
            break;
    }
    return column;
}

int ask_line() {
    char ask;
    int line;
    printf("Enter the line you want to play (1, 2 or 3) \n");
    scanf("%s", &ask);

    while (ask != '1' && ask != '2' && ask != '3' ) {
        printf("Please enter a valid option (1, 2 or 3) \n");
        scanf("%s", &ask);
    }

    switch (ask) {
        case '1':
            line = 1;
            break;
        case '2':
            line = 2;
            break;
        case '3':
            line = 3;
            break;
    }
    return line;
}

void available(int *line, int *column) {//Validates whether the chosen field is available
    while (tabletop[*line][*column] != '-') {
        table_print();
        printf("The selected location has already been chosen, please choose another \n");
        *column = ask_column();
        *line = ask_line();
    }
}

void play_player() { //Moment when the player chooses his move
    int move[2];
    printf("Your turn! \n");
    move[1] = ask_column();
    move[0] = ask_line();
    available(&move[0], &move[1]);
    tabletop[move[0]][move[1]] = 'O';
    table_print();
}

void possibility_victory(int *line, int *column, char player) { //Checks if it is possible for someone to win and return the value that would cause this
    //Check diagonal (There is no need to check if the middle is empty because it will always be filled in the first round)
    if (tabletop[1][1] == player && tabletop[2][2] == player && tabletop[3][3] == '-') {
        *column = 3;
        *line = 3;
    }else if (tabletop[1][1] == '-' && tabletop[2][2] == player && tabletop[3][3] == player) {
        *column = 1;
        *line = 1;
    }else if (tabletop[1][3] == player && tabletop[2][2] == player && tabletop[3][1] == '-') {
        *column = 1;
        *line = 3;
    }else if (tabletop[1][3] == '-' && tabletop[2][2] == player && tabletop[3][1] == player) {
        *column = 3;
        *line = 1;
    }else { //Check horizontal and vertical
        for (int i = 1; i < 4; i++) {
            if (tabletop[i][1] == player && tabletop[i][2] == player && tabletop[i][3] == '-') {
                *column = 3;
                *line = i;
            }else if (tabletop[i][1] == player && tabletop[i][2] == '-' && tabletop[i][3] == player) {
                *column = 2;
                *line = i;
            }else if (tabletop[i][1] == '-' && tabletop[i][2] == player && tabletop[i][3] == player) {
                *column = 1;
                *line = i;
            }else if (tabletop[1][i] == player && tabletop[2][i] == player && tabletop[3][i] == '-') {
                *column = i;
                *line = 3;
            }else if (tabletop[1][i] == player && tabletop[2][i] == '-' && tabletop[3][i] == player) {
                *column = i;
                *line = 2;
            }else if (tabletop[1][i] == '-' && tabletop[2][i] == player && tabletop[3][i] == player) {
                *column = i;
                *line = 1;
            }
        }
    }
}

void random_move (int *line, int *column) {
    //the corners need to be prioritized and the [3][3] is the last one in the for
    if (tabletop[1][1] == '-') {
        *column = 1;
        *line = 1;
    }else if (tabletop[3][1] == '-') {
        *column = 1;
        *line = 3;
    }else if (tabletop[1][3] == '-') {
        *column = 3;
        *line = 1;
    }else {
        for (int i = 1; i < 4; i++) {
            for (int j = 1; j < 4; j++) {
                if (tabletop[i][j] == '-') {
                    *column = j;
                    *line = i;
                }
            }
        }
    }
}

void play_machine() {
    int move[2];
    printf("Machine turn! \n");
    random_move(&move[0], &move[1]);
    possibility_victory(&move[0], &move[1], 'O');//test if the player will win in the next round and block it
    possibility_victory(&move[0], &move[1], 'X');//test if you can win now
    tabletop[move[0]][move[1]] = 'X';
    table_print();
}

bool check_victory(char player) {//checks if someone won and returns True or False
    //Check diagonal
    if (tabletop[1][1] == player && tabletop[2][2] == player && tabletop[3][3] == player) {
        return true;
    }
    if (tabletop[1][3] == player && tabletop[2][2] == player && tabletop[3][1] == player) {
        return true;
    }
    for (int i = 1; i < 4; i++) {
        if (tabletop[i][1] == player && tabletop[i][2] == player && tabletop[i][3] == player) {
            return true;
        }
        if (tabletop[i][1] == player && tabletop[i][2] == player && tabletop[i][3] == player) {
            return true;
        }
        if (tabletop[i][1] == player && tabletop[i][2] == player && tabletop[i][3] == player) {
            return true;
        }
        if (tabletop[1][i] == player && tabletop[2][i] == player && tabletop[3][i] == player) {
            return true;
        }
        if (tabletop[1][i] == player && tabletop[2][i] == player && tabletop[3][i] == player) {
            return true;
        }
        if (tabletop[1][i] == player && tabletop[2][i] == player && tabletop[3][i] == player) {
            return true;
        }
    }
    return false;
}

char if_victory() {//checks who won and returns "m" if it is the machine, "p" if it is the player and "n" if none
    if (check_victory('O')) {
        return 'p';
    }
    if (check_victory('X')) {
        return 'm';
    }
    return 'n';
}

void player_first() {
    //Turn 1 - Player
    table_print();
    play_player();
    //Turn 2 - Machine
    if (tabletop[2][2] == '-') {
        printf("Machine turn! \n");
        tabletop[2][2] = 'X';
        table_print();
    }else {
        play_machine();
    }
    //Turn 3 - Player
    play_player();
    //Turn 4 - Machine
    //If the player marks the two opposite ends, the only way to stop the play is for the machine to play on some "non-end"
    if (tabletop[1][1] == 'O' && tabletop[3][3] == 'O') {
        tabletop[3][2] = 'X';
        table_print();
    } else if (tabletop[1][3] == 'O' && tabletop[3][1] == 'O') {
        tabletop[3][2] = 'X';
        table_print();
    }//If the player crosses a middle-edge with an end, the machine can only stop playing on one end
    else if (tabletop[1][1] == 'O' && tabletop[2][3] == 'O') {
        tabletop[1][2] = 'X';
        table_print();
    }else if (tabletop[3][1] == 'O' && tabletop[2][3] == 'O') {
        tabletop[3][3] = 'X';
        table_print();
    }else if (tabletop[2][1] == 'O' && tabletop[3][3] == 'O') {
        tabletop[1][3] = 'X';
        table_print();
    }else if (tabletop[2][3] == 'O' && tabletop[3][2] == 'O') {
        tabletop[3][3] = 'X';
        table_print();
    }else {
        play_machine();
    }
    //Turn 5 - Player
    play_player();
    if (if_victory()=='p') {
        printf("Oh no! You really beat me, congratulations! Let's go again! \n");
    }else {
        //Turn 6 - Machine
        play_machine();
        if (if_victory()=='m') {
            printf("What a shame! You didn't succeed this time, but don't give up, try again! \n");
        }else {
            //Turn 7 - Player
            play_player();
            if (if_victory()=='p') {
                printf("Oh no! You really beat me, congratulations! Let's go again! \n");
            }else {
                //Turn 8 - Machine
                play_machine();
                if (if_victory()=='m') {
                    printf("What a shame! You didn't succeed this time, but don't give up, try again! \n");
                }else {
                    //turn 9 - Player
                    play_player();
                    if (if_victory()=='p') {
                        printf("Oh no! You really beat me, congratulations! Let's go again! \n");
                    }else {
                        printf("Very good! That was close, but I'm sure you'll win next time! Try again! \n");
                    }
                }
            }
        }
    }
}

void machine_first() {
    //Turn 1 - Machine
    printf("Machine turn! \n");
    tabletop[2][2] = 'X';
    table_print();
    //Turn 2 - Player
    play_player();
    //Turn 3 - Machine
    play_machine();
    //Turn 4 - Player
    play_player();
    //Turn 5 - Machine
    play_machine();
    if (if_victory()=='m') {
        printf("What a shame! You didn't succeed this time, but don't give up, try again! \n");
    }else {
        //Turn 6 - Player
        play_player();
        if (if_victory()=='p') {
            printf("Oh no! You really beat me, congratulations! Let's go again! \n");
        }else {
            //Turn 7 - Machine
            play_machine();
            if (if_victory()=='m') {
                printf("What a shame! You didn't succeed this time, but don't give up, try again! \n");
            }else {
                //Turn 8 - Player
                play_player();
                if (if_victory()=='p') {
                    printf("Oh no! You really beat me, congratulations! Let's go again! \n");
                }else {
                    //turn 9 - Machine
                    play_machine();
                    if (if_victory()=='m') {
                        printf("What a shame! You didn't succeed this time, but don't give up, try again! \n");
                    } else{
                        printf("Very good! That was close, but I'm sure you'll win next time! Try again! \n");
                    }
                }
            }
        }
    }
}

bool who_starts () {
    char ask;
    printf("Would you like to get started? (Y or N)\n");
    scanf("%s", &ask);
    while (ask != 'Y' && ask != 'N') {
        printf("Please enter a valid option (Y or N) \n");
        scanf("%s", &ask);
    }
    if (ask == 'Y') {
        return true;
    }
    return false;
}

bool play_again () {
    char ask;
    printf("\033"); //Arrow :)
    printf("Would you like to play again? (Y or N)\n");
    scanf("%s", &ask);
    while (ask != 'Y' && ask != 'N') {
        printf("Please enter a valid option (Y or N) \n");
        scanf("%s", &ask);
    }
    if (ask == 'Y') {
        return true;
    }
    return false;
}

int main(){
    if (who_starts()) {
        player_first();
    } else {
        machine_first();
    }
    while (play_again()) {
        reset_table();
        if (who_starts()) {
            player_first();
        } else {
            machine_first();
        }
    }
    return EXIT_SUCCESS;
}
