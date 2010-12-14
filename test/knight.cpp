#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <queue>
#include "knights.h"

const int MAX_P = 55, STAM = 3;
int P;

struct Arrow {
	Arrow(int _x, int _y, int _t) : x(_x), y(_y), t(_t) {}
	int x, y, t;
	bool operator<(const Arrow& a) const {
		return t < a.t;
	}
};

struct Player {
	Player(int _x = 0, int _y = 0, int _hp = 0, int _stam = 0, int _vx = 0, int _vy = 0) : x(_x), y(_y), hp(_hp), stam(_stam), vx(_vx), vy(_vy) {}
	int x, y, hp, stam, vx, vy;
} players[55];

struct Move {
	Move(int _x = 0, int _y = 0, int _score = 0) : x(_x), y(_y), score(_score) {}
	int x, y, score;
};
std::vector<Arrow> arrows;

int dist(int x0, int y0, int x1, int y1) {
	return ceil(sqrt((x0-x1)*(x0-x1) + (y0-y1)*(y0-y1)));
}

void client_name(void) {
  set_name("Best Korean");
  set_colour(143, 0, 255);
}

void client_init(int NP) {
	srand(time(NULL));
	P = NP;
}

Move avoid() {
	int dx[4] = {-100, 100, 0, 0}, dy[4] = {0, 0, -100, 100};
	std::sort(arrows.begin(), arrows.end());

	int max = -1;
	Move mmax(dx[rand()%4], dy[rand()%4]);
	for (int d = 0; d < 4; ++d) {
		int nx = players[0].x+dx[d], ny = players[0].y+dy[d];
		if (nx >= 1000 || nx <= -1000 || ny >= 1000 || ny <= -1000) continue;
		int score = 0;
		for (; score < arrows.size(); ++score)
			if (arrows[score].t > 0 && dist(arrows[score].x, arrows[score].y, nx, ny) <= 100) break;
		if (score > max) {
			max = score;
			mmax = Move(nx-players[0].x, ny-players[0].y, score);
		}
	}
	return mmax;
}


void client_your_turn() {
	int mx = 0, my = 0;
	for (int i = 0; i < arrows.size(); ++i) {
		if (dist(arrows[i].x, arrows[i].y, players[0].x, players[0].y) <= 100) {
			Move m = avoid();
			mx = m.x; my = m.y;
			break;
		}
	}
	for (int i = 1; i <= P; ++i) {
		if ((players[i].hp > 0 && players[i].stam < STAM) || i == P) {
			int shoot = 1;
			if ((mx || my) && players[0].stam <= 1) shoot = 0;
			double vadj = dist(players[0].x, players[0].y, players[i].x, players[i].y)/400.0;
			int sx = players[i].x+(rand()%100 - 50)+int(vadj*players[i].vx), sy = players[i].y+(rand()%100 - 50)+int(vadj*players[i].vy);
		 	printf("player:%d sx:%d sy:%d\n", i, sx, sy);
		 	move(mx, my, sx, sy, shoot);
		 	break;
		 }
	}
	for (int i = 0; i < P; ++i)
		players[i].hp = 0;

	for (std::vector<Arrow>::iterator i = arrows.begin(); i != arrows.end();) {
		(i->t)--;
		if (i->t == 0)
			i = arrows.erase(i);
		else ++i;
	}
}

void client_tank_location(int id, int x, int y, int hp, int stamina) {
	printf("Player %d: x:%d y:%d hp:%d\n", id, x, y, hp);
  players[id] = Player(x, y, hp, stamina, x-players[id].x, y-players[id].y);
}

void client_shell_launched(int x, int y, int t) {
	arrows.push_back(Arrow(x, y, t));
}

