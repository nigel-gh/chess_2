// main.cpp

#include "Game.h"
#include "Players/Player.h"
#include "Players/DummyPlayer.h"
#include "Players/RandomPlayer.h"
#include "Players/SmartPlayers/BruteForcePlayer.h"
#include "Players/SmartPlayers/AlphaBetaPlayer.h"

int main() {

    // Game game = Game(new RandomPlayer(WHITE), new ABPlayer(BLACK, 2));
    Game game = Game(new ABPlayer(WHITE, 2), new RandomPlayer(BLACK));
    game.play();
    
    return 0;

}