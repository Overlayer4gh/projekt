#pragma once
#include "Enemy.h"
class Boss : public Enemy
{

public:
	Boss() : Enemy() {
		// Ustawienia specyficzne dla bossa
		sprite->setScale({ 1.3f, 1.3f }); // 2x wiêkszy boss
		eq.hp = 200; // Wiêksze HP dla bossa
		eq.maxHp = 200; // Maksymalne HP bossa
		eq.attackPower = 10; // Wiêksza si³a ataku
		attackframeCount = 6; // Wiêcej klatek animacji ataku
		frameSize = { 192, 192 }; // Zwiêkszenie rozmiaru ramki
		frameTime = 0.1f; // Czas trwania jednej klatki animacji
		currentRow = 0; // Domyœlny rz¹d animacji
		float cooldown = 1.f; // czas odnowienia ataku
		float attackCooldown = 1.f; // czas odnowienia ataku (w sekundach)
	}
	void update(float deltaTime, sf::Vector2f playerPos, Player& player) override {
		Enemy::update(deltaTime, playerPos, player);
		// Dodatkowe logiki specyficzne dla bossa mog¹ byæ tutaj dodane
	}


};

