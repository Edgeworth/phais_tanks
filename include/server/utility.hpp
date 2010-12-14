#ifndef UTILITY_HPP_
#define UTILITY_HPP_
#include "common.hpp"

std::string obsRecv(Obs& obs);
void obsSend(Obs& obs, std::string msg);
std::vector<std::string> split(const std::string& a, const std::string& sep);
void reap();


#endif
