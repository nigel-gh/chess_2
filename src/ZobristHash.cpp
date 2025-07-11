#include "ZobristHash.h"
#include "Move.h"
#include <random>
#include <chrono>
#include <iostream>


ZobristHash::ZobristHash() {
    populateZobristLut();
    initializeHashWithStartingPosition();
    positionFrequencyHash.insert({currentPostitionHashKey, 1});
}

unsigned long long ZobristHash::getCurrentPositionHashKey() const {
    return currentPostitionHashKey;
}

void ZobristHash::populateZobristLut() {
    int row, col;
    for (row = 0; row < NUM_ZOBRIST_PIECES; row++) {
        for (col = 0; col < NUM_ZOBRIST_SQUARES; col++) {
            zobristTable[row][col] = getRandom64BitsSlow();
        }
    }
}

// Slow because all numbers are created using the random_device.
// This is fine because we're only using this for initalization of the ZobHash.
// NOTE: the random_device{}() behaved randomly on linux, but deterministically on windows.
// Thank you random numbers for not being random, but being random sometimes, and confusing always.
unsigned long long ZobristHash::getRandom64BitsSlow() {

    static std::mt19937_64 gen(std::random_device{}());
    static std::uniform_int_distribution<unsigned long long> dist(
        0, std::numeric_limits<unsigned long long>::max()
    );
    unsigned long long val = dist(gen);
    std::cout << "value: \"" << val << "\"\n";
    return val;

}


void ZobristHash::hashZobristPiece(Colour colour, PieceType type, int square) {
    int zobristIndex = getZobristIndexOfPiece(colour, type);
    currentPostitionHashKey ^= zobristTable[zobristIndex][square];
}

void ZobristHash::incrementHashAtCurrentPosition() {
    if (positionFrequencyHash.find(currentPostitionHashKey) != positionFrequencyHash.end()) {
        positionFrequencyHash[currentPostitionHashKey] += 1;
    } else {
        positionFrequencyHash.insert({currentPostitionHashKey, 1});
    }
}
    
void ZobristHash::decrementHashAtCurrentPosition() {    
    positionFrequencyHash[currentPostitionHashKey] -= 1;
}

constexpr unsigned char ZobristHash::getZobristIndexOfPiece(Colour colour, PieceType type) {
    // DANGEROUS IMPLEMENTATION: RELIES ON THE ORDER OF PIECETYPE DEFENITION AND ZOBHASH ATTRIBUTES!
    unsigned char scale = colour == WHITE ? 0 : 1;
    return scale * NUM_PIECE_TYPES + type;
}

int ZobristHash::getNumberOfTimesPositionOccured(unsigned long long zobKey) const {
    auto it = positionFrequencyHash.find(zobKey);
    return it != positionFrequencyHash.end() ? it->second :  0;
}

// XOR all the pieces into the current position by finding their codes in the LUT
void ZobristHash::initializeHashWithStartingPosition() {
    currentPostitionHashKey = 0x0000'0000'0000'0000;
    currentPostitionHashKey ^= zobristTable[WHITE_PAWN_INDEX][48];
    currentPostitionHashKey ^= zobristTable[WHITE_PAWN_INDEX][49];
    currentPostitionHashKey ^= zobristTable[WHITE_PAWN_INDEX][50];
    currentPostitionHashKey ^= zobristTable[WHITE_PAWN_INDEX][51];
    currentPostitionHashKey ^= zobristTable[WHITE_PAWN_INDEX][52];
    currentPostitionHashKey ^= zobristTable[WHITE_PAWN_INDEX][53];
    currentPostitionHashKey ^= zobristTable[WHITE_PAWN_INDEX][54];
    currentPostitionHashKey ^= zobristTable[WHITE_PAWN_INDEX][55];
    currentPostitionHashKey ^= zobristTable[WHITE_ROOK_INDEX][56];
    currentPostitionHashKey ^= zobristTable[WHITE_KNIGHT_INDEX][56];
    currentPostitionHashKey ^= zobristTable[WHITE_BISHOP_INDEX][56];
    currentPostitionHashKey ^= zobristTable[WHITE_QUEEN_INDEX][56];
    currentPostitionHashKey ^= zobristTable[WHITE_KING_INDEX][56];
    currentPostitionHashKey ^= zobristTable[WHITE_BISHOP_INDEX][56];
    currentPostitionHashKey ^= zobristTable[WHITE_KNIGHT_INDEX][56];
    currentPostitionHashKey ^= zobristTable[WHITE_ROOK_INDEX][56];
    currentPostitionHashKey ^= zobristTable[BLACK_PAWN_INDEX][8];
    currentPostitionHashKey ^= zobristTable[BLACK_PAWN_INDEX][9];
    currentPostitionHashKey ^= zobristTable[BLACK_PAWN_INDEX][10];
    currentPostitionHashKey ^= zobristTable[BLACK_PAWN_INDEX][11];
    currentPostitionHashKey ^= zobristTable[BLACK_PAWN_INDEX][12];
    currentPostitionHashKey ^= zobristTable[BLACK_PAWN_INDEX][13];
    currentPostitionHashKey ^= zobristTable[BLACK_PAWN_INDEX][14];
    currentPostitionHashKey ^= zobristTable[BLACK_PAWN_INDEX][15];
    currentPostitionHashKey ^= zobristTable[BLACK_ROOK_INDEX][0];
    currentPostitionHashKey ^= zobristTable[BLACK_KNIGHT_INDEX][1];
    currentPostitionHashKey ^= zobristTable[BLACK_BISHOP_INDEX][2];
    currentPostitionHashKey ^= zobristTable[BLACK_QUEEN_INDEX][3];
    currentPostitionHashKey ^= zobristTable[BLACK_KING_INDEX][4];
    currentPostitionHashKey ^= zobristTable[BLACK_BISHOP_INDEX][5];
    currentPostitionHashKey ^= zobristTable[BLACK_KNIGHT_INDEX][6];
    currentPostitionHashKey ^= zobristTable[BLACK_ROOK_INDEX][7];
}