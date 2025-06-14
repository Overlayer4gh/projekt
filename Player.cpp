#include "Player.h"
#include <iostream>
#include <string>
#include "Globals.hpp"

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

	// Pasek zdrowia (t³o)
	sf::RectangleShape healthBarBack(sf::Vector2f(180.f, 20.f));  
	healthBarBack.setFillColor(sf::Color(100, 100, 100));
	healthBarBack.setPosition({ 14.f, 14.f });

	// Pasek zdrowia (aktualny)
	sf::RectangleShape healthBar(sf::Vector2f(180.f * (float)eq.hp / eq.maxHp, 20.f)); 
	healthBar.setFillColor(sf::Color(250, 20, 20));
	healthBar.setPosition({ 14.f, 14.f }); 

	window.draw(healthBarBack);
	window.draw(healthBar);

	// Tekst HP
	sf::Text hpText(globalFont, std::to_string(eq.hp) + " / " + std::to_string(eq.maxHp), 12);
	hpText.setFont(globalFont);
	hpText.setCharacterSize(12); 
	hpText.setFillColor(sf::Color::White);
	hpText.setPosition(sf::Vector2f(14.f + 90.f - hpText.getLocalBounds().size.x / 2.f, 14.f + 2.f)); // wyœrodkowany
	window.draw(hpText);

	// Pasek many (t³o)
	sf::RectangleShape manaBarBack(sf::Vector2f(180.f, 20.f)); // analogicznie jak zdrowie
	manaBarBack.setFillColor(sf::Color(100, 100, 100));
	manaBarBack.setPosition({ 14.f, 44.f }); // 10 px od góry + 24 (wysokoœæ t³a zdrowia) + 6 px odstêpu

	// Pasek many (aktualny)
	sf::RectangleShape manaBar(sf::Vector2f(180.f * (float)eq.mana / eq.maxMana, 20.f));
	manaBar.setFillColor(sf::Color(20, 20, 250));
	manaBar.setPosition({ 14.f, 44.f });

	window.draw(manaBarBack);
	window.draw(manaBar);

	// Tekst many
	sf::Text ManaText(globalFont, std::to_string(eq.mana) + " / " + std::to_string(eq.maxMana), 12);
	ManaText.setFont(globalFont);
	ManaText.setCharacterSize(12);
	ManaText.setFillColor(sf::Color::White);
	ManaText.setPosition(sf::Vector2f(14.f + 90.f - ManaText.getLocalBounds().size.x / 2.f, 44.f + 2.f));
	window.draw(ManaText);
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
			currentRow = 0; 
		}
		else {
			currentRow = attackDirection; 
		}
	}
	else {
		if (frameTimer >= frameTime) {
			frameTimer = 0.f;
			currentFrame = (currentFrame + 1) % frameCount;
		}
		
		
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

			// Zapamiêtaj ostatni kierunek ruchu 
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

	// Kolizja , ograniczenia ekranu 

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

		// Skalowanie sprite (flip) jeœli atak w lewo
		if (attackDirection == 2) {  
			sprite->setScale({ -1.f, 1.f });  // flip poziomy
			isFacingLeft = true;
		}
		else {
			sprite->setScale({ 1.f, 1.f });  
			isFacingLeft = false;
		}
	}
}

		//hitbox

sf::FloatRect Player::getAttackHitbox() const {				
	if (!isAttacking || !sprite.has_value()) return sf::FloatRect();

	sf::Vector2f pos = sprite->getPosition();
	float range = 40.f; // zasiêg ataku
	sf::Vector2f size(40.f, 40.f);
	sf::Vector2f offset;

	switch (attackDirection) {
	case 2: offset = { -range, 0.f }; break;  // lewo
	case 3: offset = { range, 0.f }; break;   // prawo
	case 4: offset = { 0.f, range }; break;   // dó³
	case 6: offset = { 0.f, -range }; break;  // góra
	default: offset = { 0.f, 0.f }; break;
	}

	sf::Vector2f center = pos + offset;
	sf::Vector2f topLeft(center.x - size.x / 2.f, center.y - size.y / 2.f);
	return sf::FloatRect(topLeft, size);
}

bool Player::isCurrentlyAttacking() const {
	return isAttacking;
}


sf::Vector2f Player::getPosition() const {
	if (sprite.has_value()) {
		return sprite->getPosition();
	}
	return sf::Vector2f(0.f, 0.f); // lub inna wartoœæ domyœlna
}


