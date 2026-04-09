#include "game/game.h"
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
