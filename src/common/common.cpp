#include "common.hpp"

std::vector<std::string> split(const std::string& a, const std::string& sep) {
	std::vector<std::string> s;
	boost::split(s, a, boost::is_any_of(sep), boost::token_compress_on);
	return s;
}

void sfSend(sf::SocketTCP& sock, const std::string& msg) {
	int status = sock.Send(msg.c_str(), msg.size());
	if (status != sf::Socket::Done) {
		throw std::runtime_error((FMT("obsSend: %1%")%(
		status == sf::Socket::NotReady ? "Not ready." :
		status == sf::Socket::Disconnected ? "Disconnected." : "Error.")).str());
	}
}
std::string sfRecv(sf::SocketTCP& sock) {
	char data[1000];
	size_t sz;

	int status = sock.Receive(data, 1000, sz);
	if (status != sf::Socket::Done) {
		throw std::runtime_error((FMT("obsRecv: %1%")%(
		status == sf::Socket::NotReady ? "Not ready." :
		status == sf::Socket::Disconnected ? "Disconnected." : "Error.")).str());
	}

	return boost::trim_copy_if(std::string(data, data+sz), boost::is_any_of("\n\r"));
}
