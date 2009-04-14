
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

piece_cell_t * piece_info_points(piece_info_t * piece_info, uintptr_t * retsz){
    piece_datum_t * pd;
    int8_t _x, _y;
    uintptr_t i;
    piece_cell_t * ret;

    pd = &piece_data[piece_info->type];
    *retsz = pd->num;
    ret = malloc(pd->num * sizeof(piece_cell_t));

    ret[0].x = piece_info->x;
    ret[0].y = piece_info->y;
    
    for(i = 0;i < pd->num - 1;i++){
        _x = pd->diff_extras[i].x;
        _y = pd->diff_extras[i].y;
        transform(piece_info->rot, piece_info->reversed, &_x, &_y);
        _x += piece_info->x;
        _y += piece_info->y;
        ret[i + 1].x = _x;
        ret[i + 1].y = _y;
    }

    return ret;
}

piece_type_t * available_piece_types(board_t * board, role_t role, uintptr_t * retsz){
    *retsz = 0;
    uintptr_t i;
    piece_type_t * ret;
    piece_info_t ** pieces;
    ret = NULL;

    if (role == ROLE_BLACK){
        pieces = board->pieces_black;
    } else if (role == ROLE_WHITE){
        pieces = board->pieces_white;
    } else {
        return NULL;
    }

    for (i = 0;i < BLOKUSDUO_PIECENUM;i++){
        if (pieces[i] == NULL){
            (*retsz)++;
            ret = realloc(ret, sizeof(piece_type_t) * (*retsz));
            ret[(*retsz) - 1] = i;
        }
    }

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
    int8_t _x, _y;
    uintptr_t i;

    if (x == move->x && y == move->y){
        return true;
    }

    piece_cell_t * cells;
    uintptr_t sz;
    cells = piece_info_points(move, &sz);

    for (i = 1;i < sz;i++){
        _x = cells[i].x;
        _y = cells[i].y;

        if (x == _x && y == _y){
            return true;
        }
    }
    free(cells);

    return false;
}

bool validate_move_inside_board(board_t * board, piece_info_t * move){
    uintptr_t i;

    if (!valid_coord(move->x, move->y)){
        return false;
    }

    piece_cell_t * cells;
    uintptr_t sz;
    cells = piece_info_points(move, &sz);

    for (i = 1;i < sz;i++){
        int8_t x,y;
        x = cells[i].x;
        y = cells[i].y;

        if (!valid_coord(x, y)){
            return false;
        }
    }
    free(cells);

    return true;
}

bool validate_move_no_conflict(board_t * board, piece_info_t * move){
    uintptr_t i;

    if (!valid_coord(move->x, move->y)){
        return false;
    }

    piece_cell_t * cells;
    uintptr_t sz;
    cells = piece_info_points(move, &sz);

    for (i = 1;i < sz;i++){
        int8_t x,y;
        x = cells[i].x;
        y = cells[i].y;

        if (!valid_coord(x, y) || occupied(board, x, y)){
            return false;
        }
    }
    free(cells);

    return true;
}

bool validate_move_check_no_cross_edge(board_t * board
                                      , role_t role, int8_t x, int8_t y){
    piece_cell_t NG_cells[4] = {{x+1,y}, {x,y+1}, {x-1,y}, {x,y-1}};
    uintptr_t i;

    if (occupied(board, x, y)){
        return false;
    }
    
    for (i = 0;i < 4;i++){
        piece_cell_t * cell = &NG_cells[i];
        if (occupied(board, cell->x, cell->y)
            && role == occupied_by(board, cell->x, cell->y)){
            return false;
        }
    }

    return true;
}

bool validate_move_check_cross_corner(board_t * board
                                      , role_t role, int8_t x, int8_t y){
    piece_cell_t required_cells[4] = {{x+1,y+1}, {x+1,y-1}, {x-1,y+1}, {x-1,y-1}};
    uintptr_t i;

    for (i = 0;i < 4;i++){
        piece_cell_t * cell = &required_cells[i];
        if (occupied(board, cell->x, cell->y)
            && role == occupied_by(board, cell->x, cell->y)){
            return true;
        }
    }

    return false;
}

bool validate_move_check_corner(board_t * board, piece_info_t * move){
    uintptr_t i;

    piece_cell_t * cells;
    uintptr_t sz;
    cells = piece_info_points(move, &sz);
    
    for (i = 0;i < sz;i++){
        int8_t x,y;
        x = cells[i].x;
        y = cells[i].y;

        if (!validate_move_check_no_cross_edge(board
                                              , move->role
                                              , x
                                              , y)){
            return false;
        }
    }

    for (i = 0;i < sz;i++){
        int8_t x,y;
        x = cells[i].x;
        y = cells[i].y;

        if (validate_move_check_cross_corner(board
                                             , move->role
                                             , x
                                             , y)){
            return true;
        }
    }
    
    free(cells);

    return false;
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

void adjust_move_inside(board_t * board, piece_info_t * move){
    uintptr_t i;
    piece_cell_t * cells;
    uintptr_t sz;
    cells = piece_info_points(move, &sz);

    if(move->x < 2){
        int8_t min_x = 0;
        for (i = 0;i < sz;i++){
            if (cells[i].x < min_x){
                min_x = cells[i].x;
            }
        }
        if (min_x < 0){
            move->x = (move->x - min_x);
        }
    } else if (move->x > (BLOKUSDUO_BOARDSIZE - 3)){
        int8_t max_x = 0;
        for (i = 0;i < sz;i++){
            if (cells[i].x > max_x){
                max_x = cells[i].x;
            }
        }
        if (max_x >= BLOKUSDUO_BOARDSIZE){
            move->x = BLOKUSDUO_BOARDSIZE - (max_x - move->x) - 1;
        }
    }

    if(move->y < 2){
        int8_t min_y = 0;
        for (i = 0;i < sz;i++){
            if (cells[i].y < min_y){
                min_y = cells[i].y;
            }
        }
        if (min_y < 0){
            move->y = (move->y - min_y);
        }
    } else if (move->y > (BLOKUSDUO_BOARDSIZE - 3)){
        int8_t max_y = 0;
        for (i = 0;i < sz;i++){
            if (cells[i].y > max_y){
                max_y = cells[i].y;
            }
        }
        if (max_y >= BLOKUSDUO_BOARDSIZE){
            move->y = BLOKUSDUO_BOARDSIZE - (max_y - move->y) - 1;
        }
    }

    free(cells);
}

uintptr_t count_valid_moves(board_t * board, role_t role){
    uintptr_t ret;
    free(make_valid_moves(board, role, &ret));
    return ret;
}

bool check_finished(board_t * board){
    uintptr_t black_valid_moves, white_valid_moves;
    black_valid_moves = count_valid_moves(board, ROLE_BLACK);
    white_valid_moves = count_valid_moves(board, ROLE_WHITE);

    if (black_valid_moves == 0
        && white_valid_moves == 0){
        return true;
    }
    return false;
}

piece_info_t * make_valid_moves(board_t * board, role_t role, uintptr_t * retsize){
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
        if (pieces[i] != NULL){ // alreay used piece
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

    piece_cell_t * cells;
    uintptr_t sz;
    cells = piece_info_points(move, &sz);

    for (i = 1;i < sz;i++){
        x = cells[i].x;
        y = cells[i].y;

        mask = 1 << x;
        color_mask = move->role << x;
        board->occupied[y] |= mask;
        board->cellcolors[y] |= color_mask;
    }
    free(cells);

    (*piecenum)++;
    return true;
}

turn_t * decode_turn(uint32_t turn_code, role_t role){
    turn_t * ret;
    ret = malloc(sizeof(turn_t));
    memset(ret, 0, sizeof(turn_t));
    ret->pass = false;
    
    if (turn_code == 0){
        // pass
        ret->pass = true;
    }

    uint8_t bytes[4];
    int8_t x, y;
    piece_type_t type;
    bool reversed;
    piece_rot_t rot;
    reversed = false;

    uintptr_t i;
    for(i = 0;i < 4;i++){
        bytes[i] = turn_code & 0xff; // (turn_code & (0xff << (i * 4))) >> (i * 4);
        turn_code >>= 8;
    }
    
    x = bytes[0];
    y = bytes[1];
    type = bytes[2];
    rot = bytes[3];
    if (rot > ROTATE_270){
        rot -= 4;
        reversed = true;
    }

    ret->move.x = x;
    ret->move.y = y;
    ret->move.reversed = reversed;
    ret->move.type = type;
    ret->move.rot = rot;
    ret->move.role = role;

    return ret;
}

uint32_t encode_turn(turn_t * turn){
    uint32_t ret;
    uint8_t byte;
    ret = 0;

    if (turn->pass){
        return 0;
    }

    byte = turn->move.rot;
    if (turn->move.reversed){
        byte += 4;
    }
    ret |= byte;

    ret <<= 8;
    ret |= turn->move.type;

    ret <<= 8;
    ret |= turn->move.y;

    ret <<= 8;
    ret |= turn->move.x;

    return ret;
}
