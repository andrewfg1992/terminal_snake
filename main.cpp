#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <thread>
#include <conio.h>

bool gameOver;
int room_width = 60; // width of room including wall space
int room_height = 20; // height of room including wall space
enum eDirection { STOP = 0, LEFT = 1, RIGHT = 2, UP = 3, DOWN = 4 };
eDirection dir;

int x, y;
std::vector<int> tail_x, tail_y;
uint16_t len;
int food_x, food_y;
int score;

void setup()
{
	bool gameOver = false;
	x = room_width / 2 + 1;
	y = room_height / 2 + 1;
	dir = STOP;
	len = 1;
	score = 0;
	food_x = 1 + (std::rand() % (room_width - 2));
	food_y = 1 + (std::rand() % (room_height - 2));
	while (food_x == x && food_y == y)
	{
		// If food is staring on the snake, move it
		food_y -= 1;
	}
}

void draw()
{
	// clear the terminal / console
	std::cout << std::string(100, '\n');

	// populate the screen data
	// start with blank screen
	std::vector<std::string> screen;

	auto blank_line = std::string(room_width, ' ');

	for (int i = 0; i < room_height; i++)
	{
		screen.push_back(blank_line);
	}

	// add walls
	for (int i = 0; i < room_height; i++)
	{
		screen.at(i).at(0) = '#';
		screen.at(i).at(room_width-1) = '#';
	}
	screen.at(0) = std::string(room_width, '#');
	screen.at(room_height-1) = std::string(room_width, '#');

	// add food
	screen.at(food_y).at(food_x) = '$';

	// add snake player
	screen.at(y).at(x) = 'O';

	// add the tail
	for (int i = 0; i < tail_x.size(); i++)
	{
		screen.at(tail_y.at(i)).at(tail_x.at(i)) = 'o';
	}

	// actually print the screen
	for (int i = 0; i < screen.size(); i++)
	{
		std::cout << screen.at(i) << std::endl;
	}

	// also print score
	std::cout << "Score: " << score << std::endl;
}

void input()
{
	if (_kbhit())
	{
		switch (_getch())
		{ // the real value
			case 'w':
				dir = UP;
				break;
			case 's':
				dir = DOWN;
				break;
			case 'd':
				dir = RIGHT;
				break;
			case 'a':
				dir = LEFT;
				break;
		}
	}
}

void logic()
{
	// Manage the snake tail
	tail_x.insert(tail_x.begin(), x);
	tail_y.insert(tail_y.begin(), y);

	while(tail_x.size() >= len)
	{
		tail_x.pop_back();
	}

	while (tail_y.size() >= len)
	{
		tail_y.pop_back();
	}

	// Move the character
	switch (dir)
	{
	case UP:
		y -= 1;
		break;
	case DOWN:
		y += 1;
		break;
	case LEFT:
		x -= 1;
		break;
	case RIGHT:
		x += 1;
		break;
	}

	// if snake collides with the wall, end the game
	if ((x<=0 || x>=room_width - 1) || (y<=0 || y>=room_height - 1))
		gameOver = true;

	// Check if colliding with food
	if ((x == food_x) && (y == food_y))
	{
		score++;
		len++;

		// respawn the food
		food_x = 1 + (std::rand() % (room_width - 2));
		food_y = 1 + (std::rand() % (room_height - 2));
	}

}

int main()
{
	setup();

	while (!gameOver)
	{
		auto start = std::chrono::high_resolution_clock::now();
		input();
		logic();
		draw();
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = end - start;
		std::this_thread::sleep_for(std::chrono::microseconds(125000) - elapsed);
		std::cout << 1 + (std::rand() % (room_height - 2)) << std::endl;
	}

}