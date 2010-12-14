#include "net.hpp"
#include "draw.hpp"

sf::Mutex copyMutex;
bool running = true, gameover = false;
std::vector<std::vector<Tank> > tanks, copyTanks;
std::vector<std::vector<Shell> > shells, copyShells;

void runNet(void* ip) {
	try {
		sf::SocketTCP serv;
		serv.Connect(12317, (char*)ip);
		sfRecv(serv);
		sfSend(serv, "OBS\r\n");
		tanks.resize(1);
		shells.resize(1);

		while (running) {
			std::vector<std::string> cmds = split(sfRecv(serv), "\r\n");

			for (int i = 0; i < cmds.size(); ++i) {
				std::vector<std::string> cmd = split(cmds[i], " ");
				if (cmd[0] == "END") {
					sf::Lock lock(copyMutex);
					copyTanks = tanks;
					copyShells = shells;
					tanks.push_back(std::vector<Tank>());
					shells.push_back(std::vector<Shell>());
				} else if (cmd[0] == "NEWGAME") {
					sf::Lock lock(copyMutex);
					tanks.clear(); tanks.resize(1); copyTanks.clear();
					shells.clear(); shells.resize(1); copyShells.clear();
					state = 0;
				} else if (cmd[0] == "PLAYER") {
					Tank tank = {ITOA(cmd[1]), ITOA(cmd[2]), ITOA(cmd[3]),
						ITOA(cmd[4]), ITOA(cmd[5]), ITOA(cmd[6]), ITOA(cmd[7]), ITOA(cmd[8]), cmd[9]};
					tanks.back().push_back(tank);
				} else if (cmd[0] == "SHELL") {
					Shell shell = {ITOA(cmd[1]), ITOA(cmd[2]), ITOA(cmd[3]),
						ITOA(cmd[4]), ITOA(cmd[5]), ITOA(cmd[6])};
					shells.back().push_back(shell);
				}
			}
		}
	} catch (const std::exception& e) {
		LOG("Disconnected from server: %1%",%e.what());
		running = false;
	}
}
