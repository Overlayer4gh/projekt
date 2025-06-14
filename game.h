#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window/Event.hpp>
#include <optional>
#include "mapa.h"
#include "Player.h"
#include "Enemy.h"
#include "Globals.hpp"
#include "Level.h"
#include "Boss.h"






class Game {

private:
	//zmienne
	//okno
	sf::RenderWindow* window;
	sf::VideoMode videoMode; 

	
	TileMap map;
	Player player;
	Enemy enemy;
	Boss boss;
	std::vector<std::unique_ptr<Enemy>> enemies;


	//funkcje private
	void initVariables();
	void initWindow();
	int initMap();
	void initGlobals();
	void RenderGameOver();
	void checkPlayerAttack(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies);
	void startlevel(int levelNumber);
	void increaselevel();
	void renderLevelNumber();


public:

	int currentLevelNumber = 1;
	std::unique_ptr<Level> currentLevel;




	//akcesory

	const bool running() const;



	// konstruktory i destruktory;
	Game();
	virtual ~Game();

	//funkcje

	void update();
	void render();
	void pollEvents();
	
};
