#include "Functions.h"


int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_X, SCREEN_Y), "My window");
	sf::Clock clock;
	sf::Clock cd_ctr;
	sf::Texture texture;
	sf::SoundBuffer bang_buff;
	sf::SoundBuffer las_buff;
	sf::SoundBuffer off_buff;
	sf::Sound bang;
	sf::Sound laser;
	sf::Sound off;
	sf::Music starwars;


	int x = 100, y = 0, amount = 0;
	double k;
	int count;
	int size = 10;
	std::pair <sf::Vector2f, sf::Vector2f> coord;
	std::vector <Shots> bullets(size);

	bang_buff.loadFromFile("blast.wav");
	bang.setBuffer(bang_buff);

	starwars.openFromFile("stars_long.wav");
	

	las_buff.loadFromFile("laser.wav");
	laser.setBuffer(las_buff);
	laser.setLoop(false);

	off_buff.loadFromFile("l_off.wav");
	off.setBuffer(off_buff);
	off.setLoop(false);

	texture.loadFromFile("Falcon.png");
	
	sf::Sprite fighter(texture);
	fighter.setPosition(250, 250);
	
	sf::CircleShape missle(MISSLE_RAD);
	missle.setFillColor(sf::Color::Red);
	missle.setOrigin(MISSLE_RAD, MISSLE_RAD);
	
	
	
	while (window.isOpen())
	{
		window.clear(sf::Color::Black);
		sf::Time time = clock.getElapsedTime();
		coord = MouseOrientation(fighter, window, k);

		Music(starwars);
		
		Motion (fighter);
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (laser.getStatus() != sf::Sound::Playing)
				laser.play();

			sf::Vertex line[] =
			{
				sf::Vertex(coord.first),
				LaserDirection(coord.first, coord.second)
			};
			line[0].color = sf::Color::Red;
			line[1].color = sf::Color::Red;
			window.draw(line, 2, sf::Lines);
		}
		
		else
		{
			if (laser.getStatus() == sf::Sound::Playing)
			{
				laser.stop();
				off.play();
			}
		}
	
				
		if ((sf::Mouse::isButtonPressed(sf::Mouse::Right)) &&
			(cd_ctr.getElapsedTime().asMilliseconds() > COOL_DOWN))
		{
			bang.stop();
			bang.play();
			cd_ctr.restart();
			bullets[amount].k = k;
			bullets[amount].start = coord.first;
			bullets[amount].dest = coord.second - coord.first;
			bullets[amount].timer.restart();
			amount++;
			if (amount + 1 == size)
			{
				size *= 2;
				bullets.resize(size);
			}
		}
		
	
		for (count = 0; count < amount; count++)
		{
			missle.setPosition(bullets[count].start.x + 
			calc(bullets[count]).first, bullets[count].start.y + calc(bullets[count]).second);
			window.draw(missle);
		}
			
		window.draw(fighter);
		
		sf::Event event;
		while (window.pollEvent(event));
		{
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}
		window.display();
	}
	bullets.clear();
	system("PAUSE");
	return 0;
}



