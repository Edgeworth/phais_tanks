#ifndef ADD_CLIENT_HPP_
#define ADD_CLIENT_HPP_
#include "common.hpp"
#include "tank.hpp"

void addClients(void*);
void addClient(Tank* k);
void deleteClient(int i);
void addObs(const Obs& s);
void deleteObs(int i);

#endif
