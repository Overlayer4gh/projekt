#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "Eq.h"
#include "Player.h"
class Enemy  {
public:
    Enemy();
    Eq eq;
    
	bool isAttacking = false; // czy wróg atakuje
	float attackTimer = 0.f; // czas ataku
    int currentRow = 0;
	int attackframeCount = 6; // liczba klatek animacji ataku
	float attackDuration = 0.5f; // czas trwania ataku
	int attackDirection = 0; // kierunek ataku (0 - brak, 1 - w prawo, 2 - w lewo, 3 - w dó³, 4 - w górê)
	bool isFacingLeft = false; // czy wróg patrzy w lewo
	sf::Vector2f lastMovement{ 0.f, 1.f }; // domyœlnie w dó³
    bool wasDamagedThisAttack = false; // flaga jednorazowego trafienia
	bool isDead = false; // czy wróg jest martwy
	float cooldown = 1.f; // czas odnowienia ataku
	float attackCooldown = 1.f; // czas odnowienia ataku (w sekundach)
    bool isIdle = true;
    sf::Vector2f previousPosition;


    void Attack(int attackdirection);
    virtual void update(float deltaTime, sf::Vector2f playerPos, Player& player);
    virtual void render(sf::RenderWindow& window);
    void setPosition();
    void FollowPlayer(const sf::Vector2f& playerPos, float deltaTime);  // dodane do œledzenia gracza
	sf::Vector2f getPosition() const { return sprite->getPosition(); } // dodane do pobierania pozycji wroga
	sf::FloatRect getBounds() const; // zwraca prostok¹t reprezentuj¹cy granice wroga


    

	

private:
    sf::Texture texture;
    std::optional<sf::Sprite> sprite;

    sf::Vector2i frameSize = {192, 192 };  // rozmiar pojedynczej klatki animacji
    int frameCount = 6;                 // liczba klatek w animacji (w pierwszym wierszu)
	int currentFrame = 0; 		  // aktualna klatka animacji
    float frameTime = 0.1f;            // czas na jedn¹ klatkê (sekundy)
	float frameTimer = 0.f; // licznik czasu dla animacji

    friend class Boss;
};