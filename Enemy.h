#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class Enemy {
public:
    Enemy();

    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void setPosition(const sf::Vector2f& pos);

private:
    sf::Texture texture;
    std::optional<sf::Sprite> sprite;

    sf::Vector2i frameSize = {192, 192 };  // rozmiar pojedynczej klatki animacji
    int frameCount = 7;                 // liczba klatek w animacji (w pierwszym wierszu)
    int currentFrame = 0;
    float frameTime = 0.15f;            // czas na jedn¹ klatkê (sekundy)
    float frameTimer = 0.f;
};