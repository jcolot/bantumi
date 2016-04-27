/************************************************

   main.c
   ------

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

#define DEPTH 15


/*
 * Calcule le meilleur resultat qu'on peut attendre d'un coup a une profondeur
 * d'arbre donnee
 */

int getBestWorstValueOld(game_t * game, int move, int depth) {

    int value;
    int tmp;
    int minimax;
    player_t player;
    
    game_t * gamecpy;
    gamecpy = malloc(sizeof(game_t));

    memcpy(gamecpy, game, sizeof(game_t));
    player = game->player;

/*
 * Si le player est 'human'    (1) on met le facteur minimax a -1 (recherche de minima)
 * si le player est 'computer' (0) on met le facteur minimax a +1 (recherche du maxima)
 */
    minimax = player?1:-1; 
    value = -INT_MAX;
    doMove(gamecpy, move);

    if (isEndGame(gamecpy)) {
/*
 *  Fin de partie, on retourne le value
 */
        value = evalBoard(gamecpy);
        free(gamecpy);
	    return  value;
    }

    if (!depth) {
        value = evalBoard(gamecpy);
        free(gamecpy);
	    return  value;
    }
    
/*
 * La boucle principale
 */

    for (move = 0; move < 6; move++) {
/*
 * On ne tient pas compte des coups non-legaux
 */
        
        if (game->board[player][move] > 0) {
/*
 * Si le joueur courant est computer, on cherche le max
 * sinon on cherche le min en changeant le signe du value
 * Le plus grand nombre d'un ensemble devient le plus petit si on change
 * les signes
 *
 */

	        tmp = getBestWorstValueOld(gamecpy, move, depth - 1) * minimax;       
            if (tmp > value) value = tmp;
        }
    }

    value = value * minimax;
    free(gamecpy);
    return value;
}


int getBestWorstValue(game_t * game, int move, int depth, int alpha, int beta) {

    int value;
    int tmp;
    player_t player;
    
    game_t * gamecpy;

    gamecpy = malloc(sizeof(game_t));
    memcpy(gamecpy, game, sizeof(game_t));
   
    player = game->player;

    doMove(gamecpy, move);

    if (isEndGame(gamecpy)) {
/*
 *  Fin de partie, on retourne le value
 */
        value = evalBoard(gamecpy);
        free(gamecpy);
	    return  value;
    }

    if (!depth) {
        value = evalBoard(gamecpy);
        free(gamecpy);
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
/*
 * On ne tient pas compte des coups non-legaux
 */
        value = -INT_MAX;
        for (move = 0; move < 6; move++) {
       
            if (game->board[player][move] > 0) {

	            tmp = getBestWorstValue(gamecpy, move, depth - 1, alpha, beta);
                if (tmp > value) value = tmp;
                if (value > alpha) alpha = value;
                if (beta <= alpha) break;
            }
        }

        
    } else {
        value = INT_MAX;
        for (move = 0; move < 6; move++) {
       
            if (game->board[player][move] > 0) {

	            tmp = getBestWorstValue(gamecpy, move, depth - 1, alpha, beta);
                if (tmp < value) value = tmp;
                if (value < beta) beta = value;
                if (beta <= alpha) break;
            }
        }
    }

    free(gamecpy);
    return value;
}


/*
 * Calcule le meilleur coup
 * en comparant les meilleurs values
 *
 */

int getBestMove(game_t * game) {
    int bestMove;
    int bestValue;
    int player;
    int move;
    int tmp;

    player = game->player;
    bestMove = -1;
    bestValue = -INT_MAX;

    for (move = 0; move < 6; move++) {
        if (game->board[player][move] != 0) {
/*
 *  Coup non legal
 */
	        tmp = getBestWorstValue(game, move, DEPTH, -INT_MAX, INT_MAX);

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
