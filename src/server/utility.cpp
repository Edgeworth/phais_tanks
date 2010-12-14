#include "utility.hpp"
#include "add_client.hpp"
#include "game.hpp"

std::string obsRecv(Obs& obs) {
	if (!obs.valid)
		LOG("Warning: Trying to receive data from invalid socket.",);

	char data[1000];
	size_t sz;

	int status = obs.sock.Receive(data, 1000, sz);
	if (status != sf::Socket::Done) {
		obs.valid = false;
		throw std::runtime_error((FMT("obsRecv: %1%")%(
		status == sf::Socket::NotReady ? "Not ready." :
		status == sf::Socket::Disconnected ? "Disconnected." : "Error.")).str());
	}

	std::string str = std::string(data, data+sz);
	boost::trim_if(str, boost::is_any_of("\n\r"));
	return str;
}

void obsSend(Obs& obs, std::string msg) {
	if (!obs.valid) LOG("Warning: Sending data to invalid socket.",);
	int status = obs.sock.Send(msg.c_str(), msg.size());
	if (status!=sf::Socket::Done) {
		obs.valid = false;
		throw std::runtime_error((FMT("obsSend: %1%")%(
		status == sf::Socket::NotReady ? "Not ready." :
		status == sf::Socket::Disconnected ? "Disconnected." : "Error.")).str());
	}
}

std::vector<std::string> split(const std::string& a, const std::string& sep) {
	std::vector<std::string> s;
	boost::split(s, a, boost::is_any_of(sep), boost::token_compress_on);
	return s;
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
