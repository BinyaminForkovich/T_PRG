#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game.h"

int main(void) {
    srand((unsigned)time(NULL));

    for (;;) {
        UserProfile profile;

        memset(&profile, 0, sizeof(profile));

        if (!authenticate(&profile)) {
            return 0;
        }

        if (play_session(&profile)) {
            return 0;
        }
    }
}