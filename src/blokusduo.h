#ifndef BLOKUSDUO_H
#define BLOKUSDUO_H

#include <stdint.h>
#include <stdbool.h>

#define BLOKUSDUO_BOARDSIZE 14
#define BLOKUSDUO_PIECENUM 21

/*
 * 1単位:
 *
 * a: ■
 *
 * 2単位:
 *
 * b: ■
 *    □
 *
 * 3単位:
 *
 * c: □     d: □
 *    ■        ■□
 *    □
 *
 * 4単位:
 *
 * e:  □   f:  □   g:  □     h:  ■□  i: □■
 *     ■       ■       ■□       □□       □□
 *     □     □□       □
 *     □
 *
 * 5単位:
 *
 * j:  □   k:       l:  □    m:   □   n:  □□   o:  □     p: □
 *     □       □       □       □■         ■       ■□      ■
 *     ■       □     □■       □□       □□       □      □□□
 *     □       ■     □                               □
 *     □     □□
 *
 * q:  □      r: □□    s:  □       t:  □       u:   □
 *     □           ■□      □■□       □■□      □■□
 *     ■□□         □          □         □          □
 */


typedef enum {
    PLAYER_NONE = -1,
    PLAYER_WHITE = 0,
    PLAYER_BLACK = 1,
} player_t;

// clockwise rotation
typedef enum {
    ROTATE_NONE = 0,
    ROTATE_90 = 1,
    ROTATE_180 = 2,
    ROTATE_270 = 3,
} piece_rot_t;

typedef enum {
    PIECE_A = 0, PIECE_B = 1, PIECE_C = 2,
    PIECE_D = 3, PIECE_E = 4, PIECE_F = 5,
    PIECE_G = 6, PIECE_H = 7, PIECE_I = 8,
    PIECE_J = 9, PIECE_K = 10, PIECE_L = 11,
    PIECE_M = 12, PIECE_N = 13, PIECE_O = 14,
    PIECE_P = 15, PIECE_Q = 16, PIECE_R = 17,
    PIECE_S = 18, PIECE_T = 19, PIECE_U = 20,
} piece_type_t;

typedef struct piece_info {
    int8_t x, y;
    bool reversed;
    piece_rot_t rot;
    piece_type_t type;
    player_t player;
} piece_info_t;

// 14x14 = 196
typedef struct board {
    // This is a kind of cache.
    // Every time a move occurs, it is updated.
    // If (x,y) is occupied, x-th bit of occupied[y] is set.
    uint32_t occupied[BLOKUSDUO_BOARDSIZE];

    uint32_t cellcolors[BLOKUSDUO_BOARDSIZE];
    
    // "black" = first move, "white" = opposite of "black"
    // The index of this array is based on <type of piece>
    // If, for example, pieces[16] is NULL, it means 16 = PIECE_Q is not placed yet.
    piece_info_t * pieces_black[BLOKUSDUO_PIECENUM];
    piece_info_t * pieces_white[BLOKUSDUO_PIECENUM];
} board_t;

typedef struct piece_cell {
    int8_t dx;
    int8_t dy;
} piece_cell_t;

typedef struct piece_datum {
    // num = 1 ~ 5
    int num;

    // A piece is constructed of some cells.
    // "extras" is an array of cells that central cell is excluded.
    // The length of "extras" is equivalent to "num" minus 1.
    piece_cell_t diff_extras[4];
} piece_datum_t;

/*  Coordinate
 *
 *    +-----------------> x
 *    |
 *    |
 *    |    ■ <- central cell
 *    |      □ <- x=1, y=1
 *    |      □ <- x=1, y=2
 *    |
 *    |
 *  y v
 *
 */


#endif
