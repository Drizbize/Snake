#include <iostream>
#include <conio.h>

int width;
int height;

int foodX;
int foodY;

int x;
int y;

int score;
bool gameActive = true;
enum { NONE, UP, DOWN, LEFT, RIGHT } key;

char **map;

void initialization()
{
	width = 32;
	height = 22;

	x = width / 2;
	y = height / 2;

	foodX = rand() % width;
	foodY = rand() % height;

	score = 0;
	gameActive = true;

	map = new char*[width];

	for (int i = 0; i < height; i++)
	{
		map[i] = new char[width + 1];
		if (i == 0 || i == height - 1)
		{
			for (int j = 0; j <= width; j++)
			{
				map[i][j] = '#';

				if (j == width)
				{
					map[i][j] = '\0';
				}
			}
		}
		else
		{
			for (int j = 0; j <= width; j++)
			{
				if (j == 0 || j == width - 1)
				{
					map[i][j] = '#';
				}
				else
				{
					map[i][j] = ' ';
				}

				map[x][y] = '@';

				if (j == width)
				{
					map[i][j] = '\0';
				}
			}
		}		
	}
}

void Draw_map()
{
	system("cls");

	for (int i = 0; i < height; i++)
	{
		std::cout << map[i] << "\n";
	}
}

void keyboardInput()
{
	if (_kbhit())
	{
		switch (_getch())
		{
			case 'a':
			{
				key = LEFT;
				break;
			}
			case 'd':
			{
				key = RIGHT;
				break;
			}
			case 'w':
			{
				key = UP;
				break;
			}
			case 's':
			{
				key = DOWN;
				break;
			}
			case 'x':
			{
				gameActive = false;
				break;
			}
		}
	}
}

void gameLogic()
{
	switch (key)
	{
	case NONE:
		return;
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	}

	if (x < 0 || y < 0 || x > width || y > height)
	{
		gameActive = false;
	}

	if (foodX == x || foodY == y)
	{
		score = +1;
	}
}

int main()
{
	initialization();

	while (gameActive)
	{
		Draw_map();
		gameLogic();
	}
	return 0;
}