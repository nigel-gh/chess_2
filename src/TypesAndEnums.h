//TypesAndEnums.h

/*
    A collection of misc. functions and constants that shoould probably go somewhere else.
*/

#ifndef TYPES_AND_ENUMS
#define TYPES_AND_ENUMS

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <list>

constexpr int NUM_SQUARES_ON_BOARD              = 64;

constexpr int WHITE_BACK_RANK_START_SQUARE      = 56;
constexpr int WHITE_PAWN_RANK_START_SQUARE      = 48;
constexpr int WHITE_PAWN_RANK_END_SQUARE        = 55;

constexpr int BLACK_BACK_RANK_START_SQUARE      = 0;
constexpr int BLACK_PAWN_RANK_START_SQUARE      = 8;
constexpr int BLACK_PAWN_RANK_END_SQUARE        = 15;

constexpr int WHITE_LEFT_ROOK_START_SQUARE      = 56;
constexpr int WHITE_LEFT_KNIGHT_START_SQUARE    = 57;
constexpr int WHITE_LEFT_BISHOP_START_SQUARE    = 58;
constexpr int WHITE_QUEEN_START_SQUARE          = 59;
constexpr int WHITE_KING_START_SQUARE           = 60;
constexpr int WHITE_RIGHT_BISHOP_START_SQUARE   = 61;
constexpr int WHITE_RIGHT_KNIGHT_START_SQUARE   = 62;
constexpr int WHITE_RIGHT_ROOK_START_SQUARE     = 63;

constexpr int BLACK_LEFT_ROOK_START_SQUARE      = 0;
constexpr int BLACK_LEFT_KNIGHT_START_SQUARE    = 1;
constexpr int BLACK_LEFT_BISHOP_START_SQUARE    = 2;
constexpr int BLACK_QUEEN_START_SQUARE          = 3;
constexpr int BLACK_KING_START_SQUARE           = 4;
constexpr int BLACK_RIGHT_BISHOP_START_SQUARE   = 5;
constexpr int BLACK_RIGHT_KNIGHT_START_SQUARE   = 6;
constexpr int BLACK_RIGHT_ROOK_START_SQUARE     = 7;

constexpr int BOARD_UP_OFFSET                   = -8;
constexpr int BOARD_DOWN_OFFSET                 =  8;
constexpr int BOARD_LEFT_OFFSET                 = -1;
constexpr int BOARD_RIGHT_OFFSET                =  1;
constexpr int BOARD_UP_LEFT_OFFSET              = -9;
constexpr int BOARD_DOWN_RIGHT_OFFSET           =  9;
constexpr int BOARD_UP_RIGHT_OFFSET             = -7;
constexpr int BOARD_DOWN_LEFT_OFFSET            =  7;

constexpr int RESET_ENPASSANT_SQUARE            = -1;
constexpr unsigned int MAX_NUM_MOVES_IN_GAME    = 10000;
constexpr unsigned int NUM_PLY_FOR_50_MOVE_RULE = 100;

// Piece and Move things
class Piece;
class Move;

typedef int KING_POS;

// Colour
enum Colour {
    BLACK       = -1,
    NO_COLOUR   =  0,
    WHITE       =  1
};
std::string colourToString(Colour);
constexpr Colour getOppositeColour(Colour clr) {
    if (clr == WHITE) {
        return BLACK;
    } else if (clr == BLACK) {
        return WHITE;
    } else {
        return NO_COLOUR;
    }
}
bool isOppositeColour(Colour clr1, Colour clr2);

// PieceType
constexpr int NUM_PIECE_TYPES = 7;
enum PieceType {
    PAWN    = 0,
    ROOK    = 1,
    KNIGHT  = 2,
    BISHOP  = 3,
    KING    = 4,
    QUEEN   = 5,
    NULL_PIECE = 6
};
std::string pieceTypeToString(PieceType);

constexpr int PIECE_VALUES[7] = {1, 5, 3, 3, 999, 9, 0};

// GameOverReason and GameStatus
enum GameOverReason {
    CHECKMATE,
    STALEMATE,
    RESIGNATION,
    AGREED_DRAW,
    THREE_FOLD_REPETITION,
    FIFTY_MOVE_RULE,
    DEAD_POSITION,
    IN_PROGRESS,
    NULL_END_GAME_STATE
};
struct GameStatus {
    bool            inProgress;
    GameOverReason  gameOverReason;
    Colour          winningPlayer;
    bool            drawIsOffered;
    bool            playerActionIsValid;
    
    bool didGameFinishByWin();
    std::string gameOverReasonToString();
    std::string getWinningPlayerName();
};

const std::string squareToString(const int);
std::string boolToString(bool);

#endif