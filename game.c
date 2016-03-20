#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define SEEDS_PER_BOWL 10

typedef enum {bottom = 0, top} player_t;
typedef enum {false = 0, true} bool;
typedef enum {INIT_STATE, PLAY_STATE, CONFIG_STATE, ERROR_STATE, EXIT_STATE} state_t;
typedef struct {
    int board[2][7];
    player_t player;
} game_t;

static game_t * game;

void initGame() {
    int i;
    game = malloc(sizeof(game_t));
    for (i = 0; i < 14; i++) {
       (*game->board)[i] = SEEDS_PER_BOWL;
    }

    game->board[0][6] = 0;
    game->board[1][6] = 0;
    game->player = rand();
}

state_t playGame() {

}

void move(int bowlNum){

    int player = game->player;
    int opponent = !player;
    int offset = player * 7;
    int seeds = game->board[player][bowlNum];
    int i;
    int lastBowlNum = (bowlNum + seeds) % 7;
    int lastBowlOwner = (((bowlNum + seeds) % 14 > 6))?player:opponent;
    int seedsInLastBowl = game->board[lastBowlOwner][lastBowlNum];

    game->board[player][bowlNum] = 0;

    for (i = 1; i <= seeds; i++){
        (*game->board)[(bowlNum + i) % 14] ++;
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


int main(){
    stateMachine();
    return 1;
}

int displayBoard(int board[]){
    int i,j,k;

/* Un peu de decoration */    
        
    printf("    ");
    printf("+");
    for (j = 1; j < 64; j++){
        printf("-");
    };
    printf("+");
    printf("\n");

/* Rangee de bols du joueur du haut (cpu) */    
    k = 14;

    for (i=0;i<3;i++){
        printf("    ");
        for (j=0;j<=64;j++){
            if (j % 8 == 0){
                printf("|");
            } else {
                if (((j+4)%8)==0 &&i==1){
                    if (k>7&&k<14){
                        printf("\b%2d", board[k-1]);
                    } else {
                        printf(" ");
                    }
                    k--;
                } else {
                    printf(" ");
                } 
            }        
        }
        printf("\n"); 
    }
    
/* Les deux bols a la droite des joueurs (les scores) */

    printf("    ");
    for (j = 0; j <= 64; j++){
        if (j > 8 && j < 56) {
            printf("-");
        } else if (!(j % 8)) {
            printf("+");
        } else if (j == 4) {
            printf("\b%2d", board[13]);
        } else if (j == 60) {
            printf("\b%2d", board[6]);
        } else {
            printf(" ");
        }
    };
    printf("\n");


/* rangee de bols du joueur du bas */    
    k = 0;

    for (i=0;i<3;i++){
        printf("    ");
        for (j=0;j<=64;j++){
            if (j % 8 == 0){
                printf("|");
            } else {
                if (((j+4)%8)==0 &&i==1){
                    if (k>0&&k<7){
                        printf("\b%2d", board[k-1]);
                    } else {
                        printf(" ");
                    }
                    k++;
                } else {
                    printf(" ");
                } 
            }        
        }
        printf("\n"); 
    }

/* encore un peu de decoration */    
        
    printf("    ");
    printf("+");
    for (j = 1; j < 64; j++){
        printf("-");
    };
    printf("+");
    printf("\n");
    return 1;
}

int displayError() {
    printf("Error");
}

bool endGame() {

    player_t player;
    short bowlNum;
    short emptyBowls = 0;

    for (player = 0; player < 2; player++) {
        for (bowlNum = 0; bowlNum < 6; bowlNum++) {
            if (game->board[player][bowlNum] == 0) {
                emptyBowls++;
                if (emptyBowls == 6) {
                    return true;
                }
            }
        }
    }
}

int stateMachine() {

    int move;
    char input = getchar();
    printf("%c", input);
    state_t state = INIT_STATE;

    while (state != EXIT_STATE) {

        switch (state) {

            case INIT_STATE :

                switch(input) {

                    case 'q' :

                        state = EXIT_STATE;
                        break;
                    
                    case 'p' :

                        initGame(&game);
                        state = PLAY_STATE;
                        break;

                    default :

                        printf("%c", input);
                        displayError();
                }
                
                break;

            case PLAY_STATE:
                
                input = getchar();                                 
                move = input - 'A';

                if (move >= 0 && move <= 5) {
                    playGame(move);
                }

                break;

            case EXIT_STATE :
                printf("Exit!");
                return;

        }
    }

    /* Quitte la fonction, et le jeu */

    printf("Bye bye!");
}

