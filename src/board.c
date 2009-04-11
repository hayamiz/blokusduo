
#include "board.h"

static piece_datum_t piece_data[BLOKUSDUO_PIECENUM] =
{   // piece:a
    { 1, { {0,0}, {0,0}, {0,0}, {0,0} } },
    
    // piece:b
    { 2, { {0,1}, {0,0}, {0,0}, {0,0} } },
    
    // piece:c
    { 3, { {0,1}, {0,-1}, {0,0}, {0,0} } },

    // piece:d
    { 3, { {1,0}, {0,-1}, {0,0}, {0,0} } },

    // piece:e
    { 4, { {0,-1}, {0,1}, {0,2}, {0,0} } },

    // piece:f
    { 4, { {0,-1}, {0,1}, {-1,1}, {0,0} } },

    // piece:g
    { 4, { {0,-1}, {1,0}, {0,1}, {0,0} } },

    // piece:h
    { 4, { {1,0}, {0,1}, {1,1}, {0,0} } },

    // piece:i
    { 4, { {-1,0}, {0,1}, {1,1}, {0,0} } },

    // piece:j
    { 5, { {0,-2}, {0,-1}, {0,1}, {0,2} } },

    // piece:k
    { 5, { {0,-2}, {0,-1}, {0,1}, {-1,1} } },

    // piece:l
    { 5, { {0,-2}, {0,-1}, {-1,0}, {-1,1} } },

    // piece:m
    { 5, { {0,-1}, {-1,0}, {-1,1}, {0,1} } },

    // piece:n
    { 5, { {-1,-1}, {0,-1}, {0,1}, {-1,1} } },

    // piece:o
    { 5, { {0,-1}, {1,0}, {0,1}, {0,2} } },

    // piece:p
    { 5, { {0,-1}, {0,1}, {-1,1}, {1,1} } },

    // piece:q
    { 5, { {0,-2}, {0,-1}, {1,0}, {2,0} } },

    // piece:r
    { 5, { {-1,-1}, {0,-1}, {1,0}, {1,1} } },

    // piece:s
    { 5, { {-1,-1}, {-1,0}, {1,0}, {1,1} } },

    // piece:t
    { 5, { {-1,-1}, {-1,0}, {0,1}, {1,0} } },

    // piece:u
    { 5, { {0,-1}, {-1,0}, {1,0}, {0,1} } }
};


bool valid_coord(int8_t x, int8_t y){
    if ((0 <= x && x < BLOKUSDUO_BOARDSIZE)
        && ((0 <= y && y < BLOKUSDUO_BOARDSIZE))){
        return true;
    }
    return false;
}

board_t * make_board(){
    board_t * ret;
    ret = malloc(sizeof(board_t));
    
    memset(&ret->occupied[0], 0, sizeof(uint32_t) * BLOKUSDUO_BOARDSIZE);
    memset(&ret->cellcolors[0], 0, sizeof(uint32_t) * BLOKUSDUO_BOARDSIZE);
    memset(&ret->pieces_black[0], 0, sizeof(piece_info_t *) * BLOKUSDUO_PIECENUM);
    memset(&ret->pieces_white[0], 0, sizeof(piece_info_t *) * BLOKUSDUO_PIECENUM);

    return ret;
}

bool occupied(board_t * board, int8_t x, int8_t y){
    uint32_t mask;
    
    if (!valid_coord(x,y)){
        return false;
    }
    
    mask = 1 << x;
    if (board->occupied[y] & mask){
        return true;
    }

    return false;
}

player_t occupied_by(board_t * board, int8_t x, int8_t y){
    if (!valid_coord(x,y)){
        return PLAYER_NONE;
    }

    if (!occupied(board, x, y)){
        return PLAYER_NONE;
    }

    uint32_t mask;
    mask = 1 << x;
    if (board->cellcolors[y] & mask){
        return PLAYER_BLACK;
    }
    return PLAYER_WHITE;
}

void transform(piece_rot_t rot, bool reversed, int8_t * dx, int8_t * dy){
    int8_t tmp;
    switch(rot){
    case ROTATE_NONE:
        break;
    case ROTATE_90:
        tmp = *dy;
        *dy = *dx;
        *dx = - tmp;
        break;
    case ROTATE_180:
        *dx = - *dx;
        *dy = - *dy;
        break;
    case ROTATE_270:
        tmp = *dy;
        *dy = - *dx;
        *dx = tmp;
        break;
    }

    if (reversed){
        *dx = - *dx;
    }
}

bool validate_move_no_conflict(board_t * board, piece_info_t * move){
    piece_datum_t pd;
    uintptr_t i;

    if (!valid_coord(move->x, move->y)){
        return false;
    }
    
    pd = piece_data[move->type];
    for (i = 0;i < pd.num - 1;i++){
        int8_t x,y;
        x = pd.diff_extras[i].dx;
        y = pd.diff_extras[i].dy;
        transform(move->rot, move->reversed, &x, &y);
        x += move->x;
        y += move->y;

        if (!valid_coord(x, y) || !occupied(board, x, y)){
            return false;
        }
    }

    return true;
}

bool validate_move_check_corner_point(board_t * board
                                      , player_t player, int8_t x, int8_t y){
    piece_cell_t required_cells[4] = {{x+1,y+1}, {x+1,y-1}, {x-1,y+1}, {x-1,y-1}};
    piece_cell_t NG_cells[4] = {{x+1,y}, {x,y+1}, {x-1,y}, {x,y-1}};
    uintptr_t i;
    for (i = 0;i < 4;i++){
        piece_cell_t * cell = &NG_cells[i];
        if (occupied(board, cell->dx, cell->dy)
            && player == occupied_by(board, cell->dx, cell->dy)){
            return false;
        }
    }

    for (i = 0;i < 4;i++){
        piece_cell_t * cell = &required_cells[i];
        if (occupied(board, cell->dx, cell->dy)
            && player == occupied_by(board, cell->dx, cell->dy)){
            return true;
        }
    }

    return false;
}

bool validate_move_check_corner(board_t * board, piece_info_t * move){
    piece_datum_t pd;
    uintptr_t i;
    
    pd = piece_data[move->type];
    for (i = 0;i < pd.num - 1;i++){
        int8_t x,y;
        x = pd.diff_extras[i].dx;
        y = pd.diff_extras[i].dy;
        transform(move->rot, move->reversed, &x, &y);
        x += move->x;
        y += move->y;

        if (!validate_move_check_corner_point(board
                                              , move->player
                                              , move->x
                                              , move->y)){
            return false;
        }
    }

    return true;
}

bool validate_move(board_t * board, piece_info_t * move){
    if (validate_move_no_conflict(board, move)
        && validate_move_check_corner(board, move)){
        return true;
    }

    return false;
}

piece_info_t * make_valid_moves(board_t * board, player_t player, uint16_t * retsize){
    uintptr_t i;
    piece_info_t * ret;
    uint16_t _retsize;
    _retsize = 0;
    ret = malloc(sizeof(piece_info_t));

    piece_info_t ** pieces;
    switch (player){
    case PLAYER_NONE:
        free(ret);
        return false;
        break;
    case PLAYER_BLACK:
        pieces = board->pieces_black;
        break;
    case PLAYER_WHITE:
        pieces = board->pieces_white;
        break;
    }

    for (i = 0;i < BLOKUSDUO_PIECENUM;i++){
        if (pieces[i] != NULL){ // already used piece
            continue;
        }

        piece_info_t move_cand;
        move_cand.player = player;
        move_cand.type = i;
        
        uintptr_t x,y,rot,rev;
        for(x = 0;BLOKUSDUO_BOARDSIZE;x++){
            for(y = 0;BLOKUSDUO_BOARDSIZE;y++){
                for(rot = 0;rot < 4; rot++){
                    for(rev = 0;rev < 2;rev++){
                        move_cand.x = x;
                        move_cand.y = y;
                        move_cand.reversed = rev;
                        move_cand.rot = rot;

                        if (validate_move(board, &move_cand)){
                            memcpy(ret + sizeof(piece_info_t) * _retsize
                                   , &move_cand, sizeof(piece_info_t));
                            ret = realloc(ret, sizeof(piece_info_t) * (_retsize + 1));
                            _retsize++;
                        }
                    }
                }
            }
        }
    }

    *retsize = _retsize;

    return ret;
}



