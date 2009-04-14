
#include "main.h"


int main(int argc, char ** argv){
    char buf[1024];
    uintptr_t buflen;

    buflen = 1;
    buf[0] = '*';

    player_t * player1;
    player_t * player2;
    player1 = make_player_human_cui(ROLE_BLACK);
    player2 = make_player_human_cui(ROLE_WHITE);

    game_t * game = make_game(player1, player2);

    game_play(game);

    return 0;
}
