#include"MyConsoleEnegine.h"

Hunter::Hunter()
{
	this->thinking = 0;
	this->move_speed = 0;
	this->run_thinking = 0;
	this->run_speed = 0;
	this->is_death = false;
}
Hunter::~Hunter()
{

}

//让猎人Blinky移动到下一个路径点
void Hunter::BMoveToNextPoint(int nextX, int nextY, std::wstring& map, int map_width, int map_height, bool* is_info_fragment)
{
	int newX = nextX;

	int newY = nextY;

	if (map[newY * map_width + newX] == '#' || newX < 0 || newX >= map_width || newY < 0 || newY >= map_height)
	{
		return;
	}

	else
	{
		if (is_info_fragment[(int)y * map_width + (int)x])
			map[(int)y * map_width + (int)x] = '.';
		else if (!is_info_fragment[(int)y * map_width + (int)x])
			map[(int)y * map_width + (int)x] = ' ';
		x = newX;
		y = newY;
		map[(int)y * map_width + (int)x] = 'B';
	}
}

//让猎人Tinky移动到下一个路径点
void Hunter::TMoveToNextPoint(int nextX, int nextY, std::wstring& map, int map_width, int map_height, bool* is_info_fragment)
{
	int newX = nextX;

	int newY = nextY;

	if (map[newY * map_width + newX] == '#' || newX < 0 || newX >= map_width || newY < 0 || newY >= map_height)
	{
		return;
	}

	else
	{
		if (is_info_fragment[(int)y * map_width + (int)x])
			map[(int)y * map_width + (int)x] = '.';
		else if (!is_info_fragment[(int)y * map_width + (int)x])
			map[(int)y * map_width + (int)x] = ' ';
		x = newX;
		y = newY;
		map[(int)y * map_width + (int)x] = 'T';
	}
}

//就算猎人与玩家之间的距离
float Hunter::CalculateDistance(float playerX, float playerY)
{
	return sqrtf(pow(x - playerX, 2) + pow(y - playerY, 2));
}
