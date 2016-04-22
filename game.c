#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include "game.h"


game_t * initGame() {

    short i;
    short iniSeeds;
    game_t * game;
    game  = malloc(sizeof(game_t));

    do { 
        iniSeeds = rand() % 10; 
    } while (iniSeeds < 4);

    for (i = 0; i < 14; i++) {
       (*game->board)[i] = iniSeeds;
    }

   
    game->board[0][6] = 0;
    game->board[1][6] = 0;
    game->player = rand() % 2;
    return game;
}

state_t playGame(game_t * game) {

    short move;
    char input;
    
    printf("Enter the coordinate of the bowl you want to play:\n\n");

    while (! isEndGame(game)) {
        
        if (game->player == human) {

            while ((input = getchar()) == '\n' || input == EOF);
            
            if (input == 'q') {

                return EXIT_STATE;

            }

            move = input - 'A';

            if (move >= 0 && move <= 5) {
                doMove(game, move);
                displayBoard(game);
            } else {
                printf("Error: please choose a value between 'A' and 'F'\n");
            }

        } else {
            do {
                move = rand() % 6;
                printf("rand = %d\n", move);
            } while (game->board[computer][move] == 0);
            
            doMove(game, move);
            displayBoard(game);
        }
    }

    return ENDGAME_STATE;

}

void doMove(game_t * game, int bowlNum) {

    int player = game->player;
    int opponent = !player;
    int offset = player * 7;
    int seeds = game->board[player][bowlNum];
    int i;
    int lastBowlNum = (bowlNum + seeds) % 7;
    int lastBowlOwner = (((bowlNum + seeds) % 14 < 6))?player:opponent;
    int seedsInLastBowl = game->board[lastBowlOwner][lastBowlNum];

    game->board[player][bowlNum] = 0;

    for (i = 1; i <= seeds; i++){
        (*game->board)[(bowlNum + offset + i) % 14] ++;
    }


/*   
 *  si le dernier grain tombe du meme cote
 *  et que le dernier bol incremente etait vide...       
 */

    if (lastBowlNum < 6 && lastBowlOwner == player && seedsInLastBowl == 0) {
        
/* 
 *      on ajoute le bol d'en face au score... 			
 */

        game->board[player][6] += game->board[opponent][5 - lastBowlNum];
    	game->board[opponent][5 - lastBowlNum] = 0;

/*        
 *      on ajoute le dernier bol au score et on vide ce bol 
 */
        game->board[player][6]++;	
        game->board[player][lastBowlNum] = 0;
    } 

/* 
 *   C'est le tour de l'autre joueur,
 *   sauf si la derniere graine termine le bol de score, 
 */
   
     if ((bowlNum + seeds) % 14 != 6) {
        game->player = opponent;
    }
}


int main(){

    srand(time(NULL));
    stateMachine();
    printf("Bye bye!\n");
    exit(0);

}

void displayBoard(game_t * game) {


    int i,j,k;
    int * board;
    board = &(game->board[0][0]);

/* Un peu de decoration */    
        
    printf("       ");
    printf("+");
    for (j = 1; j < 64; j++){
        printf("-");
    };
    printf("+");
    printf("\n");

/* Rangee de bols du joueur du haut (cpu) */    
    k = 14;

    for (i=0;i<3;i++){
        printf("       ");
        for (j=0;j<=64;j++){
            if (j % 8 == 0){
                printf("|");
            } else {
                if (((j+4)%8)==0 &&i==1){
                    if (k>7&&k<14){
                        printf("\b%2d", board[k-1]);
                    } else {
                        printf(" ");
                    }
                    k--;
                } else {
                    printf(" ");
                } 
            }        
        }
        printf("\n"); 
    }
    
/* Les deux bols a la droite des joueurs (les scores) */

    printf("       ");
    for (j = 0; j <= 64; j++) {
        if (j > 8 && j < 56) {
            printf("-");
        } else if (!(j % 8)) {
            printf("+");
        } else if (j == 4) {
            printf("\b%2d", board[13]);
        } else if (j == 60) {
            printf("\b%2d", board[6]);
        } else {
            printf(" ");
        }
    };
    printf("\n");


/* rangee de bols du joueur du bas */    
    k = 0;

    for (i=0;i<3;i++){
        printf("       ");
        for (j=0;j<=64;j++){
            if (j % 8 == 0){
                printf("|");
            } else {
                if (((j+4)%8)==0 &&i==1){
                    if (k>0&&k<7){
                        printf("\b%2d", board[k-1]);
                    } else {
                        printf(" ");
                    }
                    k++;
                } else {
                    printf(" ");
                } 
            }        
        }
        printf("\n"); 
    }

/* encore un peu de decoration */    
        
    printf("       ");
    printf("+");
    for (j = 1; j < 64; j++){
        printf("-");
    };
    printf("+");
    printf("\n");
}

void displayError() {
    printf("Error");
}

bool isEndGame(game_t * game) {

    short player;
    short bowlNum;
    short emptyBowls = 0;

    for (player = 0; player < 2; player++) {
        for (bowlNum = 0; bowlNum < 6; bowlNum++) {
            if (game->board[player][bowlNum] == 0) {
                emptyBowls++;
                if (emptyBowls == 6) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

void printScore(game_t * game) {
    printf("Your score: %d\n", game->board[human][6]);
    printf("Score of the computer: %d\n", game->board[computer][6]);
}

void stateMachine() {

    char input;
    state_t state = INIT_STATE;
    game_t * game;
    int i;

    printf("\n\n");
    

    while (state != EXIT_STATE) {

        switch (state) {

            case INIT_STATE :

                printf(
            "     _______  _______  __    _  _______  __   __  __   __  ___ \n" 
            "    |  _    ||   _   ||  |  | ||       ||  | |  ||  |_|  ||   |\n" 
            "    | |_|  _||  |_|  ||   |_| ||_     _||  | |  ||       ||   |\n" 
            "    |     |_ |       ||       |  |   |  |  | |  ||       ||   |\n" 
            "    |  _    ||   _   ||  _    |  |   |  |  |_|  || || || ||   |\n" 
            "    | |_|   ||  | |  || | |   |  |   |  |       || ||_|| ||   |\n" 
            "    |_______||__| |__||_|  |__|  |___|  |_______||_|   |_||___|\n" 
            "                                                               \n" 
            "                                                               \n" 
                );
                
                printf("Make a selection:\n\n");
                printf("(Q)uit the game\n");
                printf("(P)lay\n\n");
                printf("Your choice> ");

                while ((input = getchar()) == '\n' || input == EOF);
 
                switch(input) {

                    case 'Q':
                        state = EXIT_STATE;
                        break;
                    
                    case 'P':
                        state = PLAY_STATE;
                        break;

                    default:
                        displayError();
                }
                
                break;

            case PLAY_STATE:
               /*
                * On delegue la gestion du jeu a 
                * la fonction playGame, jusqu'a la fin de la partie
                */ 
                game = initGame();
		displayBoard(game);
                state = playGame(game);
                break;
            
            case ENDGAME_STATE :

                printScore(game);
                printf("Do you want to play a new game?\n");
                do {scanf("%c", &input); } while (input == 32 || input == 9);

                switch(input) {

                    case 'q':
                        state = EXIT_STATE;
                        break;
                    
                    case 'p':
                        state = INIT_STATE;
                        break;

                    default:
                        displayError();
                }
                break;
        }
    }

    /* Quitte la fonction, et le jeu */
}
