#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define L   3
#define H   3

typedef enum {
    USER = 0,
    COMPUTER = 1
} Id;

typedef enum {
    BLANK = 10,
    X = 20,
    O = 30;
} Cell;

typedef struct {
    const char symbol;
    const bool is_computer;
} Player;

typedef enum {
    IN_PROGRESS = 100,
    WINNER_USER = 200,
    WINNER_COMPUTER = 300,
    DRAW = 400
} Status;

typedef struct {
    int board[L][H]; /* See #define L, H */
    int move_number;
    Status game_status;
    Player player[1]; /* See #define PLAYER_USER, PLAYER_COMPUTER */
} State;

void state_init(State *s) {
    printf("Tic Tac Toe v0.1, Graham Greenfield\n\n");

    s->move_number = 0;
    s->game_status = IN_PROGRESS;

    char user_symbol;
    printf("Play as X or O (X/O)? [X]: ");
    scanf("%c", &user_symbol);
    user_symbol = toupper(user_symbol);

    if (user_symbol != ('X' || 'O')) {
        user_symbol == 'X';
    }

    if (user_symbol == 'O') {
        s->player[USER]->player_symbol = 'O';
        s->player[USER]->is_computer = false;
        s->player[COMPUTER]->player_symbol = 'X';
        s->player[COMPUTER]->is_computer = true;
    } else {
        s->player[USER]->player_symbol = 'X';
        s->player[USER]->is_computer = false;
        s->player[COMPUTER]->player_symbol = 'O';
        s->player[COMPUTER]->is_computer = true;
    }

    printf("Player X moves first.\n");
}

bool prompt_repeat(void) {
    bool repeat_game = false;

    printf("Play again (Y/N)? [N]: ");
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

void prompt_game_grid(State *s) {
}

void state_player_move(State *s) {
}

void state_eval_move(State *s) {
}

bool tic_tac_toe(void) {
    State s;

    state_init(&s);

    do {
        prompt_game_grid(&s);
        state_player_move(&s);
        state_eval_move(&s);
    } while (s.game_status == IN_PROGRESS);

    return prompt_repeat();
}

int main(void) {
    bool replay = false;

    do {
        replay = tic_tac_toe();
    } while (replay == true);

    return 0;
}
