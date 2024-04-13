#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>
#include <iostream>
#include <list>
#include <random>

#include "utils.h"

using namespace std;

const sf::Vector2i BORDER_ADJUST = {-45, -65};

sf::FloatRect bounds(const sf::RectangleShape& shape);

sf::Vector2f collisionPoint(const sf::Sprite& sprite);

double dot(const sf::Vector2f& a, const sf::Vector2f& b);

double length(const sf::Vector2f& vec);

void makeObstacle(list<sf::Sprite>& obstacles, const sf::Texture& texture);

int main() {
	srand(time(nullptr));
	sf::RenderWindow window(sf::VideoMode(1200, 800), "Window");

	window.setPosition(sf::Vector2i(0, 0));

	sf::RectangleShape floor(sf::Vector2f(1200, 200));
	floor.setFillColor(sf::Color::Green);
	floor.setPosition(0, 600);

	array<sf::Texture, 4> textures;
	for (int i = 0; i < 4; i++) {
		sf::Texture texture;
		texture.loadFromFile("sprite" + to_string(i) + ".png");
		textures[i] = texture;
	}

	sf::Sprite player(textures[0]);
	player.setScale(0.3, 0.3);
	player.setPosition(50, 600 - textures[0].getSize().y * 0.3);

	sf::Texture obstacleTexture;
	obstacleTexture.loadFromFile("obstacle.png");

	float momentum = 0;
	int score = 0, textureIdx = 0, frameCount = 1;

	sf::Font font;
	font.loadFromFile("arial.ttf");

	list<sf::Sprite> obstacles;

	sf::Text scoreText("Score: " + to_string(score), font);
	scoreText.setPosition(0, 0);

	while (window.isOpen()) {
		window.clear();
		window.draw(floor);
		window.draw(player);
		window.draw(scoreText);

		for (auto& obstacle : obstacles) {
			window.draw(obstacle);
		}

		window.display();

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				cout << "Closed" << endl;
				return 0;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			player.setPosition(player.getPosition() + sf::Vector2f(-1, 0));
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			player.setPosition(player.getPosition() + sf::Vector2f(1, 0));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player.getGlobalBounds().intersects(bounds(floor))) {
			momentum = 4;
		}

		player.setPosition(player.getPosition() - sf::Vector2f(0, momentum));
		frameCount++;
		if (frameCount % 50 == 0) {
			textureIdx = (textureIdx + 1) % 4;
			player.setTexture(textures[textureIdx]);
			frameCount = 1;
		}

		if (!player.getGlobalBounds().intersects(bounds(floor))) {
			momentum -= 0.05;
		} else {
			momentum = 0;
		}

		for (auto it = obstacles.begin(); it != obstacles.end(); it++) {
			it->setPosition(it->getPosition() - sf::Vector2f(2, 0));

			if (it->getPosition().x <= 0) {
				obstacles.erase(it);
				it = obstacles.begin();
				score++;
				scoreText.setString("Score: " + to_string(score));
			}
		}

		if (player.getPosition().y > 1000) {
			sf::Text gameOverText("Game Over", font, 72);
			gameOverText.setFillColor(sf::Color::Red);
			gameOverText.setPosition(sf::Vector2f(425, 300));
			window.draw(gameOverText);
			window.display();
			sf::sleep(sf::seconds(2.5));
			window.close();
			return 0;
		}

		for (auto& obstacle : obstacles) {
			if (player.getGlobalBounds().intersects(obstacle.getGlobalBounds())) {
				sf::Text gameOverText("Game Over", font, 72);
				gameOverText.setFillColor(sf::Color::Red);
				gameOverText.setPosition(sf::Vector2f(425, 300));
				window.draw(gameOverText);
				window.display();
				sf::sleep(sf::seconds(2.5));
				window.close();
				return 0;
			}
		}

		if (random() % 1000 < 2) {
			makeObstacle(obstacles, obstacleTexture);
		}
	}

	return 0;
}

sf::FloatRect bounds(const sf::RectangleShape& shape) { return sf::FloatRect(shape.getPosition(), shape.getSize()); }

sf::Vector2f collisionPoint(const sf::Sprite& sprite) {
	sf::FloatRect boundingRect = sprite.getTransform().transformRect(sprite.getLocalBounds());
	return sf::Vector2f(boundingRect.left + boundingRect.width, boundingRect.top + boundingRect.height);
}

double dot(const sf::Vector2f& a, const sf::Vector2f& b) { return a.x * b.x + a.y * b.y; }

double length(const sf::Vector2f& vec) { return sqrt(dot(vec, vec)); }

void makeObstacle(list<sf::Sprite>& obstacles, const sf::Texture& texture) {
	sf::Sprite obstacle(texture);
	obstacle.setScale(0.2, 0.2);
	obstacle.setPosition(sf::Vector2f(1200, 600) - sf::Vector2f(texture.getSize()) * 0.2f);

	obstacles.push_back(obstacle);
}