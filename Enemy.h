#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "Eq.h"
class Enemy  {
public:
    Enemy();
    Eq eq;
    void update(float deltaTime, sf::Vector2f playerPos);
    void render(sf::RenderWindow& window);
    void setPosition();
	void FollowPlayer(const sf::Vector2f& playerPos, float deltaTime);  // dodane do �ledzenia gracza
	bool isAttacking = false; // czy wr�g atakuje
	float attackTimer = 0.f; // czas ataku
    int currentRow = 0;
	int attackframeCount = 6; // liczba klatek animacji ataku
	float attackDuration = 0.5f; // czas trwania ataku
	int attackDirection = 0; // kierunek ataku (0 - brak, 1 - w prawo, 2 - w lewo, 3 - w d�, 4 - w g�r�)
	bool isFacingLeft = false; // czy wr�g patrzy w lewo
	sf::Vector2f lastMovement{ 0.f, 1.f }; // domy�lnie w d�
    void Attack(int attackdirection);

	

private:
    sf::Texture texture;
    std::optional<sf::Sprite> sprite;

    sf::Vector2i frameSize = {192, 192 };  // rozmiar pojedynczej klatki animacji
    int frameCount = 6;                 // liczba klatek w animacji (w pierwszym wierszu)
    int currentFrame = 0;
    float frameTime = 0.15f;            // czas na jedn� klatk� (sekundy)
    float frameTimer = 0.f;
};