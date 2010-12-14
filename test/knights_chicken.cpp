#include <stdio.h>
#include <stdlib.h>

#include "knights.h"

int self_x = 0;
int self_y = 0;

void client_name(void) {
  set_name("Headless_Chicken");
  set_colour(255, 128, 0);
}

void client_init(int playercount) {

}

void client_your_turn() {
  // Run in a crazy direction
  move(self_y, -self_x, 0, 0, 0);
}

void client_tank_location(int id, int x, int y, int hp, int stamina) {
  // If this is me...
  if (id == 0) {
    
    // ...store my coordinates so I know where to flee.
    self_x = x;
    self_y = y;
  }
}

void client_shell_launched(int x, int y, int t) {}

