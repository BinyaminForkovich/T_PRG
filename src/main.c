#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 4

static void clear_screen(void) {
    printf("\033[2J\033[H");
}

static void reset_board(int board[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            board[row][col] = 0;
        }
    }
}

static int random_index(int limit) {
    if (limit <= 0) {
        return 0;
    }

    int bucket_size = RAND_MAX / limit;
    int threshold = bucket_size * limit;
    int value;

    do {
        value = rand();
    } while (value >= threshold);

    return value / bucket_size;
}

static void add_random_tile(int board[SIZE][SIZE]) {
    int empty_count = 0;
    int empty_rows[SIZE * SIZE];
    int empty_cols[SIZE * SIZE];

    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            if (board[row][col] == 0) {
                empty_rows[empty_count] = row;
                empty_cols[empty_count] = col;
                ++empty_count;
            }
        }
    }

    if (empty_count == 0) {
        return;
    }

    int choice = random_index(empty_count);
    board[empty_rows[choice]][empty_cols[choice]] = (rand() % 10 == 0) ? 4 : 2;
}

static bool slide_line(int line[SIZE], uint64_t *score) {
    int original[SIZE];
    int compact[SIZE];
    int merged[SIZE] = {0};
    int compact_count = 0;

    memcpy(original, line, sizeof(original));

    for (int i = 0; i < SIZE; ++i) {
        if (line[i] != 0) {
            compact[compact_count++] = line[i];
        }
    }

    int out = 0;
    for (int i = 0; i < compact_count; ++i) {
        if (i + 1 < compact_count && compact[i] == compact[i + 1]) {
            merged[out] = compact[i] * 2;
            *score += merged[out];
            ++i;
        } else {
            merged[out] = compact[i];
        }
        ++out;
    }

    for (int i = 0; i < SIZE; ++i) {
        line[i] = merged[i];
    }

    return memcmp(original, line, sizeof(original)) != 0;
}

static bool move_left(int board[SIZE][SIZE], uint64_t *score) {
    bool moved = false;

    for (int row = 0; row < SIZE; ++row) {
        moved |= slide_line(board[row], score);
    }

    return moved;
}

static bool move_right(int board[SIZE][SIZE], uint64_t *score) {
    bool moved = false;

    for (int row = 0; row < SIZE; ++row) {
        int line[SIZE];

        for (int col = 0; col < SIZE; ++col) {
            line[col] = board[row][SIZE - 1 - col];
        }

        moved |= slide_line(line, score);

        for (int col = 0; col < SIZE; ++col) {
            board[row][SIZE - 1 - col] = line[col];
        }
    }

    return moved;
}

static bool move_up(int board[SIZE][SIZE], uint64_t *score) {
    bool moved = false;

    for (int col = 0; col < SIZE; ++col) {
        int line[SIZE];

        for (int row = 0; row < SIZE; ++row) {
            line[row] = board[row][col];
        }

        moved |= slide_line(line, score);

        for (int row = 0; row < SIZE; ++row) {
            board[row][col] = line[row];
        }
    }

    return moved;
}

static bool move_down(int board[SIZE][SIZE], uint64_t *score) {
    bool moved = false;

    for (int col = 0; col < SIZE; ++col) {
        int line[SIZE];

        for (int row = 0; row < SIZE; ++row) {
            line[row] = board[SIZE - 1 - row][col];
        }

        moved |= slide_line(line, score);

        for (int row = 0; row < SIZE; ++row) {
            board[SIZE - 1 - row][col] = line[row];
        }
    }

    return moved;
}

static bool has_won(int board[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            if (board[row][col] == 2048) {
                return true;
            }
        }
    }

    return false;
}

static bool has_moves(int board[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            if (board[row][col] == 0) {
                return true;
            }
            if (col + 1 < SIZE && board[row][col] == board[row][col + 1]) {
                return true;
            }
            if (row + 1 < SIZE && board[row][col] == board[row + 1][col]) {
                return true;
            }
        }
    }

    return false;
}

static void print_board(int board[SIZE][SIZE], uint64_t score, bool won) {
    clear_screen();
    printf("2048\n");
    printf("Score: %llu\n\n", (unsigned long long)score);

    for (int row = 0; row < SIZE; ++row) {
        printf("+------+------+------+------+");
        printf("\n");
        for (int col = 0; col < SIZE; ++col) {
            if (board[row][col] == 0) {
                printf("|%6s", "");
            } else {
                printf("|%6d", board[row][col]);
            }
        }
        printf("|\n");
    }
    printf("+------+------+------+------+");
    printf("\n\n");
    printf("Controls: W/A/S/D move, R restart, Q quit\n");
    if (won) {
        printf("You reached 2048. Keep going or restart for another run.\n");
    }
}

static void start_new_game(int board[SIZE][SIZE], uint64_t *score, bool *won) {
    reset_board(board);
    *score = 0;
    *won = false;
    add_random_tile(board);
    add_random_tile(board);
}

int main(void) {
    int board[SIZE][SIZE];
    uint64_t score = 0;
    bool won = false;
    char input[64];

    srand((unsigned)time(NULL));
    start_new_game(board, &score, &won);

    for (;;) {
        print_board(board, score, won);

        if (!has_moves(board)) {
            printf("Game over. Press R to restart or Q to quit.\n");
        }

        printf("Move: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        char command = (char)tolower((unsigned char)input[0]);
        bool moved = false;

        if (command == 'q') {
            break;
        }
        if (command == 'r') {
            start_new_game(board, &score, &won);
            continue;
        }

        switch (command) {
            case 'a':
                moved = move_left(board, &score);
                break;
            case 'd':
                moved = move_right(board, &score);
                break;
            case 'w':
                moved = move_up(board, &score);
                break;
            case 's':
                moved = move_down(board, &score);
                break;
            default:
                continue;
        }

        if (moved) {
            add_random_tile(board);
            if (!won && has_won(board)) {
                won = true;
            }
        }

        if (!has_moves(board)) {
            print_board(board, score, won);
            printf("Game over. Press R to restart or Q to quit.\n");
            printf("Move: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                break;
            }
            command = (char)tolower((unsigned char)input[0]);
            if (command == 'q') {
                break;
            }
            if (command == 'r') {
                start_new_game(board, &score, &won);
            }
        }
    }

    return 0;
}