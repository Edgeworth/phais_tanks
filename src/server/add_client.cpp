#include "add_client.hpp"
#include "game.hpp"
#include "utility.hpp"

void addClients(void*) {
	sf::SocketTCP listen;
	sf::IPAddress ip;
	listen.Listen(12317);

	sf::SocketTCP client;
	while (running) {
		try {
			listen.Accept(client, &ip);
			Obs o(client, ip.ToString());
			obsSend(o, "NAME PLEASE\n");

			std::vector<std::string> reply = split(obsRecv(o), " ");
			if (reply[0] == "OBS")
				addObs(o);
			else if (reply[0] == "NAME") {
				Tank* k = new Tank(o);
				k->name = reply[1];
				k->r = ITOA(reply[2]);
				k->g = ITOA(reply[3]);
				k->b = ITOA(reply[4]);
				addClient(k);
			}
		} catch (const std::exception& e) {
			LOG("addClients: Client %1% error: %2%",%ip.ToString()%e.what());
		}
	}
}

void addClient(Tank* k) {
	sf::Lock lock(gMutex);
	LOG("Adding client %1%",%k->ip);
	pool.push_back(k);
}

void deleteClient(int i) {
	sf::Lock lock(gMutex);
	pool[i]->sock.Close();
	for (int j = 0; j < gpool.size(); ++j)
		if (pool[i] == gpool[j]) {
			gpool.erase(gpool.begin()+j);
			break;
		}
	pool.erase(pool.begin()+i);

	LOG("Client %1% deleted",%i);
}


void addObs(const Obs& s) {
	sf::Lock lock(gMutex);
	LOG("Adding obs %1%",%s.ip);
	obs.push_back(s);
}
void deleteObs(int i) {
	sf::Lock lock(gMutex);
	obs[i].sock.Close();
	obs.erase(obs.begin()+i);

	LOG("Obs %1% deleted",%i);
}
