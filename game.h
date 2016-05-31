/************************************************
    game.c
    ------
    
    Contient les fonctions qui implementent les
    regles du jeu
    
    $Author: julien colot $
    Copyleft
************************************************/

#ifndef GAME_H
#define GAME_H
#include "typedefs.h"

game_t * initGame(); 
state_t playGame(game_t * game, int maxDepth);
bool isEndGame(game_t * game);
void doMove(game_t * game, int bowlNum);
void displayBoard(game_t * game);
void stateMachine(); 
void printScore(game_t * game);
void displayError();
#endif

