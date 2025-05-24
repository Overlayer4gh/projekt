#include "Enemy.h"
#include <iostream>

Enemy::Enemy() {
    if (!texture.loadFromFile("assets/wrog.png")) {
        std::cout << "Failed to load enemy texture\n";
    }
    sprite.emplace(texture);
    sprite->setOrigin( sf::Vector2f(frameSize.x / 2.f ,  frameSize.y / 2.f) );
    sprite->setTextureRect(sf::IntRect({ 0, 0 }, {frameSize.x, frameSize.y}));
	sprite->setPosition(sf::Vector2f(640.f, 320.f)); 
}

void Enemy::update(float deltaTime) {
    frameTimer += deltaTime;
    if (frameTimer >= frameTime) {
        frameTimer = 0.f;
        currentFrame = (currentFrame + 1) % frameCount;

       
        sprite->setTextureRect(sf::IntRect({ currentFrame * frameSize.x , 0 }, { frameSize.x, frameSize.y }));
    }
}

void Enemy::render(sf::RenderWindow& window) {
    if (sprite.has_value()) {
        window.draw(*sprite);
    }
}

void Enemy::setPosition(const sf::Vector2f& pos) {
    if (sprite.has_value()) {
        sprite->setPosition(sf::Vector2f( 1240 / 2.f ,  640 / 2.f ));       // do poprawy
    }
}
