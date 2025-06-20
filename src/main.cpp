// main.cpp

#include "Game.h"
#include "Players/Player.h"
#include "Players/DummyPlayer.h"
#include "Players/RandomPlayer.h"
#include "Players/SmartPlayers/BruteForcePlayer.h"
#include "Players/SmartPlayers/AlphaBetaPlayer.h"


#include "CStack.h"



int main() {

    Game game = Game(new ABPlayer(WHITE, 5), new ABPlayer(BLACK, 5));
    game.play();
    return 0;

}