#include "game.h"
#include <iostream>
#include <string>

Game game;

int main() {
	game.init();
	while(1) {
		game.interact();
		game.updateEffect();
		game.render();
	}
}
