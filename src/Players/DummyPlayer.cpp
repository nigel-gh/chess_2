// DummyPlayer.cpp
#include "Player.h"
#include "DummyPlayer.h"
#include "../Move.h"
#include <iostream>
#include <stdlib.h>

DummyPlayer::DummyPlayer(Colour _colour) : Player(_colour) {}

const Action DummyPlayer::chooseAction(const MoveSet* possibleMoves, const Board* board) const {
    
    std::string playerName = getColour() == WHITE ? "white" : "black";
    const Move* chosenMove;
    ActionType actionType = NO_ACTION;
    std::string userInput;
    
    while (actionType == NO_ACTION) {

        std::cin >> userInput;
        actionType = getActionFromString(userInput);

    }
    
    if (actionType == MOVE) {

        chosenMove = possibleMoves->getMoveClone(userInput);
        if (chosenMove == nullptr) {
            actionType = NO_ACTION;
        }

    }   
    
    return Action(actionType, chosenMove);

}

ActionType DummyPlayer::getActionFromString(std::string str) {

    // Dirty way of closing the program.

    if (str == "x") {
        #ifdef __linux__
            quick_exit(0);
        #elif _WIN32
            exit(-1);
        #else
            std::cout << "ERROR: COULD NOT FIND TARGET PLATFORM!!\n";
        #endif
    } else if (str == "undo")           {
        return UNDO;
    } else if (str == "resign")         {
        return RESIGN;
    } else {
        return MOVE;
    }
}

Player* DummyPlayer::clone() const {
    return new DummyPlayer(getColour());
}