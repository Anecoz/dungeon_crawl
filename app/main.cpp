#include <iostream>

#include "Game.h"

int main()
{
  std::cout << "Starting... " << std::endl;
  Game game;
  game.init();
  game.run();
  return 0;
}