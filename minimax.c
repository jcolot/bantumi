/* $Id: minimax.c,v 1.1.1.1 1995-07-25 11:55:21 sverrehu Exp $ */
/**************************************************************************
 *
 *  FILE            minimax.c
 *
 *  DESCRIPTION     General routines for doing simple minimax game tree
 *                  search. The caller supplies helper functions for doing
 *                  everything that is not directly a part of the algorithm.
 *
 *  WRITTEN BY      Sverre H. Huseby & Glenn Terje Lines
 *
 **************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "minimax.h"

/**************************************************************************
 *                                                                        *
 *                       P R I V A T E    D A T A                         *
 *                                                                        *
 **************************************************************************/

/*
 *  Define RANDOM_CHOICE to have the routine choose randomly among moves
 *  giving equal scores. The main part of the program should use srand()
 *  to initialize the random number generator.
 */
#define RANDOM_CHOICE

/*
 *  User supplied callback functions.
 */
static void   (*pushBoard)(void);
static void   (*popBoard)(void);
static PMove *(*getMoves)(Player, int *);
static Player (*doMove)(Player, PMove);
static void   (*undoMove)(Player, PMove);
static Score  (*evalBoard)(Player);



/**************************************************************************
 *                                                                        *
 *                   P R I V A T E    F U N C T I O N S                   *
 *                                                                        *
 **************************************************************************/

/*-------------------------------------------------------------------------
 *
 *  NAME          recMiniMax
 *
 *  FUNCTION      Do recursive min/max.
 *
 *  INPUT         player  the player to evaluate.
 *                move    the move to do for this player.
 *                depth   max recursion depth.
 *
 *  RETURNS       The score for this move. A positive value indicates
 *                that the given player is in the lead after the move.
 *                A negative value means the opposite player is in the
 *                lead.
 *
 *  DESCRIPTION   This function is the core of the implementation. 
 *
 */
static Score recMiniMax(Player player, PMove move, int depth)
{
    int    q;             /* my favourite counter */
    PMove  *newmove;      /* array of pointers to possible following moves */
    int    numMoves;      /* number of moves in this array */
    Player nextPlayer;    /* the player to do the move follwing */
    Score  ret;           /* the score we'll return in the end */
    Score  tmp;           /* the score for a single move */

    pushBoard();
    nextPlayer = doMove(player, move);
    if (!depth) {
	/*
	 *  We have reached the max recursion depth, so we just evaluate
	 *  the board with no more checking of moves.
	 */
	ret = evalBoard(player);
	goto finish;
    }
    
    newmove = getMoves(nextPlayer, &numMoves);
    if (!numMoves) {
	/*
	 *  No more moves for the next player. Just evaluate the board.
	 */
	ret = evalBoard(player);
	goto finish;
    }

    /*
     *  Check all following moves, and choose the one maximizing
     *  the score for the player in question.
     */
    ret = -INF_SCORE;
    for (q = 0; q < numMoves; q++) {
	tmp = recMiniMax(nextPlayer, *newmove, depth - 1);
	if (tmp > ret)
	    ret = tmp;
	++newmove;
    }
    if (player != nextPlayer)
	ret = -ret;
  finish:
    undoMove(player, move);
    popBoard();
    return ret;
}



/**************************************************************************
 *                                                                        *
 *                    P U B L I C    F U N C T I O N S                    *
 *                                                                        *
 **************************************************************************/

/*-------------------------------------------------------------------------
 *
 *  NAME          miniMax
 *
 *  FUNCTION      Get the best move for a given player.
 *
 *  INPUT         Several userdefined callback functions:
 *                fncPushBoard
 *                        This is called at the start of each level
 *                        in the recursion. If neccessary it should save
 *                        the status of the board, so it can be restored
 *                        later. This is neccessary if fncUndoMove() is
 *                        unable to undo a move.
 *                fncPopBoard
 *                        This is called at the end of each level in the
 *                        recursion. It can be used to restore the board.
 *                fncGetMoves
 *                        Return an array of pointers to the possible
 *                        moves for the given player. Also return the
 *                        number of moves in this array through the pointer
 *                        to the integer. Return NULL if there are no
 *                        possible moves to do.
 *                        The functions in this file don't know what a
 *                        move is, but only what pointers to moves are.
 *                        Therefore, this is an array of pointers to moves,
 *                        not an array of moves.
 *                        It is important that this array lasts through all
 *                        sublevels of the recursion from where it is
 *                        requested. You will probably use fncPushBoard()
 *                        and fncPopBoard() to maintain a list of arrays
 *                        to choose from.
 *                fncDoMove
 *                        This is also called at the start of each recursion
 *                        level.
 *                        Update the board with the given move. Also, return
 *                        the player that is to move next. For some games
 *                        this may be the player that just moved.
 *                fncUndoMove
 *                        Undo the given move. The move to undo is always the
 *                        move most recently done. If undoing is hard, you can
 *                        leave it to fncPopBoard() instead, since they
 *                        are both called at the end of a recursion level.
 *                        fncUndoMove() is called before fncPopBoard().
 *                fncEvalBoard
 *                        function evaluating the current state of the
 *                        board for the player supplied. This function should
 *                        return a positive value if the player is in the
 *                        lead, or a negative value if the opposite player
 *                        is in the lead. A higher (absolute) value indicates
 *                        a better lead.
 *                player  the player to evaluate.
 *                maxPly  max recursion depth.
 *
 *  RETURNS       Pointer to the best move to make, or NULL if no move
 *                available. For some games NULL means game over, for other
 *                games, it just means that it's the other player's turn.
 *                You should check the state of the game between each call
 *                to this function.
 *
 *                Note that the pointer returned points into the array
 *                returned by fncGetMoves(), so you shouldn't overwrite
 *                this array before using the move.
 *
 *  DESCRIPTION   This is the front end to the minimax algorithm.
 *                It sets it all up, and uses recMiniMax() to check each
 *                possible move.
 *
 */
PMove miniMax(
	      void   (*fncPushBoard)(void),
	      void   (*fncPopBoard)(void),
	      PMove *(*fncGetMoves)(Player, int *),
	      Player (*fncDoMove)(Player, PMove),
	      void   (*fncUndoMove)(Player, PMove),
	      Score  (*fncEvalBoard)(Player),
	      Player player,
	      int    maxPly
	      )
{
    int   q;              /* a counter variable */
    PMove *move;          /* array of pointerts to possible moves to make */
    int   numMoves;       /* number of moves in this array */
    PMove bestMove;       /* the best move so far */
    Score v;              /* value for a single move */
    Score bestValue;      /* the value of the best move so far */

    /*
     *  Set up the callback functions for the recursive routine.
     */
    pushBoard = fncPushBoard;
    popBoard  = fncPopBoard;
    getMoves  = fncGetMoves;
    doMove    = fncDoMove;
    undoMove  = fncUndoMove;
    evalBoard = fncEvalBoard;
    
    /*
     *  Now loop through all possible moves for the given player,
     *  and find the one with the highest score according to the
     *  minimax algorithm. If there are no possible moves left,
     *  we return NULL.
     */
    bestMove = NULL;
    move = getMoves(player, &numMoves);
    bestValue = -INF_SCORE;
    for (q = 0; q < numMoves; q++) {
	v = recMiniMax(player, *move, maxPly - 1);
#ifdef RANDOM_CHOICE
	/*
	 *  If this value equals the previous best value, we choose
	 *  randomly whether we should use this one instead.
	 */
	if (v > bestValue || (v == bestValue && (rand() & 1))) {
#else
	if (v >= bestValue) {
#endif
	    bestValue = v;
	    bestMove = *move;
	}
	++move;
    }
    return bestMove;
}
