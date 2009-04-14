#include "game.h"

game_t * make_game(player_t * black, player_t * white){
    game_t * ret;
    ret = malloc(sizeof(game_t));
    ret->black = black;
    ret->white = white;

    return ret;
}

void quit_game(game_t * game){
    // free(game->black);
    // free(game->white);
    free(game);
}

void game_add_player(game_t * game, role_t role, player_t * player){
    switch(role){
    case ROLE_BLACK:
        game->black = player;
        break;
    case ROLE_WHITE:
        game->white = player;
        break;
    case ROLE_NONE:
        DEBUG_ABORT("never reaches here");
        break;
    }
}

bool game_play(game_t * game){
    if (game->black == NULL
        || game->white == NULL){
        return false;
    }

    board_t * board;
    role_t role;
    
    board = make_board();
    role = ROLE_BLACK;

    player_init(game->black);
    player_init(game->white);

    while(!check_finished(board)){
        uint32_t move_code;
        turn_t * turn;
        switch(role){
        case ROLE_BLACK:
            do {
                move_code = player_ask_next_move(game->black, board);
                turn = decode_turn(move_code, role);
            } while(turn->pass != true
                    && !validate_move(board, &turn->move));
            break;
        case ROLE_WHITE:
            do {
                move_code = player_ask_next_move(game->white, board);
                turn = decode_turn(move_code, role);
            } while(turn->pass != true
                    && !validate_move(board, &turn->move));
            break;
        default:
            DEBUG_ABORT("never reaches here");
            break;
        }
        if (!turn->pass){
            do_move(board, &turn->move);
        }
        free(turn);
        role ^= 1;
    }

    player_exit(game->black);
    player_exit(game->white);
    
    free(board);

    return true;
}

