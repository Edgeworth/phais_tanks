#ifndef DRAW_HPP_
#define DRAW_HPP_
#include "common.hpp"

extern sf::RenderWindow window;
extern sf::Font font;
extern int state;

void draw(sf::RenderWindow& window, sf::Clock& clock);
void drawTanks(float alpha);
void drawShells(float alpha);
#endif
