#ifndef GAME_HPP
#define GAME_HPP

#include <map>
using namespace std;

enum {
	TILE_EMPTY,
	TILE_LADST,
	TILE_LADEN,
	TILE_SNKST,
	TILE_SNKEN
};

class Game {
  public:
  	// Player positions
	int p1 = 1;
	int p2 = 1;
	int tiles[10][10];
	map<int, int> snakes;
	map<int, int> ladders;
	int gameend = 0;
	int current = 1;
	// Stores a random number from 1 to 6
	int ra;
	Game();
	void initSnakesLadders();
	pair<int, int> getCoordinates(int x);
	int getTile(int x);
	void move();
};

#endif
