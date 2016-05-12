/************************************************

   strategy.c
   ----------

  Implementation de la strategie Minimax

  $Author: julien colot $

  Copyleft

************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>
#include "typedefs.h"
#include "game.h"
#include "strategy.h"


/* 
 *  Fonction:  getBestValue
 *  -----------------------
 *
 * Calcule le meilleur resultat qu'on est assure 
 * d'obtenir dans le "worst case scenario"
 * a une profondeur d'arbre donnee (depth)
 * MINIMAX avec elagage alpha-beta
 * modifie pour le iterative-deepening
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

/*
 * On verifie le temps imparti, si le temps est passe, 
 * on renvoie -INT_MAX comme signal pour getBestMove     
 */
    if (currentTimestamp() > maxTime) return -INT_MAX;

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

    if (player == computer) {
        value = -INT_MAX;
        for (move = 0; move < 6; move++) {
 /*
 * On ne tient pas compte des coups non-legaux
 */      
            if (gameCpy->board[player][move] > 0) {
                tmp = getBestValue(gameStack, move, depth + 1, maxDepth, alpha, beta, maxTime);
                if (tmp > value) {value = tmp; alpha = tmp;}
                if (beta <= alpha) break;
            }
        }
    } else {
        value = INT_MAX;
        for (move = 0; move < 6; move++) {
       
            if (gameCpy->board[player][move] > 0) {
                 
                tmp = getBestValue(gameStack, move, depth + 1, maxDepth, alpha, beta, maxTime);
                if (tmp < value) {value = tmp; beta = tmp;}
                if (beta <= alpha) break;
            }
        }
    }

    return value;
}

/*
 *  Fonction:  getBestMove
 *  -----------------------
 *
 * Calcule le meilleur coup
 * en comparant les meilleurs value dans le temps imparti
 */

int getBestMove(game_t * game, int duration) {
    int bestMove;
    int bestValue;
    int player;
    int move;
    int tmp;
    int tmpBestMove;
    int maxDepth;
    long long maxTime;
    
    game_t * gameStack;
    gameStack = (game_t *)malloc(sizeof(game_t) * (100));
    memcpy(&gameStack[0], game, sizeof(game_t));

    player = game->player;
    bestMove = -1;
    bestValue = -INT_MAX;
    
/*
 * On definit le temps a accorder pour completer
 */
    maxTime = currentTimestamp() + (duration * 300);

/* 
 *    On commence a la profondeur 1
 */
    maxDepth = 1;
    while(maxDepth < 100) {
        for (move = 0; move < 6; move++) {
            if (game->board[player][move] != 0) {
/*
 *  On verifie si le coup est legal
 */
                tmp = getBestValue(gameStack, move, 1, maxDepth, -INT_MAX, INT_MAX, maxTime);
            
                if (tmp >= bestValue) {
                    bestValue = tmp;
                    tmpBestMove = move;
                } else if (tmp == -INT_MAX) {
/*
 *  Si le temps est ecoule:
 */
                    free(gameStack);
                    printf("\n\nComputer stopped searching at depth  %d\n\n", maxDepth - 1);
                    return bestMove;
                }
            }
        }

/* A chaque fin d'iteration, on conserve le meilleur coup        
 *
 */
        maxDepth++;
        bestMove = tmpBestMove;
    }

    free(gameStack);
    return bestMove;
}

/*  Fonction:  evalBoard
 *  --------------------
 *
 * Retourne la difference des scores pour evaluer
 * la valeur du plateau
 */

int evalBoard(game_t * game) {
    return game->board[computer][6] - game->board[human][6];
}

/*  Fonction:  currentTimestamp
 *  ---------------------------
 *
 * Retourne le temps en miillisecond
 * function trouvee sur le web
 */

long long currentTimestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; 
    return milliseconds;
}
