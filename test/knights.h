#ifndef __PHAIS_H
#define __PHAIS_H

#ifdef __cplusplus
extern "C" {
#endif

  ///////////////////////////////////////////////////////

  // The following must be implemented by the client:

  void client_init(int);
  void client_name();
  void client_shell_launched(int x, int y, int t);
  void client_tank_location(int id, int x, int y, int hp, int nrg);
  void client_your_turn();

  // The following are available to the client:
  void set_name(const char* name);
  void set_colour(int r, int g, int b);
  int move(int vx, int vy, int sx, int sy, int st);


  //////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif
