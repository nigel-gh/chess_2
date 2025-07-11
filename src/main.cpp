// main.cpp

#include "Game.h"
#include "Players/Player.h"
#include "Players/DummyPlayer.h"
#include "Players/RandomPlayer.h"
#include "Players/SmartPlayers/BruteForcePlayer.h"
#include "Players/SmartPlayers/AlphaBetaPlayer.h"

int main() {

    Game game = Game(new ABPlayer(WHITE, 4), new RandomPlayer(BLACK));
    game.play();
    
    return 0;

}