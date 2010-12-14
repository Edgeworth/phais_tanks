#include "game.hpp"
#include "utility.hpp"

sf::Mutex gMutex;
std::vector<Tank*> pool, gpool;
std::vector<Obs> obs;
std::vector<Shell> shells;
bool ingame = false, running = true;
int shellId = 1;

void run(void*) {
	sf::Clock clock;
	while (running) {
		clock.Reset();
		if (!ingame) {
			reap();
			if (!startGame()) {
				sf::Sleep(0.01);
				continue;
			}
		}

		sendClientData();
		doTanks();
		sendObsData();
		doShells();
		checkWon();
		if (clock.GetElapsedTime() < 1)
			sf::Sleep(1-clock.GetElapsedTime());
	}
}

bool startGame() {
	gpool.clear();
	if (pool.size() < 2) return false;

	int id = 1;
	for (int i = 0; i < pool.size(); ++i) {
		pool[i]->init();
		pool[i]->id = id++;
		try {
			obsSend(*pool[i], (FMT("NEWGAME %1% 42\n")%pool.size()).str());
			obsRecv(*pool[i]);
		} catch (const std::exception& e) {
			LOG("On newgame, client %1% error: %2%",%pool[i]->ip%e.what());
			return false;
		}
		gpool.push_back(pool[i]);
	}
	for (int i = 0; i < obs.size(); ++i)
		try {
			obsSend(obs[i], "NEWGAME\n");
		} catch (const std::exception& e) {
			LOG("startGame: Obs %1% error: %2%",%obs[i].ip%e.what());
		}
	ingame = true;
	return true;
}

void sendClientData() {
	for (int i = 0; i < gpool.size(); ++i) {
		if (gpool[i]->hp == 0) continue;
		try {
			for (int j = 0; j < gpool.size(); ++j) {
				if (gpool[j]->hp == 0) continue;
				obsSend(*gpool[i], (FMT("LOCATION %1% %2% %3% %4% %5%\n")%
					(i==j ? 0 : gpool[j]->id)%gpool[j]->x%gpool[j]->y%gpool[j]->hp%gpool[j]->stam).str());
			}
			for (int j = 0; j < shells.size(); ++j)
				obsSend(*gpool[i], (FMT("SHELL %1% %2% %3%\n")
					%shells[j].x%shells[j].y%shells[j].t).str());
		} catch (const std::exception& e) {
			gpool[i]->hp = 0;
			LOG("sendClientData: Client %1% error: %2%",%gpool[i]->ip%e.what());
		}
	}
}

void sendObsData() {
	bool cont = false;
	for (int i = 0; i < obs.size(); ++i) {
		try {
			for (int j = 0; j < gpool.size(); ++j) {
				//Obs specific format: PLAYER id x y hp stam r g b name
				obsSend(obs[i], (FMT("PLAYER %1% %2% %3% %4% %5% %6% %7% %8% %9%\n")
					%gpool[j]->id%gpool[j]->x%gpool[j]->y%gpool[j]->hp%
					gpool[j]->stam%gpool[j]->r%gpool[j]->g%gpool[j]->b%gpool[j]->name).str());
			}

			for (int j = 0; j < shells.size(); ++j) {
				//Obs specific format: SHELL sx sy x y st t id
				obsSend(obs[i], (FMT("SHELL %1% %2% %3% %4% %5% %6% %7%\n")
					%shells[j].sx%shells[j].sy%shells[j].x%shells[j].y%shells[j].st%shells[j].t%shells[j].id).str());
			}
			//After data sent, send END
			obsSend(obs[i], "END\n");
		} catch (const std::exception& e) {
			LOG("sendObsData: Obs %1% error: %2%",%obs[i].ip%e.what());
		}
	}
}

void doTanks() {
	for (int i = 0; i < gpool.size(); ++i) {
		try {
			LOG("Tank %1%: x:%2% y:%3% hp:%4%",%i%gpool[i]->x%gpool[i]->y%gpool[i]->hp);
			if (gpool[i]->hp == 0) continue;
			obsSend(*gpool[i], "YOURMOVE\n");
			std::string mStr = obsRecv(*gpool[i]);

			std::vector<std::string> move = split(mStr, " ");
			int vx = ITOA(move[1]), vy = ITOA(move[2]);
			int tx = ITOA(move[3]), ty = ITOA(move[4]), tt = ITOA(move[5]);

			if (tt && gpool[i]->stam > 0) {
				tx = clamp(tx, -1000, 1000);
				ty = clamp(ty, -1000, 1000);
				tt = std::min(10, tt);
				tt = std::max(1, dist(gpool[i]->x, gpool[i]->y, tx, ty)/400);
				shells.push_back(Shell(gpool[i]->x, gpool[i]->y, tx, ty, tt, tt, ++shellId));
				gpool[i]->stam--;
			}

			if ((vx||vy) && gpool[i]->stam > 0) {
				float fac = 100.0/dist(0, 0, vx, vy);
				gpool[i]->x = clamp(gpool[i]->x+fac*vx, -1000, 1000);
				gpool[i]->y = clamp(gpool[i]->y+fac*vy, -1000, 1000);
				gpool[i]->stam--;
			}

			gpool[i]->stam++;
		} catch (const std::exception& e) {
			gpool[i]->hp = 0;
			LOG("doTanks: Client %1% error: %2%",%gpool[i]->ip%e.what());
		}
	}
}

void doShells() {
	for (int i = 0; i < shells.size();) {
		if (shells[i].t == 0) {
			shells.erase(shells.begin()+i);
			continue;
		}
		shells[i].t--;
		if (shells[i].t == 0)
			for (int j = 0; j < gpool.size(); ++j) {
				if (gpool[j]->hp == 0) continue;
				if (dist(gpool[j]->x, gpool[j]->y, shells[i].x, shells[i].y) <= 100) gpool[j]->hp -= 20;
			}

		++i;
	}
}

void checkWon() {
	int alive = 0;
	for (int i = 0; i < gpool.size(); ++i)
		if (gpool[i]->hp > 0) alive++;
	if (alive > 1) return;

	for (int i = 0; i < gpool.size(); ++i)
		try {
			obsSend(*gpool[i], "GAMEOVER Goodbye, you suck\n");
		} catch (const std::exception& e) {
			LOG("checkWon: Client %1% error: %2%",%gpool[i]->ip%e.what());
		}
	ingame = false;
}
