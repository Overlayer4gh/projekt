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








class Game {

private:
	//zmienne
	//okno
	sf::RenderWindow* window;
	sf::VideoMode videoMode; 

	
	TileMap map;
	Player player;
	Enemy enemy;

	//funkcje private
	void initVariables();
	void initWindow();
	int initMap();


public:


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
