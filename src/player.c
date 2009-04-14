
#include "player.h"

void player_init(player_t * player){
    switch(player->type){
    case PLAYER_HUMAN_CUI:
        player_human_cui_init(player);
        break;
    default:
        DEBUG_ABORT("never reaches here");
    }
}

void player_exit(player_t * player){
    switch(player->type){
    case PLAYER_HUMAN_CUI:
        player_human_cui_exit(player);
        break;
    default:
        DEBUG_ABORT("never reaches here");
    }
}

uint32_t player_ask_next_move(player_t * player, board_t * board){
    uint32_t ret;
    switch(player->type){
    case PLAYER_HUMAN_CUI:
        ret = player_human_cui_ask_next_move(player, board);
        break;
    default:
        DEBUG_ABORT("never reaches here");
    }

    return ret;
}
