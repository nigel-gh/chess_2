// PieceAccessibleSquares.h

#ifndef PIECE_ACCESSIBLE_SQUARES_H
#define PIECE_ACCESSIBLE_SQUARES_H

#include <array>
#include "../TypesAndEnums.h"

// North, East, South, West ::: Rook and Queen
// NE, SE, SW, NW ::: Bishop and Queen
// Knight
// Pawn

/*
---------------------------------
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
---------------------------------
| 8 | 9 | 10| 11| 12| 13| 14| 15|
---------------------------------
| 16| 17| 18| 19| 20| 21| 22| 23|
---------------------------------
| 24| 25| 26| 27| 28| 29| 30| 31|
---------------------------------
| 32| 33| 34| 35| 36| 37| 38| 39|
---------------------------------
| 40| 41| 42| 43| 44| 45| 46| 47|
---------------------------------
| 48| 49| 50| 51| 52| 53| 54| 55|
---------------------------------
| 56| 57| 58| 59| 60| 61| 62| 63|
---------------------------------
*/

constexpr int MAX_PIECE_MOVES = 27;

struct OnBoardMoves {
    std::array<int, MAX_PIECE_MOVES> moves{};
    std::size_t                      count = 0;
    constexpr   OnBoardMoves() = default;
};

constexpr OnBoardMoves getRookAccessibleSquaresNorthFrom(int);
constexpr OnBoardMoves getRookAccessibleSquaresEastFrom(int);
constexpr OnBoardMoves getRookAccessibleSquaresSouthFrom(int);
constexpr OnBoardMoves getRookAccessibleSquaresWestFrom(int);

constexpr OnBoardMoves getBishopAccessibleSquaresNEFrom(int);
constexpr OnBoardMoves getBishopAccessibleSquaresSEFrom(int);
constexpr OnBoardMoves getBishopAccessibleSquaresSWFrom(int);
constexpr OnBoardMoves getBishopAccessibleSquaresNWFrom(int);

constexpr OnBoardMoves getKnightAccessibleSquaresFrom(int);
constexpr OnBoardMoves getPawnAccessibleSquaresFrom(int, Colour); // for check purposes, not generally pawn moves.
constexpr OnBoardMoves getKingAccessibleSquaresFrom(int);


// ROOK BEGIN //

constexpr OnBoardMoves getRookAccessibleSquaresNorthFrom(int square) {

    OnBoardMoves result{};

    int row = square / 8;
    int col = square % 8;

    for (int dist = 1; dist < 8; dist++) {
        if (row - dist >= 0)        // north
            result.moves[result.count++] = (row - dist) * 8 + col;
    }

    return result;
}

constexpr OnBoardMoves getRookAccessibleSquaresEastFrom(int square) {

    OnBoardMoves result{};

    int row = square / 8;
    int col = square % 8;

    for (int dist = 1; dist < 8; dist++) {
        if (col + dist < 8)         // east?
            result.moves[result.count++] = row * 8 + (col + dist);
    }

    return result;
}

constexpr OnBoardMoves getRookAccessibleSquaresSouthFrom(int square) {
 
    OnBoardMoves result{};

    int row = square / 8;
    int col = square % 8;

    for (int dist = 1; dist < 8; dist++) {
        if (row + dist < 8)         // south
            result.moves[result.count++] = (row + dist) * 8 + col;
    }

    return result;
}

constexpr OnBoardMoves getRookAccessibleSquaresWestFrom(int square) {

    OnBoardMoves result{};

    int row = square / 8;
    int col = square % 8;

    for (int dist = 1; dist < 8; dist++) {
        if (col - dist >= 0)        // west
            result.moves[result.count++] = row * 8 + (col - dist);
    }

    return result;
}

// ROOK END - BISHOP BEGIN //

constexpr OnBoardMoves getBishopAccessibleSquaresNEFrom(int square) {
    
    OnBoardMoves result{};

    int row = square / 8;
    int col = square % 8;

    for (int dist = 1; dist < 8; ++dist) {
        if (row - dist >= 0 && col + dist < 8)                                  // Up-right
            result.moves[result.count++] = (row - dist) * 8 + (col + dist);
    }

    return result;
}

constexpr OnBoardMoves getBishopAccessibleSquaresSEFrom(int square) {
    
    OnBoardMoves result{};

    int row = square / 8;
    int col = square % 8;

    for (int dist = 1; dist < 8; ++dist) {
        if (row + dist < 8 && col + dist < 8)                                   // Down-right
            result.moves[result.count++] = (row + dist) * 8 + (col + dist);
    }

    return result;
}

constexpr OnBoardMoves getBishopAccessibleSquaresSWFrom(int square) {
    
    OnBoardMoves result{};

    int row = square / 8;
    int col = square % 8;

    for (int dist = 1; dist < 8; ++dist) {
        if (row + dist < 8 && col - dist >= 0)                                  // Down-left
            result.moves[result.count++] = (row + dist) * 8 + (col - dist); 
    }

    return result;
}
constexpr OnBoardMoves getBishopAccessibleSquaresNWFrom(int square) {
    
    OnBoardMoves result{};

    int row = square / 8;
    int col = square % 8;

    for (int dist = 1; dist < 8; ++dist) {
        if (row - dist >= 0 && col - dist >= 0)                                 // Up-left
            result.moves[result.count++] = (row - dist) * 8 + (col - dist);
    }

    return result;
}

// BISHOP END - KNIGHT BEGIN //

constexpr OnBoardMoves getKnightAccessibleSquaresFrom(int square) {
    
    OnBoardMoves result{};

    int row = square / 8;
    int col = square % 8;

    constexpr std::array<std::pair<int, int>, 8> offsets = {
        {
            {-2, -1}, {-2, +1},
            {-1, -2}, {-1, +2},
            {+1, -2}, {+1, +2},
            {+2, -1}, {+2, +1}
        }
    };

    for (auto [dr, dc] : offsets) {
        int r = row + dr;
        int c = col + dc;
        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            result.moves[result.count++] = r * 8 + c;
        }
    }

    return result;
}

// KNIGHT END - PAWN BEGIN //

constexpr OnBoardMoves getPawnAccessibleSquaresFrom(int index, Colour kingClr) {
    
    OnBoardMoves result{};

    int row = index / 8;
    int col = index % 8;

    if (kingClr == WHITE) {
        if (row > 0 && col > 0)
            result.moves[result.count++] = (row - 1) * 8 + (col - 1);
        if (row > 0 && col < 8 - 1)
            result.moves[result.count++] = (row - 1) * 8 + (col + 1);
    } else {
        if (row < 8 - 1 && col > 0)
            result.moves[result.count++] = (row + 1) * 8 + (col - 1);
        if (row < 8 - 1 && col < 8 - 1)
            result.moves[result.count++] = (row + 1) * 8 + (col + 1);
    }

    return result;
}

constexpr OnBoardMoves getKingAccessibleSquaresFrom(int index) {
        
    OnBoardMoves result{};

    bool upIsOnBoard        = index >= 8;
    bool downIsOnBoard      = index < 56;
    bool leftIsOnBoard      = (index % 8) != 0;
    bool rightIsOnBoard     = (index % 8) != 7;

    if (upIsOnBoard) {
 
        result.moves[result.count++] = index + BOARD_UP_OFFSET;
        
        if (leftIsOnBoard) {
            result.moves[result.count++] = index + BOARD_UP_LEFT_OFFSET;
        }
        
        if (rightIsOnBoard) {
            result.moves[result.count++] = index + BOARD_UP_RIGHT_OFFSET;
        }
        
    }
    
    if (downIsOnBoard) {

        result.moves[result.count++] = index + BOARD_DOWN_OFFSET;
        
        if (leftIsOnBoard) {
            result.moves[result.count++] = index + BOARD_DOWN_LEFT_OFFSET;
        }
        
        if (rightIsOnBoard) {
            result.moves[result.count++] = index + BOARD_DOWN_RIGHT_OFFSET;
        }
        
    }
    
    if (leftIsOnBoard) {

        result.moves[result.count++] = index + BOARD_LEFT_OFFSET;
        
    }
    
    if (rightIsOnBoard) {

        result.moves[result.count++] = index + BOARD_RIGHT_OFFSET;

    }

    return result;
    
}

#endif