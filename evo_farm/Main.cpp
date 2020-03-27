#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <vector>
#include <iterator>

using namespace sf;
using namespace std;

int sign(int x)
{
	if (x > 0) return 1;
	if (x < 0) return -1;
	return 0;
}


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
	bool moveable;
	int follow;

	animal()
	{
		power = 0;
		edible = true;
		x = 100;
		y = 100;
		xSM = 1;
		ySM = 201;
		wSM = 99;
		hSM = 99;
		sprite.setPosition(x, y);
	}

	animal(int x0, int y0)
	{
		power = 0;
		edible = true;
		x = x0;
		y = y0;
		xSM = 201;
		ySM = 99;
		wSM = 99;
		hSM = 99;
		sprite.setPosition(x, y);
	}

	virtual void move(int dx, int dy)
	{

	};

	virtual void update(animal a1)
	{

	};

	virtual void getAte(animal Eater)
	{

	}
};

class player:public animal {
public:
	player()
	{
		follow = 45;
	}
	void move(int dx, int dy)
	{
		if (clock.getElapsedTime().asMilliseconds() > 200)
		{
			x += 5 * dx;
			y += 5 * dy;
			if (y > 40)
			{
				if (x > 40)
				{
					if (x < 640)
					{
						if (y < 440)
						{
							sprite.setPosition(x, y);
							xSM = abs((xSM + 100) % 400);
							sprite.setTextureRect(IntRect(xSM, ySM, wSM, hSM));
						}
						else(y = 439);
					}
					else(x = 639);
				}
				else(x = 41);
			}
			else(y = 41);
			clock.restart();
		};
	};

	void check_keys()
	{
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			move(0, 2);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			move(0, -2);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			move(2, 0);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			move(-2, 0);
		}
	}
	void update(animal p1)
	{
		check_keys();
	};

	void getAte(animal Eater)
	{
		ySM = Eater.ySM + (1 - rand() % 3) * 100;
		if (ySM < 199)
		{
			ySM = 201;
		}
		if (ySM > 1010)
		{
			ySM = 1001;
		}
		x = Eater.x - 30;
		y = Eater.y + 20;
		power = ySM;
		follow = Eater.follow - 1 + rand() % 3;
		moveable = true;
		sprite.setPosition(x, y);
		sprite.setTextureRect(IntRect(xSM, ySM, wSM, hSM));
	}
};


class NPC :public animal {
public:
	int px;
	int py;
	bool has_parent;
	NPC(int x0, int y0)
	{
		power = 0;
		edible = true;
		x = x0;
		y = y0;
		px = 0;
		py = 0;
		xSM = 1;
		ySM = 201;
		wSM = 99;
		hSM = 99;
		follow = 0;
		sprite.setPosition(x, y);
		moveable = true;
	}

	void move(int dx, int dy)
	{
		if (clock.getElapsedTime().asMilliseconds() > 200)
		{
			px += dx;
			py += dy;
			x += 5 * dx + px;
			y += 5 * dy + py;
			if (y > 40)
			{
				if (x > 40)
				{
					if (x < 640)
					{
						if (y < 440)
						{
							sprite.setPosition(x, y);
							xSM = abs((xSM + 100)  % 400);
							sprite.setTextureRect(IntRect(xSM, ySM, wSM, hSM));
						}
						else { y = 439; py = -2; };
					}
					else { x = 639; px = -2; };
				}
				else { x = 41; px = 2; };
			}
			else { y = 41; py = 2; };
			clock.restart();
		};
	};

	void getAte(animal Eater)
	{
		ySM = Eater.ySM + (1 - rand() % 3)*100;
		if (ySM < 199)
		{
			ySM = 201;
		}
		if (ySM > 1010)
		{
			ySM = 1001;
		}
		x = Eater.x - 30;
		y = Eater.y + 20;
		power = ySM;
		follow = Eater.follow - 1 + rand() % 3;
		moveable = true;
		sprite.setPosition(x, y);
		sprite.setTextureRect(IntRect(xSM, ySM, wSM, hSM));
	}

	void update(animal p1)
	{
		int dx = 0;
		int dy = 0;
		if (follow > rand() % 100)
		{
			dx = sign(p1.x - x);
			dy = sign(p1.y - y);
		}
		else
		{
			dx = 1 - rand() % 3 ;
			dy = 1 - rand() % 3 ;
		}

		if (moveable == true)
		{
			move(dx, dy);
		}
	};
};

class god {
public:
	void collision_check(animal &A1, animal &A2, animal* parents[], int i, int j)
	{
		if (abs(A1.x - A2.x) < 30)
		{
			if (abs(A1.y - A2.y) < 50)
			{
				if (A1.power > A2.power+20)
				{
					if (A2.edible)
					{
						A2.getAte(A1);
						parents[j] = &A1;
					}
				}
				else if(A2.power > A1.power + 20)
				{
					if (A1.edible)
					{
						A1.getAte(A2);
						parents[i] = &A2;
					}
				}
			}
		}
	}
	void check_all_collisions(animal* ents[], animal* parents[], int L)
	{
		for (int i = 0; i < L; i++)
		{
			for (int j = i + 1; j < L; j++)
			{
				collision_check(*ents[i], *ents[j], parents, i, j);
			}
		}
	}

	void update_all(animal *ents[], animal* parents[],int L)
	{
		for (int i = 0; i < L; i++)
		{
			(*ents[i]).update(*parents[i]);
		}
	}
};


int main()
{
	RenderWindow window(sf::VideoMode(800, 600), "Title");
	Event event;

	god gamegod;

	Texture texture;
	texture.loadFromFile("C:/Users/jsten/Documents/games/evo_farm/evo_farm/layer0.gif");
	Texture sprite_map;
	sprite_map.loadFromFile("C:/Users/jsten/Documents/games/evo_farm/evo_farm/sprite_map.png");

	background layer0;
	layer0.sprite.setTexture(texture);

	player cow;
	cow.sprite.setTexture(sprite_map);
	cow.power = 25;
	cow.ySM = 301;
	cow.sprite.setTextureRect(IntRect(cow.xSM, cow.ySM, cow.wSM, cow.hSM));

	NPC grass(200,200);
	grass.ySM = 201;
	grass.moveable = false;
	grass.power = 1;
	grass.sprite.setTexture(sprite_map);
	grass.sprite.setTextureRect(IntRect(grass.xSM, grass.ySM, grass.wSM, grass.hSM));

	NPC g2(400, 350);
	g2.ySM = 201;
	g2.moveable = false;
	g2.power = 1;
	g2.sprite.setTexture(sprite_map);
	g2.sprite.setTextureRect(IntRect(g2.xSM, g2.ySM, g2.wSM, g2.hSM));

	NPC g3(350, 200);
	g3.ySM = 201;
	g3.moveable = false;
	g3.power = 1;
	g3.sprite.setTexture(sprite_map);
	g3.sprite.setTextureRect(IntRect(g3.xSM, g3.ySM, g3.wSM, g3.hSM));

	NPC g4(250, 300);
	g4.ySM = 201;
	g4.moveable = false;
	g4.power = 1;
	g4.sprite.setTexture(sprite_map);
	g4.sprite.setTextureRect(IntRect(g4.xSM, g4.ySM, g4.wSM, g4.hSM));

	NPC g5(450, 150);
	g5.ySM = 201;
	g5.moveable = false;
	g5.power = 1;
	g5.sprite.setTexture(sprite_map);
	g5.sprite.setTextureRect(IntRect(g5.xSM, g5.ySM, g5.wSM, g5.hSM));

	NPC g6(420, 350);
	g6.ySM = 201;
	g6.moveable = false;
	g6.power = 1;
	g6.sprite.setTexture(sprite_map);
	g6.sprite.setTextureRect(IntRect(g6.xSM, g6.ySM, g6.wSM, g6.hSM));

	NPC g7(150, 380);
	g7.ySM = 201;
	g7.moveable = false;
	g7.power = 1;
	g7.sprite.setTexture(sprite_map);
	g7.sprite.setTextureRect(IntRect(g7.xSM, g7.ySM, g7.wSM, g7.hSM));

	NPC g8(370, 80);
	g8.ySM = 201;
	g8.moveable = false;
	g8.power = 1;
	g8.sprite.setTexture(sprite_map);
	g8.sprite.setTextureRect(IntRect(g8.xSM, g8.ySM, g8.wSM, g8.hSM));

	NPC fox(450, 450);
	fox.sprite.setTexture(sprite_map);
	fox.power = 75;
	fox.ySM = 501;
	fox.sprite.setTextureRect(IntRect(fox.xSM, fox.ySM, fox.wSM, fox.hSM));


	animal* ents[10] = { &cow, &grass, &g2, &g3, &g4, &g5, &g6, &g7, &g8, &fox };
	animal* parents[10] = { &cow, &grass, &g2, &g3, &g4, &g5, &g6, &g7, &g8, &fox };

	while (window.isOpen())
	{
		gamegod.update_all(ents, parents, size(ents));
		

		/*Collisions*/ 
		gamegod.check_all_collisions(ents,parents,size(ents));
		


		window.clear();
		window.draw(layer0.sprite);
		//window.draw(cow.sprite);
		//window.draw(grass.sprite);
		//window.draw(fox.sprite);
		for (int i = 0; i < size(ents); i++)
		{
			window.draw((*ents[i]).sprite);
		}
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