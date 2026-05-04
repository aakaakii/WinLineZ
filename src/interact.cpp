#include "game.h"
#include "raylib.h"
#include <math.h>

static inline fcord getMouseCoord() {
	auto [mx, my] = GetMousePosition();
	return {mx, my};
}

static inline int clamp(int x, int a, int b) {
	return std::max(std::min(x, b), a);
}

void Game::interact(int processMouseEvent) {
	cursor = ((getMouseCoord() - post) * (1. / gridsize)).cast();
	cursor.x = clamp(cursor.x, 0, N-1), cursor.y = clamp(cursor.y, 0, N-1);
	if(!processMouseEvent || ended) return ;
	if(IsMouseButtonPressed(0)) {
		if(map[cursor].type) {
			if(cursor == targ) targ = {-1, -1};
			else targ = cursor;
		} else {
			if(targ.x != -1) {
				attemptMove(targ, cursor);
			}
		}
	}
	if(IsMouseButtonPressed(1)) {
		targ = {-1, -1};
	}
}
