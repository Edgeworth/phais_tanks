#ifndef NET_HPP_
#define NET_HPP_
#include "common.hpp"

struct Tank {
	int id, x, y, hp, stam, r, g, b;
	std::string name;
};

struct Shell {
	int sx, sy, x, y, st, t, id;
};

extern sf::Mutex copyMutex;
extern bool running;
extern std::vector<std::vector<Tank> > tanks, copyTanks;
extern std::vector<std::vector<Shell> > shells, copyShells;

void runNet(void*);

#endif
