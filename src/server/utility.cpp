#include "utility.hpp"
#include "add_client.hpp"
#include "game.hpp"

std::string obsRecv(Obs& obs) {
	if (!obs.valid)
		LOG("Warning: Trying to receive data from invalid socket.",);

	try {
		return sfRecv(obs.sock);
	} catch (const std::exception& e) {
		obs.valid = false;
		throw;
	}
	return "";
}

void obsSend(Obs& obs, const std::string& msg) {
	if (!obs.valid) LOG("Warning: Sending data to invalid socket.",);

	try {
		sfSend(obs.sock, msg);
	} catch (const std::exception& e) {
		obs.valid = false;
		throw;
	}
}

void reap() {
	for (int i = 0; i < pool.size();) {
		if (pool[i]->valid) {++i; continue;}
		deleteClient(i);
	}
	for (int i = 0; i < obs.size();) {
		if (obs[i].valid) {++i; continue;}
		deleteObs(i);
	}
}
