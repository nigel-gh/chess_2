// Player.cpp
#include "Player.h"
#include "../Pieces/Piece.h"
#include "../Pieces/Rook.h"
#include "../Move.h"
#include <iostream>

Player::Player() {
    colour = WHITE;
    name = "NONAME";
}

Player::Player(Colour _colour) {
    colour = _colour;
    name = "NONAME";
}

Player::~Player() = default;

Colour Player::getColour() const {
    return colour;
}

std::string Player::getPlayerName() const {
    return colour == WHITE ? "white" : "black";
}

Action::Action()
    : actionType(NO_ACTION), move(nullptr) {}

Action::Action(ActionType actType, const Move* _move)
    : actionType(actType), move(_move) {}

const ActionType Action::getActionType() const {
    return actionType;
}

const Move* Action::getMove() const {
    return move;
}