// RandomPlayer.cpp

#include "Player.h"
#include "RandomPlayer.h"
#include "../Move.h"

#include <iostream>
#include <stdlib.h>
#include <random>
#include <iterator>

RandomPlayer::RandomPlayer(Colour _colour) : Player(_colour) {}

// Make a move at random (only move types, never draw or resign!)
const Action RandomPlayer::chooseAction(const MoveSet* possibleMoves, const Board* board) const {
    
    std::string playerName = getColour() == WHITE ? "white" : "black";
    const Move* chosenMove;
    ActionType actionType = MOVE;
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    size_t numMovesAvailable = possibleMoves->size();

    std::uniform_int_distribution<> dist(0, numMovesAvailable - 1);
    if (numMovesAvailable < 1) {
        throw std::out_of_range(std::string("RandomPlayer::choseAction() possilbe moves too small maybe? size is " + std::to_string(numMovesAvailable)));
    }
    int randomIndex = dist(gen);

    chosenMove = possibleMoves->getMoveClone(randomIndex);

    return Action(actionType, chosenMove);
}

Player* RandomPlayer::clone() const {
    return new RandomPlayer(getColour());
}