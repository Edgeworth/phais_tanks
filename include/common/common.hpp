#ifndef COMMON_HPP_
#define COMMON_HPP_
#include <cstdio>
#include <ctime>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


#define LOG(x, y) fprintf(stderr, "%s\n", (boost::format(x) y ).str().c_str());
#define FMT(x) boost::format(x)
#define ITOA boost::lexical_cast<int>

inline int clamp(int x, int a, int b) {
	return x < a ? a : (x > b ? b : x);
}

inline int dist(int x0, int y0, int x1, int y1) {
	return sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
}

std::vector<std::string> split(const std::string& a, const std::string& sep);
void sfSend(sf::SocketTCP& sock, const std::string& msg);
std::string sfRecv(sf::SocketTCP& sock);

#endif
