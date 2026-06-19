#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

#define SIZE 4
#define USERNAME_LIMIT 63
#define PASSWORD_LIMIT 63

typedef struct {
    char username[USERNAME_LIMIT + 1];
    char password[PASSWORD_LIMIT + 1];
    int board[SIZE][SIZE];
    uint64_t score;
    uint64_t highscore;
    bool won;
    bool has_saved_game;
} UserProfile;

bool authenticate(UserProfile *profile);
bool play_session(UserProfile *profile);

#endif
