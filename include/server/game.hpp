#ifndef GAME_HPP_
#define GAME_HPP_
#include "common.hpp"
#include "tank.hpp"

extern sf::Mutex gMutex;
extern std::vector<Tank*> pool, gpool;
extern std::vector<Shell> shells;
extern std::vector<Obs> obs;
extern bool ingame, running;

void run(void*);
bool startGame();
void sendClientData();
void sendObsData();
void doTanks();
void doShells();
void checkWon();

#endif
