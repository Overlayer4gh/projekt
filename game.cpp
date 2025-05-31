#pragma once
#include "game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include "Player.h"

using namespace std;


 

void Game::initVariables() {
	this->window = nullptr;
}
void Game::initGlobals() {
		if (!globalFont.openFromFile("assets/global.ttf")) {
			// obs³uga b³êdu - np. wyrzuæ wyj¹tek lub wypisz b³¹d
			throw std::runtime_error("Nie mo¿na za³adowaæ czionki globalFont");
		}
	

}

void Game::initWindow() {
	this->videoMode.size = { 1280,640 };
	this->videoMode.bitsPerPixel = 32;

	this->window = new sf::RenderWindow(this->videoMode, "Gra", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(144);
}

int::Game::initMap() {
	constexpr std::array level = {
	   0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
	   10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12,
		10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12,
	   10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12,
	   10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12,
	   10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12,
	   10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12,
		10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12,
		 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12,
		20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22,
		

	};
	if (!map.load("assets/tilemap_flat.png", { 64, 64 }, level.data(), 20, 10))
		return -1;

}

/*void::Game::initEnemies() {
	//this->enemy.setPosition();
	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
	this->enemy.setFillColor(sf::Color::Red);
	this->enemy.setOutlineColor(sf::Color::Blue);
	this->enemy.setOutlineThickness(1.f);
} */


// konstruktory i destruktory

Game::Game(){
	 
	this->initVariables();
	this->initWindow();
	//this->triangle2.initTextures();
	this->initMap();
	this->initGlobals();
}


Game::~Game() {
	delete this->window;

}

//funkcje

void Game::pollEvents() {
	

	while (auto eventOpt = this->window->pollEvent()) {
		sf::Event& event = *eventOpt;
		if (event.is<sf::Event::Closed>()) {
			this->window->close();
		}
		else if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				this->window->close();
		}
	}


};



const bool Game::running() const {
    return this->window && this->window->isOpen();
}

void Game::update() {
	this->pollEvents();
	float deltaTime = 1.f / 144.f; // deltatime
	sf::Vector2f playerPos = player.getPosition(); // pobieranie pozycji gracza
	player.updateAnimation(deltaTime);
	enemy.update(deltaTime, playerPos); // na chwile
	player.movePlayer(this->window->getSize(),deltaTime);


	cout << "mouse position: " << sf::Mouse::getPosition(*this->window).x << " " << sf::Mouse::getPosition(*this->window).y << endl;
}


void Game::render() {


	this->window->clear(sf::Color::Black);

	//rysowanie 

	this->window->draw(this->map);
	if (player.sprite.has_value()) {
		this->window->draw(*player.sprite);
		player.render(*this->window);
		enemy.render(*this->window);
	}

	this->window->display();
}