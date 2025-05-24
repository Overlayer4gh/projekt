#include <iostream>
#include "game.h" 

int main() {
   
	Game game;

	while (game.running()) {
		//udate
		game.update();

		//render
		game.render();
	}
	return 0;
}