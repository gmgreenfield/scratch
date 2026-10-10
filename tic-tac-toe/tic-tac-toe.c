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

bool read_line(char *buffer, int size) {
    return fgets(buffer, size, stdin) != NULL;
}

cell player_cell(const state *s, id who) {
    return s->player[who].symbol == 'X' ? CELL_X : CELL_O;
}

bool init(state *s) {
    printf("Tic Tac Toe v0.1, Graham Greenfield\n\n");

    s->move = 0;
    s->game_status = IN_PROGRESS;

    for (int row = 0; row < L; row++) {
        for (int col = 0; col < H; col++) {
            s->board[row][col] = CELL_BLANK;
        }
    }

    char input[128];
    char user_symbol = 'X';

    printf("Play as X or O (X/O)? [X]: ");
    fflush(stdout);

    if (!read_line(input, sizeof input)) {
        return false;
    }

    sscanf(input, " %c", &user_symbol);
    user_symbol = (char)toupper((unsigned char)user_symbol);

    if (user_symbol != 'X' && user_symbol != 'O') {
        user_symbol = 'X';
    }

    s->player[USER].symbol = user_symbol;
    s->player[COMPUTER].symbol = user_symbol == 'X' ? 'O' : 'X';

    printf("Player X moves first.\n");

    return true;
}

bool prompt_repeat(void) {
    char input[128];
    char answer = 'N';

    printf("Play again (Y/N)? [N]: ");
    fflush(stdout);

    if (!read_line(input, sizeof input)) {
        return false;
    }

    sscanf(input, " %c", &answer);

    return toupper((unsigned char)answer) == 'Y';
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

bool player_move(state *s) {
    char input[128];

    for (;;) {
        printf("\nYour move: Enter row,column (each 0-2, e.g. 1,2): ");
        fflush(stdout);
        if (!read_line(input, sizeof input)) {
            return false;
        }

        char row_char, col_char, extra;

        if (sscanf(input, " %c , %c %c", &row_char, &col_char, &extra) != 2 ||
            row_char < '0' || row_char > '2' ||
            col_char < '0' || col_char > '2') {
            printf("Invalid coordinates. Use row,column with values from 0 to 2.\n");
            continue;
        }

        int row = row_char - '0';
        int col = col_char - '0';

        if (s->board[row][col] != CELL_BLANK) {
            printf("That cell is occupied. Choose an empty cell.\n");
            continue;

        }

        s->board[row][col] = player_cell(s, USER);

        return true;
    }
}

bool moves_left(const state *s) {
    for (int row = 0; row < L; row++) {
        for (int col = 0; col < H; col++) {
            if (s->board[row][col] == CELL_BLANK) {
                return true;
            }
        }
    }

    return false;
}

/* +10 if the computer wins, -10 if the user wins, 0 otherwise. */

int evaluate(const state *s) {
    cell computer = player_cell(s, COMPUTER);
    for (int i = 0; i < L; i++) {
        if (s->board[i][0] != CELL_BLANK &&
            s->board[i][0] == s->board[i][1] &&
            s->board[i][1] == s->board[i][2]) {
            return s->board[i][0] == computer ? 10 : -10;
        }
        if (s->board[0][i] != CELL_BLANK &&
            s->board[0][i] == s->board[1][i] &&
            s->board[1][i] == s->board[2][i]) {
            return s->board[0][i] == computer ? 10 : -10;
        }
    }
    if (s->board[1][1] != CELL_BLANK &&
        ((s->board[0][0] == s->board[1][1] && s->board[1][1] == s->board[2][2]) ||
         (s->board[0][2] == s->board[1][1] && s->board[1][1] == s->board[2][0]))) {
        return s->board[1][1] == computer ? 10 : -10;
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

bool move(state *s, char symbol) {
    game_grid(s);

    if (s->player[USER].symbol == symbol) {
        return player_move(s);
    }

    computer_move(s);

    return true;
}

void eval_move(state *s) {
    int score = evaluate(s);

    if (score == 10) {
        s->game_status = WINNER_COMPUTER;
    } else if (score == -10) {
        s->game_status = WINNER_USER;
    } else if (!moves_left(s)) {
        s->game_status = DRAW;
    } else {
        s->game_status = IN_PROGRESS;
    }
}

bool tic_tac_toe(void) {
    state s;

    if (!init(&s)) {
        return false;
    }

    do {
        if (!move(&s, 'X')) {
            return false;
        }
        eval_move(&s);
        if (!move(&s, 'O')) {
            return false;
        }
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
