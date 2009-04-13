
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

    memset(ret, 0, sizeof(board_t));
    //memset(&ret->occupied[0], 0, sizeof(uint32_t) * BLOKUSDUO_BOARDSIZE);
    //memset(&ret->cellcolors[0], 0, sizeof(uint32_t) * BLOKUSDUO_BOARDSIZE);
    //memset(&ret->pieces_black[0], 0, sizeof(piece_info_t *) * BLOKUSDUO_PIECENUM);
    //memset(&ret->pieces_white[0], 0, sizeof(piece_info_t *) * BLOKUSDUO_PIECENUM);
    
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

role_t occupied_by(board_t * board, int8_t x, int8_t y){
    if (!valid_coord(x,y)){
        return ROLE_NONE;
    }

    if (!occupied(board, x, y)){
        return ROLE_NONE;
    }

    uint32_t mask;
    mask = 1 << x;
    if (board->cellcolors[y] & mask){
        return ROLE_BLACK;
    }
    return ROLE_WHITE;
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

bool move_covers(piece_info_t * move, int8_t x, int8_t y){
    piece_datum_t * pd;
    int8_t _x, _y;
    uintptr_t i;

    if (x == move->x && y == move->y){
        return true;
    }
    pd = &piece_data[move->type];
    for(i = 0;i < pd->num - 1;i++){
        _x = pd->diff_extras[i].dx;
        _y = pd->diff_extras[i].dy;
        transform(move->rot, move->reversed, &_x, &_y);
        _x += move->x;
        _y += move->y;

        if (x == _x && y == _y){
            return true;
        }
    }

    return false;
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

        if (!valid_coord(x, y) || occupied(board, x, y)){
            return false;
        }
    }

    return true;
}

bool validate_move_check_corner_point(board_t * board
                                      , role_t role, int8_t x, int8_t y){
    piece_cell_t required_cells[4] = {{x+1,y+1}, {x+1,y-1}, {x-1,y+1}, {x-1,y-1}};
    piece_cell_t NG_cells[4] = {{x+1,y}, {x,y+1}, {x-1,y}, {x,y-1}};
    uintptr_t i;
    for (i = 0;i < 4;i++){
        piece_cell_t * cell = &NG_cells[i];
        if (occupied(board, cell->dx, cell->dy)
            && role == occupied_by(board, cell->dx, cell->dy)){
            return false;
        }
    }

    for (i = 0;i < 4;i++){
        piece_cell_t * cell = &required_cells[i];
        if (occupied(board, cell->dx, cell->dy)
            && role == occupied_by(board, cell->dx, cell->dy)){
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
                                              , move->role
                                              , move->x
                                              , move->y)){
            return false;
        }
    }

    return true;
}

bool validate_move(board_t * board, piece_info_t * move){
    if (!validate_move_no_conflict(board, move)){
        return false;
    }

    if (board->blacknum == 0){
        if (board->whitenum == 0
            && move->role == ROLE_BLACK
            && move_covers(move, 4, 4)){
            return true;
        } 
        return false;
    }
    if (board->whitenum == 0){
        if (board->blacknum == 1
            && move->role == ROLE_WHITE
            && move_covers(move, 9, 9)){
            return true;
        }
        return false;
    }
    
    if (!validate_move_check_corner(board, move)){
        return false;
    }
    
    return true;
}

piece_info_t * make_valid_moves(board_t * board, role_t role, uint16_t * retsize){
    uintptr_t i;
    piece_info_t * ret;
    uint16_t _retsize;
    ret = NULL;
    _retsize = 0;

    piece_info_t ** pieces;
    switch (role){
    case ROLE_NONE:
        free(ret);
        return false;
        break;
    case ROLE_BLACK:
        pieces = board->pieces_black;
        break;
    case ROLE_WHITE:
        pieces = board->pieces_white;
        break;
    }

    for (i = 0;i < BLOKUSDUO_PIECENUM;i++){
        if (pieces[i] != NULL){ // already used piece
            continue;
        }

        piece_info_t move_cand;
        move_cand.role = role;
        move_cand.type = i;
        
        uintptr_t x,y,rot,rev;
        for(x = 0;x < BLOKUSDUO_BOARDSIZE;x++){
            for(y = 0;y < BLOKUSDUO_BOARDSIZE;y++){
                for(rot = 0;rot < 4; rot++){
                    for(rev = 0;rev < 2;rev++){
                        move_cand.x = x;
                        move_cand.y = y;
                        move_cand.reversed = rev;
                        move_cand.rot = rot;

                        if (validate_move(board, &move_cand)){
                            ret = realloc(ret, sizeof(piece_info_t) * (1 + _retsize));
                            memcpy(&ret[_retsize]
                                   , &move_cand, sizeof(piece_info_t));
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


/*
 * return true if successfully a move is done
 *
 */
bool do_move(board_t * board, piece_info_t * move){
    if (!validate_move(board, move)){
        return false;
    }
    
    piece_info_t ** pieces;
    uint8_t * piecenum;
    
    switch(move->role){
    case ROLE_BLACK:
        pieces = board->pieces_black;
        piecenum = &board->blacknum;
        break;
    case ROLE_WHITE:
        pieces = board->pieces_white;
        piecenum = &board->whitenum;
        break;
    case ROLE_NONE:
        return false;
        break;
    }

    pieces[move->type] = malloc(sizeof(piece_info_t));
    memcpy(pieces[move->type], move, sizeof(piece_info_t));

    uintptr_t i;
    uint32_t mask, color_mask;
    int8_t x,y;
    mask = 1 << move->x;
    color_mask = move->role << move->x;
    board->occupied[move->y] |= mask;
    board->cellcolors[move->y] |= color_mask;
    for(i = 0;i < piece_data[move->type].num - 1;i++){
        x = piece_data[move->type].diff_extras[i].dx;
        y = piece_data[move->type].diff_extras[i].dy;
        transform(move->rot, move->reversed, &x, &y);
        x += move->x;
        y += move->y;

        mask = 1 << x;
        color_mask = move->role << x;
        board->occupied[y] |= mask;
        board->cellcolors[y] |= color_mask;
    }

    (*piecenum)++;
    return true;
}
