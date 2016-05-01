/************************************************

  game.c
  ------

  Contient les fonctions qui implementent les
  regles du jeu

  $Author: julien colot $

  Copyleft

************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include <ctype.h>
#include "game.h"
#include "strategy.h"

/*
 *   Fonction: initGame  
 *   -----------------------
 *  
 *   Prepare le plateau avec un nombre de graine choisi au hasard 
 *   Choisit qui joue en premier
 *
 */

game_t * initGame(int firstPlayer, int iniSeeds) {

    int i;
    game_t * game;
    game  = malloc(sizeof(game_t));

/*   
 *  choisi aleatoirement le nombre de graines entre 1 et 6 
 */
    if (iniSeeds == -1) iniSeeds = 1 + rand() % 6;

    for (i = 0; i < 14; i++) (*game->board)[i] = iniSeeds;

/*
 * affecte les scores a 0
 */
   
    game->board[0][6] = 0;
    game->board[1][6] = 0;

    if (firstPlayer == -1) {
        game->player = rand() % 2;
    } else {
        game->player = firstPlayer;
    }
/*   
 *  choisi aleatoirement le premier joueur
 */

    if (game->player == 0) {

        printf("\nYou are playing first\n");        

    } else {

        printf("\nThe computer is playing first\n");        

    }

/*
 * retourne la structure decrivant le jeu
 */

    return game;
}

/*
 *   Fonction: playGame  
 *   -----------------------
 *   
 *   Boucle principale du jeu qui gere les coups du joueur machine, 
 *   et les choix de coup du joueur humain
 *
 *
 */

state_t playGame(game_t * game, int maxDepth) {

    int move;
    char input[80];
    int i;
    char c;

/*
 * On boucle tant que le jeu n'est pas fini
 *
 */

    while (! isEndGame(game)) {
        if (game->player == human) {
            printf("Enter the coordinate of the bowl you want to play (");
/* 
 * On affiche les coups possibles au joueur
 */
          
            for (i = 0; i < 6; i++) {
                c = 'A' + i;
                if (game->board[human][i] != 0) {
                    printf("%c-", c);
                }    
            }
            
            printf("\b): ");

/*
 * On protege l'input d'un overflow
 */

            if (fgets(input, 80, stdin) == NULL) {
                return FAILURE_STATE;
            }

/*
 * Q permet de quitter en cours de partie en evitant un Ctrl-c
 *
 */
            
            if (toupper(input[0]) == 'Q') {
                free(game);
                return EXIT_STATE;
            }

/*
 * On transforme la lettre du coup en nombre
 */
            move = input[0] - 'A';

            if (move >= 0 && move <= 5 && game->board[human][move] != 0) {
                doMove(game, move);
                displayBoard(game);   
            } else {
                printf("\n");
                printf("Error: '%c' is not a valid entry\n", input[0]);
                printf("\n");
            }
        } else {

/*   
 *  Sinon on cherche le meilleur coup en minimax jusqu'a la profondeur maxdepth
 *  Si maxDepth == 0, getBestMove retourne un coup aleatoirement
 *
 */

            move = getBestMove(game, maxDepth);
            
    	    printf("Computer plays %c:\n", move + 'a'); 
            doMove(game, move);
            displayBoard(game);
    
        }
    }

    return ENDGAME_STATE;
}

/*
 *   Fonction: doMove  
 *   ------------------
 *
 *   Effectue un coup suivant les regles du jeu
 *
 */

void doMove(game_t * game, int bowlNum) {

    int player = game->player;
    int opponent = !player;
    int offset = player * 7;
    int seeds = game->board[player][bowlNum];
    int i;
    int lastBowlNum = (bowlNum + seeds) % 7;
    int lastBowlOwner = (((bowlNum + seeds) % 14 < 6))?player:opponent;
    int seedsInLastBowl = game->board[lastBowlOwner][lastBowlNum];

/*
 * Premierement on vide le bol joue (sa valeur est sauvee dans la variable seeds)
 */

    game->board[player][bowlNum] = 0;

/*
 * On incremente chacun des bols suivant jusqu'a epuisement de seeds
 */

    for (i = 1; i <= seeds; i++){
        (*game->board)[(bowlNum + offset + i) % 14] ++;
    }

/*   
 *  Si le dernier grain tombe du meme cote
 *  et que le dernier bol incremente etait vide...       
 */

    if (lastBowlNum < 6 && lastBowlOwner == player && seedsInLastBowl == 0) {
        
/* 
 *  on ajoute le bol d'en face au score... 			
 */

        game->board[player][6] += game->board[opponent][5 - lastBowlNum];
    	game->board[opponent][5 - lastBowlNum] = 0;

/*        
 *  on ajoute le dernier bol au score et on vide ce bol 
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

/*
 *   Fonction: isEndGame  
 *   -------------------
 *
 *   Detecte la fin de la partie
 *
 */

bool isEndGame(game_t * game) {

    int player;
    int bowlNum;
    int emptyBowls;

/*
 * Si 6 les bols sont vides d'un des deux cotes, 
 * c'est la fin de la partie
 */

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
