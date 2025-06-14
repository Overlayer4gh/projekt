#include "Enemy.h"
#include <iostream>
#include <random>
#include "Globals.hpp" // Upewnij siê, ¿e masz dostêp do globalFont

Enemy::Enemy() : eq(100, 50, 2, 8) {
    if (!texture.loadFromFile("assets/wrog.png")) {
        std::cout << "Failed to load enemy texture\n";
    }
    sprite.emplace(texture);
    sprite->setOrigin( sf::Vector2f(frameSize.x / 2.f ,  frameSize.y / 2.f) );
    sprite->setTextureRect(sf::IntRect({ 0, 0 }, {frameSize.x, frameSize.y}));
	sprite->setPosition(sf::Vector2f(640.f, 320.f));
	setPosition(); // Ustawienie losowej pozycji wroga
}

void Enemy::update(float deltaTime, sf::Vector2f playerPos, Player& player) {
    sf::Vector2f prevPos = sprite->getPosition();
    if (attackCooldown > 0.f) {
        attackCooldown -= deltaTime;
    }

    if (!this) return;      // tutaj uwaga moze byc blad inicjalizacji enemy w enemies
    frameTimer += deltaTime;

    if (isAttacking) {
        attackTimer += deltaTime;
        

        // Animacja ataku
        if (frameTimer >= frameTime) {
            frameTimer = 0.f;
            currentFrame++;

            if (currentFrame >= attackframeCount) {
                currentFrame = 0;
            }
        }

        // Zakoñczenie ataku
        if (attackTimer >= attackDuration) {
            isAttacking = false;
            attackTimer = 0.f;
            currentFrame = 0;
            currentRow = 0; // powrót do domyœlnego rzêdu
            if (std::sqrt(lastMovement.x * lastMovement.x + lastMovement.y * lastMovement.y) < 100.f) {
                player.eq.takeDamage(this->eq.attackPower); 
            }

        }
        else {
            if (isFacingLeft) {
                currentRow = 2;
            }
            else {
                currentRow = attackDirection;
            }
        }

        



        int fx = static_cast<int>(frameSize.x);
        int fy = static_cast<int>(frameSize.y);
        int row = currentRow;
        int col = currentFrame;

        sf::IntRect rect({ col * fx, row * fy }, { fx, fy });
        sprite->setTextureRect(rect);

       
       // Flip sprite przez skalê
        sprite->setOrigin(sf::Vector2f(static_cast<float>(frameSize.x) / 2.f, static_cast<float>(frameSize.y) / 2.f));
        // Pobierz aktualn¹ skalê (wielkoœæ) sprite'a
        sf::Vector2f scale = sprite->getScale();
        float absScaleX = std::abs(scale.x);
        float absScaleY = std::abs(scale.y);
        // Ustaw odpowiedni znak X, zachowuj¹c powiêkszenie
        sprite->setScale(sf::Vector2f(isFacingLeft ? -absScaleX : absScaleX, absScaleY));

        sprite->setTextureRect(rect);
        return; // podczas ataku nie ruszamy siê
    }

    // Animacja chodzenia (nieatakuj¹cego wroga)
    if (frameTimer >= frameTime) {
        frameTimer = 0.f;
        currentFrame = (currentFrame + 1) % frameCount;
    }

    // Wiersz domyœlny – np. chodzenie w dó³
    currentRow =1;

    // Ustaw TextureRect bez flipu
    sf::Vector2i framePos = {
        currentFrame * frameSize.x,
        currentRow * frameSize.y
    };
    sprite->setTextureRect(sf::IntRect(framePos, frameSize));

    // Ruch w kierunku gracza i ewentualny atak
    FollowPlayer(playerPos, deltaTime);
    // SprawdŸ, czy pozycja siê zmieni³a
    bool isReallyIdle = (sprite->getPosition() == prevPos);

    // Animacja idle lub chodzenia
    if (isReallyIdle) {
        // Animacja idle (np. rz¹d 0, 4 klatki)
        currentRow = 0;
        frameCount = 4;
    }
    else {
        // Animacja chodzenia (np. rz¹d 1, 6 klatek)
        currentRow = 1;
        frameCount = 6;
    }

    // Animacja (idle lub chodzenia)
    if (frameTimer >= frameTime) {
        frameTimer = 0.f;
        currentFrame = (currentFrame + 1) % frameCount;
    }

    framePos = {
        currentFrame * frameSize.x,
        currentRow * frameSize.y
    };
    sprite->setTextureRect(sf::IntRect(framePos, frameSize));

    // Flipowanie sprite'a podczas idle/chodzenia
    sprite->setOrigin(sf::Vector2f(static_cast<float>(frameSize.x) / 2.f, static_cast<float>(frameSize.y) / 2.f));
    sf::Vector2f scale = sprite->getScale();
    float absScaleX = std::abs(scale.x);
    float absScaleY = std::abs(scale.y);
    sprite->setScale(sf::Vector2f(isFacingLeft ? -absScaleX : absScaleX, absScaleY));

    if (this->eq.hp == 0) {
        this->isDead = true; // oznacz wroga jako martwego
    }





}

void Enemy::Attack(int attackDirection) {
    if (!isAttacking && attackCooldown <= 0.f) {
        isAttacking = true;
        attackTimer = 0.f;
        this->attackDirection = attackDirection;
        currentFrame = 0;
        isFacingLeft = (attackDirection == 5);
        attackCooldown = cooldown; // ustaw cooldown po rozpoczêciu ataku
    }
	}






void Enemy::render(sf::RenderWindow& window) {
    if (sprite.has_value()) {
        window.draw(*sprite);

        sf::Vector2f pos = sprite->getPosition();
        sf::Vector2f barSize(60.f, 8.f); // szerokoœæ, wysokoœæ paska
        float hpPercent = static_cast<float>(eq.hp) / eq.maxHp;
        float x = pos.x;
        float y = pos.y;
        // T³o paska HP
        sf::RectangleShape hpBack(barSize);
        hpBack.setFillColor(sf::Color(50, 50, 50)); // ciemniejsze t³o
        float posX = x - barSize.x / 2.f;
        float posY = y - static_cast<float>(frameSize.y) / 4.f + 1.f;
        hpBack.setPosition(sf::Vector2f(posX, posY));
        // Pasek aktualnego HP
        sf::RectangleShape hpBar(sf::Vector2f(barSize.x * hpPercent, barSize.y));
        hpBar.setFillColor(sf::Color::Red);
        hpBar.setPosition(hpBack.getPosition());

        window.draw(hpBack);
        window.draw(hpBar);
    }
   /* sf::CircleShape dot(3.f);
    dot.setFillColor(sf::Color::Yellow);
    dot.setOrigin({ 1.5f, 1.5f });
    dot.setPosition(sprite->getPosition());
    window.draw(dot);
    */ // test sprite
}

void Enemy::setPosition() {
    if (sprite.has_value()) {
        // Sta³e: wymiary mapy
        constexpr float mapWidth = 1240.f;
        constexpr float mapHeight = 640.f;

        // Generator losowy (statyczny, ¿eby nie resetowaæ co wywo³anie)
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> distX(0.f, mapWidth);
        std::uniform_real_distribution<float> distY(0.f, mapHeight);

        // Losowa pozycja
        float randomX = distX(gen);
        float randomY = distY(gen);

        // Ustawienie losowej pozycji sprite’a
        sprite->setPosition({ randomX, randomY });
    }
}

void Enemy::FollowPlayer(const sf::Vector2f& playerPos, float deltaTime) {
	if (!sprite.has_value()) return;
	sf::Vector2f enemyPos = sprite->getPosition();
	sf::Vector2f direction = playerPos - enemyPos;
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 100.f) {
        isIdle = false;
        int attackDirection = 0;
		lastMovement = direction; // zapamiêtanie kierunku ruchu enemy
        if (std::abs(lastMovement.x) > std::abs(lastMovement.y)) {
            if (lastMovement.x > 0) attackDirection = 2;   // atak prawo
            else attackDirection = 5;                       // atak lewo (flip animacji prawej)
        }
        else {
            if (lastMovement.y > 0) attackDirection = 3;   // atak dó³
            else attackDirection = 4;                       // atak góra
        }

		Attack(attackDirection); // atak wroga, jeœli gracz jest blisko
        return;
    }
	if (distance > 50.f) { // jeœli gracz jest dalej ni¿ 50 pikseli
		direction /= distance; // normalizacja wektora kierunku
		enemyPos += direction * 100.f * deltaTime; // prêdkoœæ ruchu wrogów
		sprite->setPosition(enemyPos);
        isIdle = false;
       
	}
	else {
		isIdle = true; // wróg jest blisko gracza, wiêc jest bezczynny
	}
	// Ustawienie kierunku patrzenia wroga
	if (std::abs(direction.x) > std::abs(direction.y)) {
		isFacingLeft = direction.x < 0; // patrzy w lewo, jeœli x jest ujemne
	}
	else {
		isFacingLeft = direction.y < 0; // patrzy w górê, jeœli y jest ujemne
	}
}

sf::FloatRect Enemy::getBounds() const {
    if (sprite.has_value()) {
        return sprite->getGlobalBounds(); // lub w³asny hitbox
    }
    return {};
}
