// ZobristHash.h

/*
    A behind-the-scenes special hash to count how many times a board position has occured before.
    Required for 3-fold repetition game over condition.
    Index is a game state.
*/

#ifndef ZOBRIST_HASH_H
#define ZOBRIST_HASH_H

#include "TypesAndEnums.h"

#include <bitset>
#include <random>
#include <unordered_map>

class Move;

constexpr unsigned char NUM_ZOBRIST_PIECES   = 13; // For Zobrist hashing purposes only.
constexpr unsigned char NUM_ZOBRIST_SQUARES  = 64;
constexpr unsigned char WHITE_PAWN_INDEX     = 0x0;
constexpr unsigned char WHITE_ROOK_INDEX     = 0x1;
constexpr unsigned char WHITE_KNIGHT_INDEX   = 0x2;
constexpr unsigned char WHITE_BISHOP_INDEX   = 0x3;
constexpr unsigned char WHITE_KING_INDEX     = 0x4;
constexpr unsigned char WHITE_QUEEN_INDEX    = 0x5;
constexpr unsigned char BLACK_PAWN_INDEX     = 0x6;
constexpr unsigned char BLACK_ROOK_INDEX     = 0x7;
constexpr unsigned char BLACK_KNIGHT_INDEX   = 0x8;
constexpr unsigned char BLACK_BISHOP_INDEX   = 0x9;
constexpr unsigned char BLACK_KING_INDEX     = 0xA;
constexpr unsigned char BLACK_QUEEN_INDEX    = 0xB;
constexpr unsigned char BLACK_TO_MOVE        = 0xC;

class ZobristHash {
private:
    unsigned long long zobristTable[NUM_ZOBRIST_PIECES][NUM_ZOBRIST_SQUARES];  // Look-up table to get 64 bit code for a piece of a color on a square.
    unsigned long long currentPostitionHashKey;
    std::unordered_map<unsigned long long, int> positionFrequencyHash;
    static unsigned long long getRandom64Bits();
    void populateZobristLut();
    void initializeHashWithStartingPosition();
    constexpr unsigned char getZobristIndexOfPiece(Colour, PieceType);
public:
    ZobristHash();
    ~ZobristHash() = default;
    unsigned long long getCurrentPositionHashKey() const;
    void hashZobristPiece(Colour, PieceType, int square);
    int getNumberOfTimesPositionOccured(unsigned long long) const;
    void incrementHashAtCurrentPosition();
    void decrementHashAtCurrentPosition();
};

#endif