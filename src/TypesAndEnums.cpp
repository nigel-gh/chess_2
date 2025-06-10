//TypesAndEnums.cpp
#include "Pieces/Piece.h"
#include "TypesAndEnums.h"
#include "Move.h"
#include "Pieces/Rook.h"
#include <iostream>

const std::string squareToString(const int square) {
    if (square == -1) {
        return "NA";
    }
    char    fileStart   = 'a' + (square % 8);           // File (column)
    char    rankStart   = '0' +  8  - (square / 8);       // Rank (row)
    std::string str = {fileStart, rankStart};
    return str;
}

std::string pieceTypeToString(PieceType pieceType) {
    switch (pieceType)
    {
    case PAWN:
        return "PAWN";
    case ROOK:
        return "ROOK";
    case KNIGHT:
        return "KNIGHT";
    case BISHOP:
        return "BISHOP";
    case KING:
        return "KING";
    case QUEEN:
        return "QUEEN";
    default:
        return "UNKOWN_PIECE_TYPE";
    }
}

std::string colourToString(Colour clr) {
    switch (clr) {
        case WHITE:
            return "white";
        case BLACK:
            return "black";
        case NO_COLOUR:
            return "ERROR_NO_COLOUR";    
        default:
            return "ERROR_UNKNOWN_COLOUR";    
    }
}

bool isOppositeColour(Colour clr1, Colour clr2) {
    return clr1 * clr2 == -1;
}

bool GameStatus::didGameFinishByWin() {

    switch (gameOverReason) 
    {
        case CHECKMATE:
        return true;
        break;
        
        case STALEMATE:
        return false;
        break;
        
        case RESIGNATION:
        return true;
        break;
        
        case AGREED_DRAW:
        return false;
        break;
        
        case THREE_FOLD_REPETITION:
        return false;
        break;
        
        case FIFTY_MOVE_RULE:
        return false;
        break;
        
        case DEAD_POSITION:
        return false;
        break;
        
        case IN_PROGRESS:
        throw std::logic_error("Asked if a game finished by draw, but the game was still in progress.");
        break;
        
        case NULL_END_GAME_STATE:
        throw std::logic_error("Asked if a game finished by draw, but the game was in limbo.");
        break;
        
        default:
        throw std::logic_error("Asked if a game finished by draw, but I don't even know what the hell happened to the game - ask Steve, maybe?");
        break;
    }
}

std::string GameStatus::gameOverReasonToString() {
    switch (gameOverReason) 
    {
        case CHECKMATE:
        return "CHECKMATE";
        break;
 
        case STALEMATE:
        return "STALEMATE";
        break;
        
        case RESIGNATION:
        return "RESIGNATION";
        break;
        
        case AGREED_DRAW:
        return "AGREED DRAW";
        break;
        
        case THREE_FOLD_REPETITION:
        return "3-FOLD REPETITION";
        break;
        
        case FIFTY_MOVE_RULE:
        return "50 MOVE RULE";
        break;
        
        case DEAD_POSITION:
        return "DEAD POSITION";
        break;
        
        case IN_PROGRESS:
        throw std::logic_error("Requested game over reason, but the game was still in progress.");
        break;
        
        case NULL_END_GAME_STATE:
        throw std::logic_error("Requested game over reason, but the game was in limbo.");
        break;
        
        default:
        throw std::logic_error("Requested game over reason, but I cannot even tell what state the game is in!");
        break;
    }
}

std::string GameStatus::getWinningPlayerName() {

    if (inProgress) {
        throw std::logic_error("Requested the winning player's name, but the game is not over!");
    }

    switch (winningPlayer)
    {
        case WHITE:
        return "WHITE";
        break;
        
        case BLACK:
        return "BLACK";
        break;
        
        case NO_COLOUR:
        throw std::logic_error("Requested the winning player's name, but found \"NO_COLOUR\"");
        break;
        
        default:
        throw std::logic_error("Requested the winning player's name, but I don't even know who won (who's next? YOU DECIDEEEE)");
        break;
    }
}

std::string boolToString(bool b) {
    return b ? "TRUE" : "FALSE";
}