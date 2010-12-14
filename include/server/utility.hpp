#ifndef UTILITY_HPP_
#define UTILITY_HPP_
#include "common.hpp"
#include "tank.hpp"

std::string obsRecv(Obs& obs);
void obsSend(Obs& obs, const std::string& msg);
void reap();


#endif
