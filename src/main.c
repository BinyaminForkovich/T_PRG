#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game.h"

int main(void) {
    UserProfile profile;

    memset(&profile, 0, sizeof(profile));
    srand((unsigned)time(NULL));

    if (!authenticate(&profile)) {
        return 0;
    }

    play_session(&profile);
    return 0;
}