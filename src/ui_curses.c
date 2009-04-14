
#include "ui_curses.h"


void ui_curses_init(){
    initscr();
    cbreak();
    noecho();
    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    curs_set(0);
    refresh();
}

void ui_curses_exit(){
    endwin();
}

inline void ui_curses_draw_ruled_line(uintptr_t y){
    uintptr_t i;
    mvaddch(y, 0, '+');
    for(i = 0;i < BLOKUSDUO_BOARDSIZE;i++){
        mvaddch(y, 3 * i + 1, '-');
        mvaddch(y, 3 * i + 2, '-');
        mvaddch(y, 3 * i + 3, '+');
    }
}

inline void ui_curses_draw_cell_line(uintptr_t y){
    uintptr_t i;
    mvaddch(y, 0, '|');
    for(i = 0;i < BLOKUSDUO_BOARDSIZE;i++){
        mvaddch(y, 3 * i + 1,' ');
        mvaddch(y, 3 * i + 2,' ');
        mvaddch(y, 3 * i + 3, '|');
    }
}

void ui_curses_draw_null_board(){
    clear();
    move(0, 0);
    uintptr_t i;

    ui_curses_draw_ruled_line(0);
    for(i = 0;i < BLOKUSDUO_BOARDSIZE;i++){
        ui_curses_draw_cell_line(2 * i + 1);
        ui_curses_draw_ruled_line(2 * i + 2);
    }
}

void ui_curses_draw_board(board_t * board){
    ui_curses_draw_null_board();

    uintptr_t x,y;
    for(x = 0;x < BLOKUSDUO_BOARDSIZE;x++){
        for(y = 0;y < BLOKUSDUO_BOARDSIZE;y++){
            if(occupied(board, x, y)){
                switch(occupied_by(board, x, y)){
                case ROLE_BLACK:
                    DRAW_POINT(y, x, BLACK_SYMBOL_CH);
                    break;
                case ROLE_WHITE:
                    DRAW_POINT(y, x, WHITE_SYMBOL_CH);
                    break;
                case ROLE_NONE:
                    DRAW_POINT(y, x, '/');
                    break;
                }
            }
        }
    }

    refresh();
}

void ui_curses_draw_info(board_t * board, role_t role){
    char * insts[] = {"Arrow keys: move a piece"
                      , "c: next piece"
                      , "r: rotate a piece"
                      , "v: reverse a piece"};

    uintptr_t i;
    for (i = 0;i < 4;i++){
        mvinsstr(5 + i, CX(BLOKUSDUO_BOARDSIZE) + 4, insts[i]);
    }

    char buf[1024];
    sprintf(buf, "%c %s's turn"
            , (role == ROLE_BLACK ? BLACK_SYMBOL_CH : WHITE_SYMBOL_CH)
            , (role == ROLE_BLACK ? "BLACK" : "WHITE"));
    mvinsstr(10, CX(BLOKUSDUO_BOARDSIZE) + 4, buf);

    sprintf(buf, "BLACK: %d remaining piece%c"
            , BLOKUSDUO_PIECENUM - board->blacknum
            , (BLOKUSDUO_PIECENUM - board->blacknum <= 1 ? ' ' : ' '));
    mvinsstr(12, CX(BLOKUSDUO_BOARDSIZE) + 4, buf);
    sprintf(buf, "WHITE: %d remaining piece%c"
            , BLOKUSDUO_PIECENUM - board->whitenum
            , (BLOKUSDUO_PIECENUM - board->whitenum <= 1 ? ' ' : ' '));
    mvinsstr(13, CX(BLOKUSDUO_BOARDSIZE) + 4, buf);
}

void ui_curses_draw_move_cand(board_t * board, piece_info_t * move_cand){
    uintptr_t i;
    piece_cell_t * cells;
    uintptr_t sz;
    bool validity;
    cells = piece_info_points(move_cand, &sz);
    validity = validate_move(board, move_cand);
    
    for (i = 0;i < sz;i++){
        int8_t x,y;
        x = cells[i].x;
        y = cells[i].y;

        if (validity == true){
            mvaddch(CY(y), CX(x), 'o');
        } else {
            mvaddch(CY(y), CX(x), 'x');
        }
    }

    refresh();
}

// return 0: pass
uint32_t ui_curses_get_move(board_t * board, role_t role){
    uint32_t ret;
    uintptr_t available_types_num;
    piece_type_t * available_types;
    uintptr_t type_idx;
    uintptr_t move_cands_num;
    turn_t turn;
    piece_info_t * move;
    int c;

    move = &turn.move;
    turn.pass = false;

    free(make_valid_moves(board, role, &move_cands_num));
    if (move_cands_num == 0){
        turn.pass = true;
        return encode_turn(&turn);
    }
    available_types = available_piece_types(board, role, &available_types_num);
    if (available_types_num == 0){
        turn.pass = true;
        return encode_turn(&turn);
    }
    type_idx = 0;
    
    move->x = move->y = BLOKUSDUO_BOARDSIZE / 2;
    move->reversed = false;
    move->rot = ROTATE_NONE;
    move->type = available_types[type_idx];
    move->role = role;
    
    ui_curses_draw_board(board);
    ui_curses_draw_move_cand(board, move);
    ui_curses_draw_info(board, role);
    
    do {
        c = wgetch(stdscr);

        switch(c){
        case KEY_UP:
            move->y--;
            if (!validate_move_inside_board(board, move)){
                move->y++;
            }
            break;
        case KEY_DOWN:
            move->y++;
            if (!validate_move_inside_board(board, move)){
                move->y--;
            }
            break;
        case KEY_LEFT:
            move->x--;
            if (!validate_move_inside_board(board, move)){
                move->x++;
            }
            break;
        case KEY_RIGHT:
            move->x++;
            if (!validate_move_inside_board(board, move)){
                move->x--;
            }
            break;
        case 'r': //rotation
            switch(move->rot){
            case ROTATE_NONE:
                move->rot = ROTATE_90;
                break;
            case ROTATE_90:
                move->rot = ROTATE_180;
                break;
            case ROTATE_180:
                move->rot = ROTATE_270;
                break;
            case ROTATE_270:
                move->rot = ROTATE_NONE;
                break;
            }
            break;
        case 'c': //chage a piece
            type_idx++;
            if (type_idx == available_types_num){
                type_idx = 0;
            }
            move->type = available_types[type_idx];
            
            break;
        case 'v': //reverse
            move->reversed ^= 1;
            break;
        case KEY_ENTER:
        case '\n':
        case '\r':
            if (validate_move(board, move)){
                ret = encode_turn(&turn);
                goto finished;
            }
            break;
        default:
            break;
        }

        if(!validate_move_inside_board(board, move)){
            adjust_move_inside(board, move);
        }

        ui_curses_update(board, role);
        ui_curses_draw_move_cand(board, move);
        refresh();
    } while(true);

finished:
    
    if (available_types_num > 0){
        free(available_types);
    }

    return ret;
}

void ui_curses_update(board_t * board, role_t role){
    ui_curses_draw_board(board);
    ui_curses_draw_info(board, role);
    refresh();
}
