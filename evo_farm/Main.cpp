#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class background{
public:
	Sprite sprite;
};

class animal {
private:
public:
	Clock clock;
	Sprite sprite;
	int x;
	int y;
	int xSM;
	int ySM;
	int wSM;
	int hSM;
	int power;
	bool edible;

	animal()
	{
		power = 0;
		edible = true;
		x = 100;
		y = 100;
		xSM = 0;
		ySM = 0;
		wSM = 101;
		hSM = 51;
		sprite.setPosition(x, y);
	}

	animal(int x0, int y0)
	{
		power = 0;
		edible = true;
		x = x0;
		y = y0;
		xSM = 0;
		ySM = 0;
		wSM = 101;
		hSM = 51;
		sprite.setPosition(x, y);
	}

	virtual void move(int dx, int dy)
	{

	};

	void getAte(int butt_x, int butt_y)
	{
		edible = false;
		ySM = 104;
		x = butt_x;
		y = butt_y;
		sprite.setPosition(x, y);
		sprite.setTextureRect(IntRect(xSM, ySM, wSM, hSM));
	}
};

class player:public animal {
public:
	void move(int dx, int dy)
	{


		if (clock.getElapsedTime().asMilliseconds() > 200)
		{
			x += 5 * dx;
			y += 5 * dy;
			sprite.setPosition(x, y);
			xSM = abs((xSM + 102) % 204);
			sprite.setTextureRect(IntRect(xSM, ySM, wSM, hSM));
			clock.restart();
		};
	};

	void check_keys()
	{
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			move(0, 1);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			move(0, -1);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			move(1, 0);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			move(-1, 0);
		}
	}
};


int main()
{
	RenderWindow window(sf::VideoMode(800, 600), "Title");
	Event event;

	Texture texture;
	texture.loadFromFile("C:/Users/jsten/Documents/games/evo_farm/evo_farm/layer0.gif");
	Texture sprite_map;
	sprite_map.loadFromFile("C:/Users/jsten/Documents/games/evo_farm/evo_farm/sprite_map.png");

	background layer0;
	layer0.sprite.setTexture(texture);

	player cow;
	cow.sprite.setTexture(sprite_map);
	cow.power = 20;
	cow.sprite.setTextureRect(IntRect(cow.xSM, cow.ySM, cow.wSM, cow.hSM));

	animal grass(200,200);
	grass.ySM = 52;
	grass.power = 1;
	grass.sprite.setTexture(sprite_map);
	grass.sprite.setTextureRect(IntRect(grass.xSM, grass.ySM, grass.wSM, grass.hSM));



	while (window.isOpen())
	{
		
		cow.check_keys();
		

		/*Collisions*/
		if (abs(cow.x - grass.x) < 30)
		{
			if (abs(cow.y - grass.y) < 50)
			{
				if (cow.power > grass.power)
				{
					if (grass.edible)
					{
						grass.getAte(cow.x - 50, cow.y);
					}
				}
				else
				{
					if (cow.edible)
					{
						cow.getAte(grass.x - 50, grass.y);
					}
				}
			}
		}


		window.clear();
		window.draw(layer0.sprite);
		window.draw(cow.sprite);
		window.draw(grass.sprite);
		window.display();















		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

	}
	return 0;
}