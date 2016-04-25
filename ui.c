/************************************************

   ui.c
  -----
  Contient les fonctions d'affichage et 
  d'interaction avec l'utilisateur grace a une
  machine a etats finis.

  $Author: julien colot $

  Copyleft

************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>
#include <time.h>
#include "game.h"
#include "ui.h"

/*
 *   Fonction:  displayBoard 
 *   -----------------------
 *   
 *   Affiche le plateau dans son etat courant
 *   Utilisee apres chaque coup
 *
 */

void displayBoard(game_t * game) {

    int c,i,j,k;
    int * board;
    board = &(game->board[0][0]);

/* Un peu de decoration */    

    printf("\n");
    printf("\n");

    c = 'f'; 
    printf("             ");
    for (i = 0; i < 6; i++) {
        printf("   %c    ", c);
        c--;
    }   
    printf("\n");
        
    printf("    ");
    printf("+");
    for (j = 1; j < 64; j++){
        printf("-");
    };
    printf("+");
    printf("\n");
   
/* Rangee de bols du joueur du haut (cpu) */    

    k = 14;
    for (i=0;i<3;i++){
        printf("    ");
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
    
/* 
 * Les deux bols a la droite des joueurs (les scores) 
 */

    printf("    ");
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

/* 
 * rangee de bols du joueur du bas 
 */
    
    k = 0;
    for (i=0;i<3;i++){
        printf("    ");
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

/* 
 * encore un peu de decoration 
 */    
        
    printf("    ");
    printf("+");
    for (j = 1; j < 64; j++){
        printf("-");
    };
    printf("+");
    printf("\n");

    c = 'A'; 
    printf("             ");
    for (i = 0; i < 6; i++) {
        printf("   %c    ", c);
        c++;
    }
   
    printf("\n");
    printf("\n");
    printf("\n");
}

/*
 *   Fonction:  printScore 
 *   ---------------------
 *   
 *   Affiche le score
 *   Utilisee a la fin d'une partie
 *
 */

void printScore(game_t * game) {


    if (game->board[human][6] > game->board[computer][6]) {

        printf("Your won: %d-%d\n\n", game->board[human][6], game->board[computer][6]);

    } else if (game->board[human][6] < game->board[computer][6]) {

        printf("Game over!\n\n");
        printf("You lost: %d-%d\n\n", game->board[human][6], game->board[computer][6]);

    } else {

        printf("Draw: %d-%d\n\n", game->board[computer][6], game->board[human][6]);

    }
}

/*
 *   Fonction:  stateMachine
 *   -----------------------
 *   
 *   C'est la fonction qui implemente la machine a etat
 *   et assure les transitions d'etats
 *
 */

void stateMachine() {

    char input[80];
    state_t state = INIT_STATE;
    game_t * game;

    printf("\n\n");
    
    while (state != EXIT_STATE) {

        switch (state) {

            case INIT_STATE:

/*
 * Initialisation du jeu
 */
               
                printf("Make a selection:\n\n");
                printf("(P)lay\n");
                printf("(Q)uit the game\n\n");
                printf("Your choice> ");

                if (fgets(input, 80, stdin) == NULL) {
                    state = FAILURE_STATE;
                }

                switch(toupper(input[0])) {

                    case 'Q':
                        state = EXIT_STATE;
                        break;
                    
                    case 'P':
                        state = PLAY_STATE;
                        break;

                    default:
                        printf("\nError: entry '%c' is not valid\n\n", input[0]);
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

            case FAILURE_STATE :
/*
 * Juste au cas ou, en cas de probleme avec fgets
 */ 
                fprintf(stderr, "There was an unknown technical issue...");
                state = EXIT_STATE;
                break;
 
            case ENDGAME_STATE :
/*
 * En fin de partie
 */ 
                printScore(game);
                state = INIT_STATE;
                break;

            case EXIT_STATE :

/*
 * En fait, ce case n'est pas utilise en pratique, 
 * et n'est present que pour eviter un warning avec -Wswitch
 */ 

                state = EXIT_STATE;
                break;
        }
    }
};
