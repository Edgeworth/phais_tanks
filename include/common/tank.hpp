#ifndef TANK_HPP_
#define TANK_HPP_
#include "common.hpp"

struct Obs {
	Obs(const sf::SocketTCP& s, const std::string& i) : sock(s), ip(i), valid(true) {}
	sf::SocketTCP sock;
	std::string ip;
	bool valid;
};

struct Tank : public Obs {
	Tank(const Obs& o) : Obs(o) {}
	void init() {
		x = rand()%2001-1000;
		y = rand()%2001-1000;
		hp = 100;
		stam = 1;
	}

	int x, y, hp, stam;
	std::string name;
	int r, g, b, id;
};

struct Shell {
	Shell(int _sx, int _sy, int _x, int _y, int _st, int _t)
		: sx(_sx), sy(_sy), x(_x), y(_y), st(_st), t(_t) {}
	int sx, sy, x, y, st, t;
};

#endif
