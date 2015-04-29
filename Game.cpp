#ifndef GAME_CPP
#define GAME_CPP

#include "Game.hpp"

Game::Game() {
	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 10; ++j)
			tiles[i][j] = TILE_EMPTY;
}
void Game::initSnakesLadders() {
	map<int, int> :: iterator it;
	for (it = snakes.begin(); it != snakes.end(); it++) {
		pair<int, int> p = getCoordinates(it->first);
		tiles[p.first][p.second] = TILE_SNKST;
		p = getCoordinates(it->second);
		tiles[p.first][p.second] = TILE_SNKEN;
	}
	for (it = ladders.begin(); it != ladders.end(); it++) {
		pair<int, int> p = getCoordinates(it->first);
		tiles[p.first][p.second] = TILE_LADST;
		p = getCoordinates(it->second);
		tiles[p.first][p.second] = TILE_LADEN;
	}
}
pair<int, int> Game::getCoordinates(int x) {
	return make_pair((x - 1) / 10, (x - 1) % 10);
}
int Game::getTile(int x) {
	return tiles[(x - 1) / 10][(x - 1) % 10];
}
void Game::move() {
	if (current == 1) {
		if (p1 + ra <= 100) {
			p1 = p1 + ra;
			if (snakes.find(p1) != snakes.end()) {
				p1 = snakes[p1];
			}
			if (ladders.find(p1) != ladders.end()) {
				p1 = ladders[p1];
			}
			if (p1 == 100) {
				printf("winner is 1\n");
				gameend = 1;
			}
		}

	} else {
		if (p2 + ra <= 100) {
			p2 = p2 + ra;
			if (snakes.find(p2) != snakes.end()) {
				p2 = snakes[p2];
			}
			if (ladders.find(p2) != ladders.end()) {
				p2 = ladders[p2];
			}
			if (p2 == 100) {
				printf("winner is 2\n");
				gameend = 1;
			}
		}
	}
	current--;
	current = 1 - current;
	current++;
}

#endif

