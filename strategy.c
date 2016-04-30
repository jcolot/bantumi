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

int getBestValue(game_t * gameStack, int move, int depth, int maxDepth, int alpha, int beta, long long maxTime) {

    int value;
    int tmp;
    player_t player;
    
    game_t * gameCpy;
    
    memcpy(&gameStack[depth], &gameStack[depth - 1], sizeof(game_t));
    gameCpy = &gameStack[depth];

    player = gameCpy->player;
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
    
    if (currentTimestamp() > maxTime) return -1;
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
            if (gameCpy->board[player][move] > 0) {
                tmp = getBestValue(gameStack, move, depth + 1, maxDepth, alpha, beta, long long maxTime);
                if (tmp > value) value = tmp;
                if (value > alpha) alpha = value;
                if (beta <= alpha) break;
            }
        }
    } else {
        value = INT_MAX;
        for (move = 0; move < 6; move++) {
       
            if (gameCpy->board[player][move] > 0) {
                 
                tmp = getBestValue(gameStack, move, depth + 1, maxDepth, alpha, beta, long long maxTime);
                if (tmp < value) value = tmp;
                if (value < beta) beta = value;
                if (beta <= alpha) break;
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
    
        // Si maxDepth == 0, on cherche un coup aleatoire 
    if (! maxDepth) {
        do {
            move = rand() % 6;
        } while (game->board[player][move] == 0);
        
        // On sort de la fonction
        return move;
    }

    for (move = 0; move < 6; move++) {
        if (game->board[player][move] != 0) {
/*
 *  Coup non legal
 */
            tmp = getBestValue(gameStack, move, 1, maxDepth, -INT_MAX, INT_MAX);
            
            if (tmp >= bestValue) {
                bestValue = tmp;
                bestMove = move;
                
            }
        }
    }
    return bestMove;
}

int getBestMoveIterativeDeepening(game_t * game, int duration) {
    int bestMove;
    int bestValue;
    int player;
    int move;
    int tmp;
    int depth;
    long long maxTime;
    
    game_t * gameStack;
    gameStack = (game_t *)malloc(sizeof(game_t) * (maxDepth + 1));
    memcpy(&gameStack[0], game, sizeof(game_t));

    printf("test");

    player = game->player;
    bestMove = -1;
    bestValue = -INT_MAX;
    
    
/*
 * On definit le temps a accorder pour completer
 */
    maxTime = currentTimestamp() + (duration * 1000);

/* 
 *    On commence a la profondeur 1
 */
    depth = 1;
    while() {
        for (move = 0; move < 6; move++) {
            if (game->board[player][move] != 0) {
/*
 *  On verifie si le coup est legal
 */
                tmp = getBestValue(gameStack, move, 1, depth, -INT_MAX, INT_MAX, long long maxTime);
            
                if (tmp >= bestValue) {
                    bestValue = tmp;
                    tmpBestMove = move;
                } else if (tmp == -INT_MAX) {
/*
 *  Si le temps est ecoule:
 */
                    return bestMove;
                }
            }
        depth++;
        }
/* A chaque fin d'iteration, on conserve le meilleur coup        
 *
 */
        bestMove = tmpBestMove;
    }
    return bestMove;
}

long long currentTimestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; 
    return milliseconds;
}

int evalBoard(game_t * game) {
    return game->board[computer][6] - game->board[human][6];
}
