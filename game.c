/************************************************

  game.c
  ------

  Contient les regles du jeu

  $Author: julien colot $

  Copyleft

************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include "ui.h"
#include "game.h"


game_t * initGame() {

    short i;
    short iniSeeds;
    game_t * game;
    game  = malloc(sizeof(game_t));


/*   
 *  choisi aleatoirement le nombre de graines entre 4 et 9 
 */

    iniSeeds = 1 + rand() % 6;

    for (i = 0; i < 14; i++) {
       (*game->board)[i] = iniSeeds;
    }

   
    game->board[0][6] = 0;
    game->board[1][6] = 0;

/*   
 *  choisi aleatoirement le nombre de graines entre 4 et 9 
 */

    game->player = rand() % 2;
   
    if (game->player == 0) {

        printf("\nYou are playing first\n");        

    } else {

        printf("\nThe computer is playing first\n");        

    }

    return game;
}

state_t playGame(game_t * game) {

    short move;
    char input;
    
    while (! isEndGame(game)) {
        
        if (game->player == human) {
            
            printf("Enter the coordinate of the bowl you want to play: ");

            while ((input = getchar()) == '\n' || input == EOF);
            
            if (input == 'q') {

                return EXIT_STATE;

            }

            move = input - 'A';

            if (move >= 0 && move <= 5 && game->board[human][move] != 0) {

                doMove(game, move);
                displayBoard(game);

            } else if (move < 0 || move > 5){

                printf("\n");
                printf("Error: please choose a value between 'A' and 'F'\n");
                printf("\n");

            } else if (game->board[human][move] == 0){

                printf("\n");
                printf("Error: the selected bowl is empty\n");
                printf("\n");

            }

        } else {

            do {

                move = rand() % 6;

            } while (game->board[computer][move] == 0);
           
    	    printf("Computer played: %c\n", move + 'a'); 
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


bool isEndGame(game_t * game) {

    short player;
    short bowlNum;
    short emptyBowls;

    for (player = 0; player < 2; player++) {
    	emptyBowls = 0;
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
