#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define L 3
#define H 3

typedef enum { USER = 0, COMPUTER = 1 } id;

typedef enum { CELL_BLANK = 10, CELL_X = 20, CELL_O = 30 } cell;

typedef struct { char symbol; } player;

typedef enum {
    IN_PROGRESS = 100, WINNER_USER = 200, WINNER_COMPUTER = 300, DRAW = 400
} status;

typedef struct {
    cell board[L][H];
    int move;
    status game_status;
    player player[2];
} state;

cell player_cell(const state *s, id who) {
    return s->player[who].symbol == 'X' ? CELL_X : CELL_O;
}

void init(state *s) {
    printf("Tic Tac Toe v0.1, Graham Greenfield\n\n");

    s->move = 0;
    s->game_status = IN_PROGRESS;

    for (int row = 0; row < L; row++) {
        for (int col = 0; col < H; col++) {
            s->board[row][col] = CELL_BLANK;
        }
    }

    char user_symbol = 'X';
    printf("Play as X or O (X/O)? [X]: ");
    scanf("%c", &user_symbol);
    user_symbol = (char)toupper((unsigned char)user_symbol);

    if (user_symbol != 'X' && user_symbol != 'O') {
        user_symbol = 'X';
    }

    s->player[USER].symbol = user_symbol;
    s->player[COMPUTER].symbol = user_symbol == 'X' ? 'O' : 'X';

    printf("Player X moves first.\n");
}

bool prompt_repeat(void) {
    bool repeat_game = false;

    printf("Play again (Y/N)? [N]: ");
    char p;
    scanf("%c", &p);
    p = toupper(p);

    if (p != 'Y' && p != 'N') {
        return repeat_game;
    }

    if (p == 'Y') {
        repeat_game = true;
    }

    return repeat_game;
}

void game_grid(const state *s) {
    putchar('\n');

    for (int row = 0; row < L; row++) {
        for (int col = 0; col < H; col++) {
            if (s->board[row][col] == CELL_BLANK) {
                printf(" %d,%d ", row, col);
            } else {
                printf("  %c  ", s->board[row][col] == CELL_X ? 'X' : 'O');
            }

            if (col < H - 1) putchar('|');
        }

        putchar('\n');

        if (row < L - 1) {
            printf("-----+-----+-----\n");
        }
    }
}

void player_move(state *s) {

prompt:
    ;
    printf("\nYour move: Select a cell id (eg. 1,2 or 0,0)? ");

    int x, y;
    scanf("%d,%d", &x, &y);

    if (x >= 0 && x < L && y >= 0 && y < H && s->board[x][y] == CELL_BLANK) {
        s->board[x][y] = CELL_X;
        if(s->player[USER].symbol == 'O') {
            s->board[x][y] = CELL_O;
        }
    } else {
        goto prompt;
    }
}

/*
 * Evaluate board: +10 if AI wins, -10 if Human wins, 0 for draw/no winner.
 */

int evaluate(state *s) {
    /* Check rows and columns */
    for (int i = 0; i < L; i++) {
        if (s->board[i][0] == s->board[i][1] && s->board[i][1] == s->board[i][2]) {
            if (s->board[i][0] == CELL_O) return 10;
            if (s->board[i][0] == CELL_X) return -10;
        }
        if (s->board[0][i] == s->board[1][i] && s->board[1][i] == s->board[2][i]) {
            if (s->board[0][i] == CELL_O) return 10;
            if (s->board[0][i] == CELL_X) return -10;
        }
    }

    /* Check diagonals */
    if (s->board[0][0] == s->board[1][1] && s->board[1][1] == s->board[2][2]) {
        if (s->board[0][0] == CELL_O) return 10;
        if (s->board[0][0] == CELL_X) return -10;
    }
    if (s->board[0][2] == s->board[1][1] && s->board[1][1] == s->board[2][0]) {
        if (s->board[0][2] == CELL_O) return 10;
        if (s->board[0][2] == CELL_X) return -10;
    }

    return 0;
}

int minimax(state *s, int depth, bool is_max) {
    int score = evaluate(s);
    if (score == 10) return (score - depth);    /* Prefer quicker wins */
    if (score == -10) return (score + depth);   /* Prefer delayed losses */
    if (depth == 9) return 0;                   /* Draw */

    if (is_max) { /* AI's turn */
        int best = -1000;
        for (int i = 0; i < L; i++) {
            for (int j = 0; j < H; j++) {
                if (s->board[i][j] == CELL_BLANK) {
                    s->board[i][j] = CELL_O;
                    best = (best > minimax(s, depth + 1, false)) ? best : minimax(s, depth + 1, false);
                    s->board[i][j] = CELL_BLANK; /* Backtrack */
                }
            }
        }
        return best;
    }
    return 0;
}

void computer_move(state *s) {
    int best_score = -1000, best_row = -1, best_col = -1;

    for (int i = 0; i < L; i++) {
        for (int j = 0; j < H; j++) {
            if (s->board[i][j] == CELL_BLANK) {
                s->board[i][j] = CELL_O;
                int score = minimax(s, 0, false);
                s->board[i][j] = CELL_BLANK;

                if (score > best_score) {
                    best_score = score;
                    best_row = i;
                    best_col = j;
                }
            }
        }
    }

    s->board[best_row][best_col] = CELL_O;
}

void move(state *s, char sym) {
    game_grid(s);
    if (s->player[USER].symbol == sym) {
        player_move(s);
    } else {
        computer_move(s);
    }
}

void eval_move(state *s) {
    (void)s;
}

bool tic_tac_toe(void) {
    state s;

    init(&s);

    do {
        move(&s, 'X');
        eval_move(&s);
        move(&s, 'O');
        eval_move(&s);
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
