
#include <stdlib.h>
#include <stdio.h>
#include "typedefs.h"
#include "ui.h"



/*
 * Calcule le meilleur score qu'on peut attendre d'un coup a une profondeur
 * d'arbre donnee
 */

score_t getBestScore(short move, short depth) {

    short move;
    short player;
    short depth;
    int  ret;
    int  tmp;

    
    game_t * gamecpy;
    gamecpy  = malloc(sizeof(game_t));

    memcpy(game, gamecpy, sizeof(game_t));
   
    player = gamecpy->player;
    doMove(move);
    nextPlayer = gamecpy->player;

    if (!depth) {
	/*
	 *  
	 *  the board with no more checking of moves.
	 */
        free(gamecpy);
	return = game->board[player][6];
    }
    
    if (endGame(gamecpy)) {
	/*
	 *  No more moves for the next player. Just evaluate the board.
	 */
	ret = evalBoard(player);
	return = game->board[player][6];
    }

    /*
     *  Check all following moves, and choose the one maximizing
     *  the score for the player in question.
     */
    ret = -INT_MAX;
    for (move = 0; move < 6; move++) {
        
	tmp = getBestMove(nextPlayer, move, depth - 1);
	if (tmp > ret) {
	    ret = tmp;
	}
    }
    if (player != nextPlayer)
	ret = -ret;
}

/*
 * Calcule le meilleur coup
 * en comparant les meilleurs scores
 *
 */

move_t getBestMove(game_t * game) {
    int   q;              /* a counter variable */
    PMove *move;          /* array of pointerts to possible moves to make */
    int   numMoves;       /* number of moves in this array */
    PMove bestMove;       /* the best move so far */
    Score v;              /* value for a single move */
    Score bestValue;      /* the value of the best move so far */

    
    /*
     *  Now loop through all possible moves for the given player,
     *  and find the one with the highest score according to the
     *  minimax algorithm. If there are no possible moves left,
     *  we return NULL.
     */
     
    bestMove = NULL;
    move = getMoves(player, &numMoves);
    bestValue = -INF_SCORE;
    for (move = 0; move < 6; move++) {
	v = getBestScore(player, *move, maxPly - 1);

	if (v >= bestValue) {

	    bestValue = v;
	    bestMove = *move;
	}
	++move;
    }
    return bestMove;
}
