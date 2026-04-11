#ifndef GAME_H
#define GAME_H
#include "lib/map.h"
#include "lib/gradient.h"
#include "lib/partical.h"
#include <string>

class Game {
public:
	Game(): typeCnt(7) {}
	
	void init();
	bool isOver();
	void updateEffect();
	void render(int mask=0);
	void interact(int processMouseEvent=1);
	
private:
	void addBall();
	int checkClear();
	void generateNext();
	void attemptMove(icord s, icord t);
	
	Map map;
	Cell icon;
	int ended;
	std::string present;
	fcord post;
	int typeCnt;
	icord targ, cursor;
	std::vector<int> next;
	gradient<float> score;
	std::vector<Partical> particals;
};

#endif
