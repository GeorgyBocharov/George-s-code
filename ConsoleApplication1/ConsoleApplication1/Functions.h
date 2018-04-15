#include <SFML\Graphics.hpp>
#include <iostream>
#include <SFML\Audio\Sound.hpp>
#include <SFML\Audio\SoundBuffer.hpp>
#include <SFML\Audio\Music.hpp>


const int SCREEN_X = 800;
const int SCREEN_Y = 800;
const int HALF_SIZE = 128;
const int DELTA = 5;
const int COOL_DOWN = 1500;
const int MISSLE_RAD = 6;
const float SPEED = 0.8;
const float Pi = 3.14159f;

struct Shots
{
	/*
	Неясно из названия, что значит k.
	Точно ли необходимо хранить именно объект таймер для каждого выстрела?
	*/
	double k;
	sf::Vector2f start;
	sf::Vector2f dest;
	sf::Clock timer;
};

sf::Vector2f LaserDirection(sf::Vector2f, sf::Vector2f);

// fixit: названия ф-й и методов классов - глаголы
void Motion(sf::Sprite&);
std::pair <sf::Vector2f, sf::Vector2f> MouseOrientation(sf::Sprite&, sf::RenderWindow&, double&);
int norm(float);
std::pair <double, double> calc(Shots data);
void Music (sf::Music& background);



void Motion(sf::Sprite &figure)
{
	// У вас движение персонажа зависит от fps в игре ... т.к. при нажатой клавише движения каждый кадр сдвигаетесь на 1 пиксель.
	// И чем больше fps, тем быстрее персонаж перемещается. Так быть не должно.
	// В упражнении пойдет, но в проекте надо исправить эту проблему.
	// Нужно задать максимальную скорость движения - константу ... и на каждом кадре вычислять dt между кадрами
	
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && (figure.getPosition().y > HALF_SIZE))
		figure.move(0, -1);

	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && (figure.getPosition().x > HALF_SIZE))
		figure.move(-1, 0);

	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && (figure.getPosition().x < SCREEN_X - HALF_SIZE))
		figure.move(1, 0);

	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && (figure.getPosition().y < SCREEN_Y - HALF_SIZE))
		figure.move(0, 1);
}

std::pair <sf::Vector2f, sf::Vector2f> MouseOrientation(sf::Sprite &fighter, sf::RenderWindow& window, double&k)
{
	std::pair <sf::Vector2f, sf::Vector2f> pair;
	sf::Vector2u size = fighter.getTexture()->getSize();
	fighter.setOrigin(size.x / 2, size.y / 2);
	sf::Vector2i mouse = sf::Mouse::getPosition(window);
	sf::Vector2f center = fighter.getPosition();
	sf::Vector2f ptr = sf::Vector2f(mouse.x, mouse.y) - center;
	fighter.setRotation(270 + atan2f(ptr.y, ptr.x) * 180 / Pi);
	pair = std::make_pair(center, ptr + center);
	k = (pair.first.y - pair.second.y) / (pair.first.x - pair.second.x);
	return pair;
}

sf::Vector2f LaserDirection(sf::Vector2f ship, sf::Vector2f mouse)
{
	double  b, k;
	sf::Vector2f point;
	// а что с вертикальной прямой делать ?) обычно уравнение прямой в виде ax + by + c = 0 записывают, что такой проблемы не было
	k = (ship.y - mouse.y) / (ship.x - mouse.x);
	b = ship.y - k * ship.x;

	if (k > 0)
	{
		if (ship.x >= mouse.x)
			point.x = 0 - DELTA;
		else
			point.x = SCREEN_X + DELTA;
		point.y = k*point.x + b;
	}
	else
	{
		if (ship.x >= mouse.x)
			point.x = 0 - DELTA;
		else
			point.x = SCREEN_X + DELTA;
		point.y = k*point.x + b;
	}
	return point;
}

// это называется sign , а не norm ...
// return coord > 0 ? 1 : -1;
int norm(float coord)
{
	if (coord > 0)
		return 1;
	else
		return -1;
}

std::pair <double, double> calc(Shots data)
{
	double help = data.timer.getElapsedTime().asMilliseconds() /
		sqrt(1 + pow(data.k, 2)) * SPEED;
	double x = help * norm(data.dest.x);
	double y = help * abs(data.k) * norm(data.dest.y);
	std::pair <double, double> res = std::make_pair(x, y);
	return res;
}

void Music (sf::Music& background)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		if (background.getStatus() != sf::Sound::Playing)
			background.play();
	}
	
	else 
		background.pause();
}
