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

int getBestValue(game_t ** gameStack, int move, int depth, int maxDepth, int alpha, int beta) {

    int value;
    int tmp;
    player_t player;
    
    game_t * gameCpy;
    
    depth = depth + 1;
    gameCpy = gameStack[depth - 1];
    memcpy(gameCpy, gameStack[depth], sizeof(game_t));
   
    player = gameCpy->player;
    doMove(gameCpy, move);

    if (isEndGame(gameCpy)) {
/*
 *  Fin de partie, on retourne le value
 */
        value = evalBoard(gameCpy);
        free(gameCpy);
		return  value;
    }

    if (depth == maxDepth) {
        value = evalBoard(gameCpy);
        free(gameCpy);
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

    if (player == computer) {
        value = -INT_MAX;
        for (move = 0; move < 6; move++) {
 /*
 * On ne tient pas compte des coups non-legaux
 */      
            if (game->board[player][move] > 0) {

	        tmp = getBestValue(gameCpy, move, depth, maxDepth, alpha, beta);
                if (tmp > value) value = tmp;
                if (value > alpha) alpha = value;
                if (beta <= alpha) break;
            }
        }
    } else {
        value = INT_MAX;
        for (move = 0; move < 6; move++) {
       
            if (game->board[player][move] > 0) {

	        tmp = getBestWorstValue(gameCpy, move, depth - 1, alpha, beta);
                if (tmp < value) value = tmp;
                if (value < beta) beta = value;
                if (beta <= alpha) break;
            }
        }
    }

    free(gameCpy);
    return value;
}

/*
 * Calcule le meilleur coup
 * en comparant les meilleurs value
 */

int getBestMove(game_t * game, int maxDepth) {
    int bestMove;
    int bestValue;
    int player;
    int move;
    int tmp;
    
    game_t ** gameStack;
    gameStack = malloc(sizeof(game_t) * (maxDepth + 1));
    memcpy(gameStack[maxDepth], game, sizeof(Board));

    player = game->player;
    bestMove = -1;
    bestValue = -INT_MAX;

    for (move = 0; move < 6; move++) {
        if (game->board[player][move] != 0) {
/*
 *  Coup non legal
 */
            tmp = getBestWorstValue(gameStack, move, maxDepth, -INT_MAX, INT_MAX);
            
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
