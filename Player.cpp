#include "Player.h"
#include <iostream>
Player::Player() : eq(100,50,15,8) {
	if (!texture.loadFromFile("assets/animacje1.png")) {
		std::cout << "Error loading texture" << std::endl;
	}
	else {
		sprite.emplace(texture);
		sprite->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), frameSize));
		
		sprite->setTextureRect(sf::IntRect({ 0, 0 }, frameSize));
		sprite->setOrigin(sf::Vector2f(frameSize.x / 2.f, frameSize.y / 2.f));
		sprite->setPosition(sf::Vector2f(100.f, 100.f));
		std::cout << "jest ok";
	}
}


void Player::render(sf::RenderWindow& window) {
	window.draw(sprite.value());

	// Pasek zdrowia
	sf::RectangleShape healthBarBack(sf::Vector2f(104.f, 14.f));
	healthBarBack.setFillColor(sf::Color(100, 100, 100));
	healthBarBack.setPosition({ 10.f, 10.f });

	sf::RectangleShape healthBar(sf::Vector2f(100.f * (float)eq.hp / eq.maxHp, 10.f));
	healthBar.setFillColor(sf::Color(250, 20, 20));
	healthBar.setPosition({ 12.f, 12.f });

	window.draw(healthBarBack);
	window.draw(healthBar);
	sf::RectangleShape manaBarBack(sf::Vector2f(104.f, 14.f));
	manaBarBack.setFillColor(sf::Color(100, 100, 100));
	manaBarBack.setPosition({ 10.f, 28.f }); // 10 px pod paskiem zdrowia + 4 px odstêpu

	sf::RectangleShape manaBar(sf::Vector2f(100.f * (float)eq.mana / eq.maxMana, 10.f));
	manaBar.setFillColor(sf::Color(20, 20, 250)); // niebieski
	manaBar.setPosition({ 12.f, 30.f });

	window.draw(manaBarBack);
	window.draw(manaBar);

};











void Player::updateAnimation(float deltaTime) {
	frameTimer += deltaTime;
	if (isAttacking) {
		attackTimer += deltaTime;
		if (frameTimer >= frameTime) {
			frameTimer = 0.f;
			currentFrame++;
			if (currentFrame >= attackFrameCount) { // attackFrameCount - ile klatek ma animacja ataku
				currentFrame = 0;
			}
		}

		if (attackTimer >= attackDuration) {
			isAttacking = false;
			currentFrame = 0;
			currentRow = 0; // wróæ do idle lub chodzenia po zakoñczeniu ataku
		}
		else {
			currentRow = attackDirection; // ustaw wiersz animacji na kierunek ataku
		}
	}
	else {
		if (frameTimer >= frameTime) {
			frameTimer = 0.f;
			currentFrame = (currentFrame + 1) % frameCount;
		}
		// normalna animacja (idle lub chodzenie)
		
	}






		// Wyznacz prostok¹t: (klatka_x, klatka_y)
		sf::Vector2i framePos = {
			currentFrame * frameSize.x,
			currentRow * frameSize.y
		};
	
		sprite->setTextureRect(sf::IntRect(framePos, frameSize));
	}




void Player::movePlayer(const sf::Vector2u& windowSize, float deltaTime) {
	if (!sprite.has_value()) return;

	float speed = 200.0f;
	sf::Vector2f movement(0.f, 0.f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
		movement.y -= speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
		movement.y += speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
		movement.x -= speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
		movement.x += speed;

	if (!isAttacking) {
		if (movement != sf::Vector2f(0.f, 0.f)) {
			
			float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
			movement /= length;
			movement *= speed * deltaTime;

			currentRow = 1; // chodzenie
			sprite->move(movement);

			
			lastMovement = movement;

			// Ustaw flip zale¿nie od kierunku ruchu (x)
			if (movement.x < 0) {
				sprite->setScale({ -1.f, 1.f });
				isFacingLeft = true;
			}
			else if (movement.x > 0) {
				sprite->setScale({1.f, 1.f});
				isFacingLeft = false;
			}
		}
		else {
			currentRow = 0; 
		}

		if (!isAttacking && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space)) {
			int direction = 0;

			if (std::abs(lastMovement.x) > std::abs(lastMovement.y)) {
				if (lastMovement.x > 0) direction = 3;   // atak prawo
				else direction = 2;                       // atak lewo (flip animacji prawej)
			}
			else {
				if (lastMovement.y > 0) direction = 4;   // atak dó³
				else direction = 6;                       // atak góra
			}

			startAttack(direction);
		}
	}

	// Kolizja i ograniczenia ekranu jak wczeœniej...

	sf::Vector2f newPos = sprite->getPosition();

	auto collisionRect = sf::FloatRect({ 50.f, 80.f }, { 80.f, 120.f });
	sf::Vector2f originOffset = sf::Vector2f(frameSize.x / 2.f, frameSize.y / 2.f);

	float left = newPos.x - originOffset.x + collisionRect.position.x;
	float top = newPos.y - originOffset.y + collisionRect.position.y;
	float right = left + collisionRect.size.x;
	float bottom = top + collisionRect.size.y;

	if (left < 0.f) newPos.x += 0.f - left;
	if (right > static_cast<float>(windowSize.x)) newPos.x -= right - windowSize.x;
	if (top < 0.f) newPos.y += 0.f - top;
	if (bottom > static_cast<float>(windowSize.y)) newPos.y -= bottom - windowSize.y;

	sprite->setPosition(newPos);
}

// atakowanie

void Player::startAttack(int direction) {
	if (!isAttacking) {
		isAttacking = true;
		attackTimer = 0.f;
		attackDirection = direction;
		currentFrame = 0;

		
		if (attackDirection == 2) { 
			sprite->setScale({ -1.f, 1.f });  // flip poziomy
			isFacingLeft = true;
		}
		else {
			sprite->setScale({ 1.f, 1.f });   // normalny kierunek
			isFacingLeft = false;
		}
	}
}



