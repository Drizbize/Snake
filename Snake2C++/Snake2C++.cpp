#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

const std::string main_menu[] = {
	"   * play *",
	"* scoreboard *",
	"   * exit *"
};

const std::string lvl_menu[] = {
	"  * Go back *",
	"   * lvl 1 *",
	"   * lvl 2 *",
	"   * lvl 3 *",
	"   * lvl 4 *",
	"   * lvl 5 *"
};

int max_buttons;
int selected_button;

int width;
int height;

int x, y, fruitX, fruitY;
int tail, tailX[150], tailY[150];
int apples;
bool through_wall;
bool win;
bool loose;

int lvl;
std::vector<bool> unlocked_lvl;

bool Active_game;
enum { NONE, UP, DOWN, LEFT, RIGHT } key;
enum class PressedButton
{
	None,
	Selected,
	Up,
	Down
};
enum class state
{
	inMenu,
	inChooseLvLs,
	inGame,
	inScoreboard
};

state whereI = state::inMenu;
PressedButton pressed_button;

struct task
{
	std::string text;
	int ate_apples;
};
std::vector<task> tasks;

std::string map[] = {
	"#######################################",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#######################################"
};

const std::string LVL1[] = {
	"#######################################",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#######################################"
};

const std::string LVL2[] = {
	"#######################################",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                        ##############",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#############                         #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                        ##############",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#######################################"
};

const std::string LVL3[] = {
	"#######################################",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#         #                 #         #",
	"#         #                 #         #",
	"#         #                 #         #",
	"#         #                 #         #",
	"#         #                 #         #",
	"#         #                 #         #",
	"#         #                 #         #",
	"#         #                 #         #",
	"#         #                 #         #",
	"#         #                 #         #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#######################################"
};

const std::string LVL4[] = {
	"#######################################",
	"#                                     #",
	"#                                     #",
	"#         ###################         #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#         #                 #         #",
	"#         #                 #         #",
	"#         #                 #         #",
	"#         #                 #         #",
	"#         #                 #         #",
	"#         #                 #         #",
	"#         #                 #         #",
	"#         #                 #         #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#         ###################         #",
	"#                                     #",
	"#                                     #",
	"#######################################"
};

const std::string LVL5[] = {
	"#######################################",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#      #########       #########      #",
	"#      #                       #      #",
	"#      #                       #      #",
	"#      #                       #      #",
	"#      #                       #      #",
	"#      #                       #      #",
	"#      #                       #      #",
	"#      #                       #      #",
	"#      #                       #      #",
	"#      #                       #      #",
	"#      #                       #      #",
	"#      #########       #########      #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#                                     #",
	"#######################################"
};

void MapInitialization()
{
	for (int Y = 1; Y < height - 1; Y++)
	{
		for (int X = 1; X < width - 1; X++)
		{
			if (map[Y][X] != '#')
			{
				map[Y][X] = ' ';
			}

			if (Y == fruitY && X == fruitX)
			{
				map[fruitY][fruitX] = '%';
			}
			else if (X == x && Y == y)
			{
				map[y][x] = '@';
			}
			else
			{
				for (int i = 0; i < tail; i++)
				{
					if (X == tailX[i] && Y == tailY[i])
					{
						map[tailY[i]][tailX[i]] = 'o';
					}
				}
			}
		}
	}
}

void SetInMenu()
{
	whereI = state::inMenu;
	max_buttons = 2;
	selected_button = 0;
}

void SetInLvlMenu()
{
	whereI = state::inChooseLvLs;
	max_buttons = 5;
}

void SetInGame()
{
	whereI = state::inGame;
	win = false;
	loose = false;
}

void Initialize()
{
	SetInMenu();
	width = map->size();
	height = sizeof(map) / sizeof(map[0]);
	Active_game = true;

	unlocked_lvl.push_back(true);
	for (int i = 0; i < 4; i++)
	{
		unlocked_lvl.push_back(false);
	}

	task task_lvl1, task_lvl2, task_lvl3, task_lvl4, task_lvl5;
	task_lvl1.text = "Eat 12 apples to finish lvl";
	task_lvl1.ate_apples = 12;

	task_lvl2.text = "Eat 14 apples to finish lvl";
	task_lvl2.ate_apples = 14;

	task_lvl3.text = "Eat 18 apples to finish lvl";
	task_lvl3.ate_apples = 18;

	task_lvl4.text = "Eat 20 apples to finish lvl";
	task_lvl4.ate_apples = 20;

	task_lvl5.text = "Eat 22 apples to finish lvl";
	task_lvl5.ate_apples = 22;
	tasks.push_back(task_lvl1);
	tasks.push_back(task_lvl2);
	tasks.push_back(task_lvl3);
	tasks.push_back(task_lvl4);
	tasks.push_back(task_lvl5);
}

void SetLVL()
{
	x = width / 2;
	y = height / 2;

	do
	{
		fruitX = rand() % (width - 1) + 1;
		fruitY = rand() % (height - 1) + 1;
	} while (map[fruitY][fruitX] != ' ');

	tail = 0;

	for (int i = 0; i < 150; i++)
	{
		tailX[i] = 0;
		tailY[i] = 0;
	}
	key = NONE;
	tail = 0;
	apples = 0;
}

void SetLVL1()
{
	lvl = 1;
	through_wall = true;

	std::string tempLine;
	for (int Y = 0; Y < height; Y++)
	{
		tempLine = LVL1[Y];
		map[Y] = tempLine;
	}
	SetLVL();
	MapInitialization();
}

void SetLVL2()
{
	lvl = 2;
	through_wall = true;

	std::string tempLine;
	for (int Y = 0; Y < height; Y++)
	{
		tempLine = LVL2[Y];
		map[Y] = tempLine;
	}
	SetLVL();
	MapInitialization();
}

void SetLVL3()
{
	lvl = 3;
	through_wall = false;

	std::string tempLine;
	for (int Y = 0; Y < height; Y++)
	{
		tempLine = LVL3[Y];
		map[Y] = tempLine;
	}
	SetLVL();
	MapInitialization();
}

void SetLVL4()
{
	lvl = 4;
	through_wall = false;

	std::string tempLine;
	for (int Y = 0; Y < height; Y++)
	{
		tempLine = LVL4[Y];
		map[Y] = tempLine;
	}
	SetLVL();
	MapInitialization();
}

void SetLVL5()
{
	lvl = 5;
	through_wall = false;

	std::string tempLine;
	for (int Y = 0; Y < height; Y++)
	{
		tempLine = LVL5[Y];
		map[Y] = tempLine;
	}
	SetLVL();
	MapInitialization();
}

void RanderMenu()
{
	system("cls");
	std::string tempLine;

	for (int i = 0; i < max_buttons + 1; i++)
	{
		switch (whereI)
		{
		case state::inMenu:
			tempLine = main_menu[i];
			break;
		case state::inScoreboard:
			break;
		}

		if (selected_button == i)
		{
			int nextSymbol = 0;
			for (int j = 0; j < tempLine.size(); j++)
			{
				if (tempLine[j] == '*' && !nextSymbol)
				{
					tempLine[j] = '>';
					nextSymbol++;
				}
				if (tempLine[j] == '*' && nextSymbol)
				{
					tempLine[j] = '<';
				}
			}
		}
		else
		{
			for (int j = 0; j < tempLine.size(); j++)
			{
				if (tempLine[j] == '*')
				{
					tempLine[j] = ' ';
				}
			}
		}
		std::cout << tempLine << "\n";
	}
}

void RanderLvlMenu()
{
	system("cls");
	std::string tempLine;

	for (int i = 0; i < max_buttons + 1; i++)
	{
		tempLine = lvl_menu[i];
		if (selected_button == i)
		{
			int nextSymbol = 0;
			for (int j = 0; j < tempLine.size(); j++)
			{
				if (tempLine[j] == '*' && !nextSymbol)
				{
					tempLine[j] = '>';
					nextSymbol++;
				}
				if (tempLine[j] == '*' && nextSymbol)
				{
					tempLine[j] = '<';
				}
			}
		}
		else
		{
			for (int j = 0; j < tempLine.size(); j++)
			{
				if (tempLine[j] == '*')
				{
					if (i - 1 < unlocked_lvl.size())
					{
						if (unlocked_lvl[i - 1])
						{
							tempLine[j] = ' ';
						}
						else
						{
							tempLine[j] = 'X';
						}
					}
					else
					{
						tempLine[j] = ' ';
					}
				}
			}
		}
		std::cout << tempLine << "\n";
	}
}

void DrawMap(float MAX_FPS)
{
	HANDLE wndHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(wndHandle, { 0, 0 });

	for (int i = 0; i < height; i++)
	{
		std::cout << map[i] << "\n";
	}
	
	std::cout << "Ate apples: " << apples << "  Across wall: ";
	if (through_wall)
	{
		std::cout << "True";
	}
	else
	{
		std::cout << "False";
	}
	std::cout << "  FPS: " << MAX_FPS << "\n";

	std::cout << tasks[lvl - 1].text << "\n\n";
	if (!win && !loose)
	{
		std::cout << "(Press X to back)\n";
	}
	else if (win)
	{
		std::cout << "You won! Press X to back\n";
	}
	else if (loose)
	{
		std::cout << "You died! Press X to back and try again\n";
	}
}

void InputInGame()
{
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'a':
			if (key != RIGHT)
			{
				key = LEFT;
			}
			break;
		case 'd':
			if (key != LEFT)
			{
				key = RIGHT;
			}
			break;
		case 'w':
			if (key != DOWN)
			{
				key = UP;
			}
			break;
		case 's':
			if (key != UP)
			{
				key = DOWN;
			}
			break;
		case 'x':
			SetInMenu();
			break;
		}
	}
	if (win || loose)
	{
		while (win || loose)
		{
			switch (_getch())
			{
			case 'x':
				SetInMenu();
				win = false;
				loose = false;
				break;
			}
		}
	}
}

void InputInMenu()
{
	switch (_getch())
	{
	case 72:
		pressed_button = PressedButton::Up;
		break;
	case 80:
		pressed_button = PressedButton::Down;
		break;
	case 32:
		pressed_button = PressedButton::Selected;
		break;
	default:
		pressed_button = PressedButton::None;
		break;
	}
}

void UpdateMenu()
{
	switch (pressed_button)
	{
	case PressedButton::None:
		break;
	case PressedButton::Selected:
		switch (selected_button)
		{
		case 0:
			SetInLvlMenu();
			break;
		case 1:
			break;
		case 2:
			break;
		default:
			break;
		}
		break;
	case PressedButton::Up:
		if (selected_button > 0)
		{
			selected_button--;
		}
		break;
	case PressedButton::Down:
		if (selected_button < max_buttons)
		{
			selected_button++;
		}
		break;
	default:
		break;
	}
	pressed_button = PressedButton::None;
}

void UpdateLvlMenu()
{
	switch (pressed_button)
	{
	case PressedButton::None:
		break;
	case PressedButton::Selected:
		switch (selected_button)
		{
		case 0:
			SetInMenu();
			break;
		case 1:
			if (unlocked_lvl[0])
			{
				SetInGame();
				SetLVL1();
			}
			break;
		case 2:
			if (unlocked_lvl[1])
			{
				SetInGame();
				SetLVL2();
			}
			break;
		case 3:
			if (unlocked_lvl[2])
			{
				SetInGame();
				SetLVL3();
			}
			break;
		case 4:
			if (unlocked_lvl[3])
			{
				SetInGame();
				SetLVL4();
			}
			break;
		case 5:
			if (unlocked_lvl[4])
			{
				SetInGame();
				SetLVL5();
			}
			break;
		default:
			break;
		}
		break;
	case PressedButton::Up:
		if (selected_button > 0)
		{
			selected_button--;
		}
		break;
	case PressedButton::Down:
		if (selected_button < max_buttons)
		{
			if (unlocked_lvl[selected_button] || selected_button == 0)
			{
				selected_button++;
			}
		}
		break;
	default:
		break;
	}
	pressed_button = PressedButton::None;
}

void GameLogic()
{
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < tail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

	switch (key)
	{
	case NONE:
		break;
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

	if (through_wall)
	{
		if (x >= width - 1)
		{
			x = 1;
		}
		else if (x <= 0)
		{
			x = width - 2;
		}

		if (y >= height - 1)
		{
			y = 1;
		}
		else if (y <= 0)
		{
			y = height - 2;
		}
	}

	if (map[y][x] == '#' || map[y][x] == 'o')
	{
		loose = true;
	}

	if (fruitX == x && fruitY == y)
	{
		do
		{
			fruitX = rand() % (width - 1) + 1;
			fruitY = rand() % (height - 1) + 1;
		} while (map[fruitY][fruitX] != ' ');
		apples++;	
		tail++;
	}

	if (apples == tasks[lvl - 1].ate_apples)
	{
		unlocked_lvl[lvl] = true;
		win = true;
	}

	MapInitialization();
}

int main()
{
	srand(time(0));
	Initialize();

	while (Active_game)
	{
		const float MAX_FPS = 10.0;
		const double MAX_PERIOD = (1.0 / MAX_FPS) * 1000;

		a = std::chrono::system_clock::now();
		std::chrono::duration<double, std::milli> work_time = a - b;

		if (work_time.count() < MAX_PERIOD)
		{
			std::chrono::duration<double, std::milli> delta_ms(MAX_PERIOD - work_time.count());
			auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
			std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
		}

		b = std::chrono::system_clock::now();
		std::chrono::duration<double, std::milli> sleep_time = b - a;

		switch (whereI)
		{
		case state::inMenu:
			RanderMenu();
			InputInMenu();
			UpdateMenu();
			break;
		case state::inChooseLvLs:
			RanderLvlMenu();
			InputInMenu();
			UpdateLvlMenu();
			break;
		case state::inGame:
			DrawMap(MAX_FPS);
			InputInGame();
			GameLogic();
			//Sleep(120);
			break;
		case state::inScoreboard:
			break;
		}
	}

	return 0;
}
