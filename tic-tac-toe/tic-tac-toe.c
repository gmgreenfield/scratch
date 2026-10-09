#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define L 3
#define H 3

typedef enum {
    CELL_BLANK,
    CELL_X,
    CELL_Y
} Cell;

typedef enum {
    SYMBOL_X,
    SYMBOL_O
} Symbol;

typedef struct {
    Symbol player_symbol;
    bool is_computer;
} Player;

typedef enum {
    IN_PROGRESS,
    DRAW,
    USER_WINS,
    COMPUTER_WINS
} Status;

typedef struct {
    int board[L][H];
    int move_number;
    Status game_status;
    Player user;
    Player computer;
    Player *winner;
    Player *current_turn;
} State;

void prompt_player_symbol_select(State *s) {
    printf("Play as X or O? [X]: ");
    scanf("%c", &user_symbol);

    if (*user_symbol != ('X' || 'O')) {
        *user_symbol = 'X';
        *computer_symbol = 'O';
    }
}

bool prompt_repeat_game(void) {
    bool repeat_game = false;

    printf("Play again (Y/N)? [Y]: ");
    char p;
    scanf("%c", &p);

    if (p != ('Y' || 'N')) {
        return repeat_game;
    }

    if (p == 'Y') {
        repeat_game = true;
    }

    return repeat_game;
}

bool tic_tac_toe(void) {
    Player player_user = {.symbol = 'X', .is_computer = false};
    Player player_computer = {.symbol = 'Y', .is_computer = true};
    State s;

    printf("Tic Tac Toe v0.1, Graham Greenfield\n\n");

    prompt_player_symbol_select(&s);

    printf("Player X moves first.\n");

    return prompt_repeat_game();
}

int main(void) {
    bool replay = false;

    do {
        replay = tic_tac_toe();
    } while (replay == true);

    return 0;
}
