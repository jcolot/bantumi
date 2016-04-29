/************************************************

   strategy.c
   ----------

  $Author: julien colot $

  Copyleft

************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "typedefs.h"
#include "game.h"
#include "strategy.h"


/*
 * Calcule le meilleur resultat qu'on est assure 
 * d'obtenir dans le "worst case scenario"
 * a une profondeur d'arbre donnee (depth)
 * MINIMAX avec elagage alpha-beta
 */

int getBestValue(game_t * gameStack, int move, int depth, int maxDepth, int alphabeta[2]) {

    int value;
    int tmp;
    int minimax;
    player_t player;
    
    game_t * gameCpy;
    
    memcpy(&gameStack[depth], &gameStack[depth - 1], sizeof(game_t));
    gameCpy = &gameStack[depth];

    player = gameCpy->player;
	minimax = (player - 1) & 1;
    doMove(gameCpy, move);

    if (isEndGame(gameCpy)) {
/*
 *  Fin de partie, on retourne le value
 */
        value = evalBoard(gameCpy);
        return  value;
    }

    if (depth == maxDepth) {
        value = evalBoard(gameCpy);
        return  value;
    }
    
/*
 * La boucle principale
 * Si le joueur courant est computer, on cherche le max
 * sinon on cherche le min en changeant le signe du value
 * Le plus grand nombre d'un ensemble devient le plus petit si on change
 * les signes
 *
 */

    value = minimax * INT_MAX;
        
    for (move = 0; move < 6; move++) {
 /*
 * On ne tient pas compte des coups non-legaux
 */      
        if (gameCpy->board[player][move] > 0) {
            tmp = minimax * getBestValue(gameStack, move, depth + 1, maxDepth, alphabeta);
            if (tmp > value) {
                value = tmp;
                alphabeta[player] = tmp;
                if (alphabeta[0] - alphabeta[1] >= 0) break;
            }
        }
    }


    return value;
}

/*
 * Calcule le meilleur coup
 * en comparant les meilleurs value
 */

int getBestMove(game_t * game, int maxDepth) {
    int bestMove;
    int bestValue;
    int alphabeta[2];
    int player;
    int move;
    int tmp;
    
    game_t * gameStack;
    gameStack = (game_t *)malloc(sizeof(game_t) * (maxDepth + 1));
    memcpy(&gameStack[0], game, sizeof(game_t));

    printf("test");

    player = game->player;
    bestMove = -1;
    bestValue = -INT_MAX;

    for (move = 0; move < 6; move++) {
        if (game->board[player][move] != 0) {
/*
 *  Coup non legal
 */
            alphabeta[2] = {INT_MAX, -INT_MAX};
            tmp = getBestValue(gameStack, move, 1, maxDepth, alphabeta);
            
            if (tmp >= bestValue) {
                bestValue = tmp;
                bestMove = move;
                
            }
        }
    }
    return bestMove;
}

int evalBoard(game_t * game) {
    return game->board[computer][6] - game->board[human][6];
}
