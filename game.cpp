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
	//enemies.push_back(std::make_unique<Enemy>());
	//enemies.push_back(std::make_unique<Enemy>());



}


Game::~Game() {
	delete this->window;

}

//funkcje


void Game::checkPlayerAttack(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies) {
	if (player.isCurrentlyAttacking()) {
		sf::FloatRect playerHitbox = player.getAttackHitbox();

		for (auto& enemy : enemies) {
			if (!enemy->wasDamagedThisAttack) {
				const sf::FloatRect playerHitbox = player.getAttackHitbox();
				const sf::FloatRect enemyBounds = enemy->getBounds();

				if (playerHitbox.position.x < enemyBounds.position.x + enemyBounds.size.x &&
					playerHitbox.position.x + playerHitbox.size.x > enemyBounds.position.x &&
					playerHitbox.position.y < enemyBounds.position.y + enemyBounds.size.y &&
					playerHitbox.position.y + playerHitbox.size.y > enemyBounds.position.y)
				{
					enemy->eq.takeDamage(player.eq.attackPower);
					enemy->wasDamagedThisAttack = true;
					std::cout << "Enemy damaged! Remaining HP: " << enemy->eq.hp << std::endl;
				}
			}
		}
	}
	else {
		for (auto& enemy : enemies) {
			enemy->wasDamagedThisAttack = false;
		}
	}
}









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

void Game::startlevel(int levelNumber) {
	currentLevel = std::make_unique<Level>(levelNumber);
	enemies.clear();
	for (int i = 0; i < currentLevel->enemyCount; ++i) {
		enemies.push_back(std::make_unique<Enemy>());
	}
	if (currentLevel->hasBoss) {
		enemies.clear();
		enemies.push_back(std::make_unique<Boss>());
	}
}

void Game::increaselevel() {
	++currentLevelNumber;
	startlevel(currentLevelNumber);
}




void Game::RenderGameOver() {
	sf::Text gameOverText( globalFont, "GAME OVER",  120); // font i rozmiar w konstruktorze
	gameOverText.setFillColor(sf::Color::Red);
	gameOverText.setStyle(sf::Text::Bold);

	// Wyœrodkowanie tekstu na ekranie
	sf::FloatRect textRect = gameOverText.getLocalBounds();
	gameOverText.setOrigin(sf::Vector2f(textRect.size.x / 2.f, textRect.size.y / 2.f));
	gameOverText.setPosition(sf::Vector2f(
		static_cast<float>(window->getSize().x) / 2.f,
		static_cast<float>(window->getSize().y) / 2.f - 50.f
	));

	window->draw(gameOverText);


};

void Game::renderLevelNumber() {
	sf::Text LevelNumberText(globalFont, "Level: " + std::to_string(currentLevelNumber), 32);
	LevelNumberText.setFillColor(sf::Color::White);
	sf::FloatRect textRect = LevelNumberText.getLocalBounds();
	float margin = 60.f;
	float posX = static_cast<float>(window->getSize().x) - textRect.size.x - margin;
	float posY = 20.f;

	LevelNumberText.setPosition(sf::Vector2f(posX, posY));

	window->draw(LevelNumberText);

};









const bool Game::running() const {
    return this->window && this->window->isOpen();
}

void Game::update() {
	this->pollEvents();
	float deltaTime = 1.f / 144.f; // deltatime
	sf::Vector2f playerPos = player.getPosition(); // pobieranie pozycji gracza
	player.updateAnimation(deltaTime);
	// enemy.update(deltaTime, playerPos, player); // na chwile
	player.movePlayer(this->window->getSize(),deltaTime);
	for (auto& enemy : enemies) {
		if (enemy) {
			enemy->update(deltaTime, player.getPosition(), player);
		}
		if (enemy && enemy->isDead) {
			this->enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
				[](const std::unique_ptr<Enemy>& e) { return e->isDead; }), enemies.end());
		}
	}
	if (enemies.empty()) {
		increaselevel();

	}

	checkPlayerAttack(player, enemies);

	// cout << "mouse position: " << sf::Mouse::getPosition(*this->window).x << " " << sf::Mouse::getPosition(*this->window).y << endl;	
}


void Game::render() {


	this->window->clear(sf::Color::Black);

	//rysowanie 

	this->window->draw(this->map);
	renderLevelNumber();
	if (player.sprite.has_value()) {
		this->window->draw(*player.sprite);
		player.render(*this->window);
		for (auto& enemy : enemies) {
			enemy->render(*this->window);
		}
	}
	if (player.eq.hp == 0) {
		RenderGameOver();
		

	}


	this->window->display();
	
}