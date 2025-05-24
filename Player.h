
#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "Eq.h"
#include <SFML/Window/Event.hpp>

class Player{

private:


public:
	Player();

	Eq eq;
	void movePlayer(const sf::Vector2u& windowSize,float DeltaTime);
	void updateAnimation(float deltaTime);
	void startAttack(int direction);
	void render(sf::RenderWindow& window);

	// poruszanie i animacje
	int currentFrame = 0;
	int currentRow = 0;
	int frameCount = 6;
	float frameTime = 0.1f;
	float frameTimer = 0.f;
	sf::Vector2i frameSize = { 192, 192 };
	//atakowanie
	bool isAttacking = false;
	float attackTimer = 0.f;
	float attackDuration = 0.5f; // czas trwania animacji ataku w sekundach
	int attackDirection = 0; // który wiersz animacji ataku
	int attackFrameCount = 4;
	bool isFacingLeft = false;
	sf::Vector2f lastMovement{ 0.f, 1.f }; // domyœlnie w dó³



	sf::Texture texture;
	std::optional<sf::Sprite> sprite;

};
