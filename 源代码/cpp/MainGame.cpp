#include"Game.h"

using namespace Game;

namespace Game
{
	//计时器 创建两个时间点
	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();
	//以秒为单位接受经过的时间   一帧
	float elapsed_timeS;
	//玩家存活时间
	auto tp3 = std::chrono::system_clock::now();
	auto tp4 = std::chrono::system_clock::now();
	std::chrono::seconds live_time;
	//计算打穿墙壁能力的时间间隔
	auto tp5 = std::chrono::system_clock::now();
	std::chrono::seconds punch_time;
	//进行生成迷宫的挖掘方向
	enum Direction
	{
		UP, DOWN, LEFT, RIGHT, NO
	};
	//迷宫单元
	struct Cell
	{
		bool visited;
		bool walls[4];
		Cell()
		{
			visited = false;
			for (int i = 0; i < 4; i++)
				walls[i] = true;
		}
	};
	//迷宫
	std::vector<std::vector<Cell>>maze;
}

//构造和析构函数
MainGameClass::MainGameClass()
{
	ROW = 14;
	COL = 14;
	map_width = ROW * 2 + 1;
	map_height = COL * 2 + 1;
	map_is_open = false;
	player.depth = (float)map_height;
	screen = new wchar_t[screen_width * screen_height];
	attribute_array = new WORD[screen_width * screen_height];
	is_info_fragment = new bool[screen_width * screen_height];
}
MainGameClass::~MainGameClass()
{
	delete[] screen;
	delete[] attribute_array;
	delete[] is_info_fragment;
}

//更新并渲染游戏进程
void MainGameClass::UpdateAndRenderMainGame()
{
	//计算帧率
	tp2 = std::chrono::system_clock::now();
	std::chrono::duration<float>elapsed_time = tp2 - tp1;
	tp1 = tp2;
	elapsed_timeS = elapsed_time.count();
	//计算玩家存活时间
	tp4 = std::chrono::system_clock::now();
	live_time = std::chrono::duration_cast<std::chrono::seconds>(tp4 - tp3);
	//计算时间间隔
	if (!player.is_punch)
	{
		punch_time = std::chrono::duration_cast<std::chrono::seconds>(tp4 - tp5);
		if (num_info_fragment != 0)
		{
			if (punch_time == (std::chrono::seconds)8)
				player.is_punch = 1;
		}
		else if (num_info_fragment == 0)
		{
			if (punch_time == (std::chrono::seconds)3)
				player.is_punch = 1;
		}

	}
	switch (main_game_state)
	{
	case GameState::MainGame::GS_GameInit:
	{
		GameInit();
	}break;
	case GameState::MainGame::GS_GamePlaying:
	{
		GamePlaying();
	}break;
	case GameState::MainGame::GS_GamePuase:
	{
		GamePause();
	}break;
	}
}



//游戏开始前的初始化
void MainGameClass::GameInit()
{
	player_direction = 0;
	num_hunter = 2;
	player.is_punch = 1;
	Blinky.is_death = false;
	Tinky.is_death = false;
	can_open_map = 0;
	collect_info = 0;
	//初始化信息碎片数组
	for (int x = 0; x < map_width; x++)
	{
		for (int y = 0; y < map_height; y++)
		{
			is_info_fragment[y * map_width + x] = false;
		}
	}
	num_info_fragment = 0;
	//将字符属性数组都设置为白色
	for (int i = 0; i < screen_width * screen_height; i++)
		attribute_array[i] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	//设置随机数种子
	srand((unsigned)time(NULL));
	for (int i = 0; i < ROW; i++)
		maze.push_back(std::vector<Cell>(COL));
	GenerateMaze(0, 0);
	SetMap();
	GenerateBlincy();
	GenerateTinky();
	GeneratePlayerPos();
	//开始游戏
	tp3 = std::chrono::system_clock::now();
	main_game_state = GameState::MainGame::GS_GamePlaying;
}

//生成迷宫  使用递归回溯算法
void MainGameClass::GenerateMaze(int row, int col)
{
	maze[row][col].visited = true;
	std::vector<Direction>directions = { UP,DOWN,LEFT,RIGHT };

	std::random_shuffle(directions.begin(), directions.end());

	for (auto& direction : directions)
	{
		int new_row = row, new_col = col;
		switch (direction)
		{
		case UP:
			new_row--;
			break;
		case DOWN:
			new_row++;
			break;
		case LEFT:
			new_col--;
			break;
		case RIGHT:
			new_col++;
			break;
		}
		if (new_row < 0 || new_row >= ROW || new_col < 0 || new_col >= COL || maze[new_row][new_col].visited)
			continue;
		switch (direction)
		{
		case UP:
		{
			maze[row][col].walls[0] = false;
			maze[new_row][new_col].walls[1] = false;
		}break;
		case DOWN:
		{
			maze[row][col].walls[1] = false;
			maze[new_row][new_col].walls[0] = false;
		}break;
		case LEFT:
		{
			maze[row][col].walls[2] = false;
			maze[new_row][new_col].walls[3] = false;
		}break;
		case RIGHT:
		{
			maze[row][col].walls[3] = false;
			maze[new_row][new_col].walls[2] = false;
		}break;
		}
		GenerateMaze(new_row, new_col);
	}
}

//给地图赋值
void MainGameClass::SetMap()
{
	//地图字符赋值
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			map += L"#";
			if (maze[i][j].walls[0])
				map += L"#";
			else
				map += L".";
		}
		map += L"#";
		for (int j = 0; j < COL; j++)
		{
			if (maze[i][j].walls[2])
				map += L"#";
			else
				map += L".";
			map += L".";

			if (j == COL - 1)
				if (maze[i][j].walls[3])
					map += L"#";
				else
					map += L".";
		}
	}
	for (int i = 0; i < map_width; i++)
		map += L"#";

	//给信息碎片数组赋值
	for (int x = 0; x < map_width; x++)
	{
		for (int y = 0; y < map_height; y++)
		{
			if (map[y * map_width + x] == '.')
			{
				is_info_fragment[y * map_width + x] = true;
				num_info_fragment++;
			}
		}
	}
}

//检查是否有墙壁
bool MainGameClass::CheckWall(int x, int y)
{
	if (map[y * map_width + x] == '#')
		return false;
	return true;
}

//生成猎人Blinky
void MainGameClass::GenerateBlincy()
{
	while (1)
	{
		Blinky.x = rand() % map_width;
		Blinky.y = rand() % map_height;
		if (CheckWall((int)Blinky.x, (int)Blinky.y) && Blinky.CalculateDistance(player.x, player.y) > 5)
		{
			map[Blinky.y * map_width + Blinky.x] = 'B';
			break;
		}
	}
}

//生成猎人Tinky
void MainGameClass::GenerateTinky()
{
	while (1)
	{
		Tinky.x = rand() % map_width;
		Tinky.y = rand() % map_height;
		if (CheckWall((int)Tinky.x, (int)Tinky.y) && Blinky.CalculateDistance(player.x, player.y) > 5)
		{
			map[Tinky.y * map_width + Tinky.x] = 'T';
			break;
		}
	}
}

//随机生成玩家的位置
void MainGameClass::GeneratePlayerPos()
{
	player.x = rand() % map_width;
	player.y = rand() % map_height;
	while (map[player.y * map_width + player.x] == '#' || Blinky.CalculateDistance(player.x, player.y) <= 15)
	{
		player.x = rand() % map_width;
		player.y = rand() % map_height;
	}
}


//游戏进行中的总函数
void MainGameClass::GamePlaying()
{
	SetAIBlinky();
	SetAITinky();
	PlayerTrunAndMove();
	PlayerRaySimulation();
	ShowMap();
	PauseGame();
	DrawGamePlaying();
	PlayJumpSound();
	JudgeGameOver();
	CollectInfoFragment();
	ExitGame();
}

//游戏玩家视角转动和前后移动 
void MainGameClass::PlayerTrunAndMove()
{
	//向左旋转
	if (IS_KEY_DOWN(0x41))
	{
		player.angle -= (3.2f) * elapsed_timeS;
	}
	//向右旋转
	if (IS_KEY_DOWN(0x44))
	{
		player.angle += (3.2f) * elapsed_timeS;
	}
	//向前移动
	if (IS_KEY_DOWN(0x57))
	{
		player.x += sinf(player.angle) * 5.0f * elapsed_timeS;
		player.y += cosf(player.angle) * 5.0f * elapsed_timeS;
		//碰撞检测
		if (map[(int)player.y * map_width + (int)player.x] == '#')
		{
			player.x -= sinf(player.angle) * 5.0f * elapsed_timeS;
			player.y -= cosf(player.angle) * 5.0f * elapsed_timeS;
		}
	}
	//向后移动
	if (IS_KEY_DOWN(0x53))
	{
		player.x -= sinf(player.angle) * 5.0f * elapsed_timeS;
		player.y -= cosf(player.angle) * 5.0f * elapsed_timeS;
		//碰撞检测
		if (map[(int)player.y * map_width + (int)player.x] == '#')
		{
			player.x += sinf(player.angle) * 5.0f * elapsed_timeS;
			player.y += cosf(player.angle) * 5.0f * elapsed_timeS;
		}
	}
}

//模拟玩家视角中177条(由屏幕宽度决定)射线的射出以及碰撞 并对screen进行赋值     打穿墙壁
void MainGameClass::PlayerRaySimulation()
{
	for (int x = 0; x < screen_width; x++)
	{
		//每一条射线的角度，相对于初始角度的
		float ray_angle = (player.angle - player.field_of_view / 2.0f) + ((float)x / (float)screen_width * player.field_of_view);
		//这个变量记录射线移动的距离
		float distance_to_wall = 0;
		//记录射线到猎人的距离
		float distance_to_Blinky = 0;
		float distance_to_Tinky = 0;
		//射线到信息碎片的距离
		float distance_to_info = 0;
		//射线是否碰撞到墙壁
		bool is_hit_wall = false;
		//判断射线是否碰撞到边界
		bool is_hit_boundary = false;
		//判断射线是否碰到猎人
		bool is_hit_Blinky = false;
		bool is_hit_Tinky = false;
		//判断射线是否碰到信息碎片
		bool is_hit_info = false;

		//射线的单位向量  方便后面的计算
		float ray_unit_vectorX = sinf(ray_angle);
		float ray_unit_vectorY = cosf(ray_angle);

		//这个循环开始模拟射线的移动
		while (!is_hit_wall && distance_to_wall < player.depth)
		{
			distance_to_wall += 0.01f;
			//测试点  即射线目前移动到的位置 不用太准确，固使用int
			int testX = (int)(player.x + ray_unit_vectorX * distance_to_wall);
			int testY = (int)(player.y + ray_unit_vectorY * distance_to_wall);
			//如果射线到达边界
			if (testX < 0 || testX > map_width || testY < 0 || testY > map_height)
			{
				is_hit_wall = true;
				distance_to_wall = player.depth;
			}
			else
			{
				//射线碰到了墙壁  
				if (!CheckWall(testX, testY))
				{
					is_hit_wall = true;
					//接下要模拟每一块墙壁的边界   这样看起来效果更好
					std::vector<std::pair<float, float>>p;//储存四个角落到玩家的距离，以及与玩家发出射线的角度
					//有四个角
					for (int tx = 0; tx < 2; tx++)
					{
						for (int ty = 0; ty < 2; ty++)
						{
							//这里用float为了更加精确一点  墙的边界的向量
							float vx = (float)testX - player.x + tx;
							float vy = (float)testY - player.y + ty;
							//计算墙边界到玩家的距离
							float distance = sqrtf(vx * vx + vy * vy);
							//计算两个向量的角度(墙边界向量和玩家视角射线向量)
							float angle = (ray_unit_vectorX * vx / distance) + (ray_unit_vectorY * vy / distance);
							//存储起来
							p.push_back(std::make_pair(distance, angle));
						}
					}
					//用lambda函数进行排列
					sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

					//定义一个角度界限 小于这个界限的即为边界
					float bound = 0.005;
					//看到两个边界，这样不会出现角落边界重叠的bug
					if (acos(p.at(0).second) < bound) is_hit_boundary = true;
					if (acos(p.at(1).second) < bound) is_hit_boundary = true;

					//打穿墙壁
					if (IS_KEY_DOWN(VK_SPACE))
					{
						if (player.is_punch == 1)
						{
							if (x == screen_width / 2 && testX < map_width - 1 && testX>0 && testY > 0 && testY < map_width - 1)
							{
								map[testY * map_width + testX] = ' ';
								player.is_punch = 0;
								tp5 = std::chrono::system_clock::now();
								Sleep(200);
							}
						}
					}
				}
				//如果射线碰到了猎人
				if (map[testY * map_width + testX] == 'B')
				{
					is_hit_Blinky = true;
					distance_to_Blinky = distance_to_wall;
				}
				if (map[testY * map_width + testX] == 'T')
				{
					is_hit_Tinky = true;
					distance_to_Tinky = distance_to_wall;
				}
				//如果射线碰到了信息碎片
				if (map[testY * map_width + testX] == '.')
				{
					is_hit_info = true;
					distance_to_info = distance_to_wall;
				}
			}
		}

		//计算天花板和地板的距离   当你距离墙壁越进，天花板就越高，地板就越来越低，所以天花板和地板的高度与玩家距离墙壁的距离有关
		//玩家距离墙壁越近  ceiling的值就越小，所以ceiling与distance_to_wall成正比
		//这里的1.5系数可以控制一个格子的宽度，这个系数越大，格子越宽
		int  ceiling = (float)(screen_height / 2.0f) - (screen_height / 1.5f) / ((float)distance_to_wall);
		int floor = screen_height - ceiling;
		//计算猎人的高度
		int Blinky_ceiling = (float)(screen_height / 2.0f) - (screen_height) / ((float)distance_to_Blinky);
		int Blinky_floor = (float)(screen_height * 5.0f / 6.0f) - ((float)distance_to_Blinky);
		int Tinky_ceiling = (float)(screen_height / 2.0f) - (screen_height) / ((float)distance_to_Tinky);
		int Tinky_floor = (float)(screen_height * 5.0f / 6.0f) - ((float)distance_to_Tinky);
		//计算信息碎片的高度
		int info_ceiling = (float)(screen_height * 3.0f / 4.0f);
		int info_floor = (float)(screen_height * 6.0f / 7.0f) - (float)distance_to_info;


		//现在开始y轴方向的遍历，绘制
		for (int y = 0; y < screen_height; y++)
		{
			if (y <= ceiling)
				screen[y * screen_width + x] = ' ';
			else if (y > ceiling && y < floor)
			{
				short shade;
				if (distance_to_wall <= player.depth / 4.0f) shade = 0x2588;//近
				else if (distance_to_wall <= player.depth / 3.0f) shade = 0x2593;
				else if (distance_to_wall <= player.depth / 2.0f) shade = 0x2592;
				else if (distance_to_wall <= player.depth / 1.0f) shade = 0x2591; //远
				else shade = ' ';
				if (is_hit_boundary) shade = ' ';
				screen[y * screen_width + x] = shade;
			}
			else
			{
				short shade = ' ';
				float b = 1.0f - (((float)y - screen_height / 2.0f) / ((float)screen_height / 2.0f));
				if (b < 0.25) shade = '#';
				else if (b < 0.5) shade = 'x';
				else if (b < 0.75) shade = '.';
				else if (b < 0.9) shade = '-';
				else shade = ' ';
				screen[y * screen_width + x] = shade;
			}
			if (is_hit_Blinky && y >= Blinky_ceiling && y <= Blinky_floor)
				screen[y * screen_width + x] = 'B';
			if (is_hit_Tinky && y >= Tinky_ceiling && y <= Tinky_floor)
				screen[y * screen_width + x] = 'T';
			if (is_hit_info && y >= info_ceiling && y <= info_floor && x > 60 && x < 100)
				screen[y * screen_width + x] = '|';
		}
	}
	screen[screen_width * screen_height - 1] = '\0';
}

//绘制主屏幕
void MainGameClass::DrawGamePlaying()
{
	//显示存活时间
	swprintf_s(screen, 177, L"X=%3.2f  Y=%3.2f  FPS=%3.2f  InfoFragment=%d  Punch=%d  Map=%d  LiveTime=%ds", player.x, player.y, 1.0f / elapsed_timeS, num_info_fragment, player.is_punch, can_open_map, live_time.count());

	//没啥用，但是必须要有
	DWORD dw_byte_wirtten = 0;

	//如果地图没有打开  检查并清除字符颜色
	if (!map_is_open)
	{
		WriteConsoleOutputAttribute(*renderer.h_p_output, attribute_array, screen_width * screen_height, { 0,0 }, &dw_byte_wirtten);
	}

	WriteConsoleOutputCharacter(*renderer.h_p_output, screen, screen_width * screen_height, { 0,0 }, &dw_byte_wirtten);
}

//按下M键显示地图
void MainGameClass::ShowMap()
{
	//设置玩家朝向
	COORD player_direction_pos;
	float caculate_angle = player.angle;
	while (caculate_angle < 0)
		caculate_angle += 6.28;
	while (caculate_angle >= 6.28)
		caculate_angle -= 6.28;
	float direction = fabs(caculate_angle) / 1.570796;
	if (direction < 0.5)
	{
		player_direction_pos.X = (int)player.x, player_direction_pos.Y = (int)player.y + 2;
		player_direction = 1;
	}
	else if (fabs(direction - 1) < 0.5)

	{
		player_direction_pos.X = (int)player.x + 1, player_direction_pos.Y = (int)player.y + 1;
		player_direction = 3;
	}
	else if (fabs(direction - 2) < 0.5)
	{
		player_direction_pos.X = (int)player.x, player_direction_pos.Y = (int)player.y;
		player_direction = 0;
	}
	else if (fabs(direction - 3) < 0.5)
	{
		player_direction_pos.X = (int)player.x - 1, player_direction_pos.Y = (int)player.y + 1;
		player_direction = 2;
	}
	else
	{
		player_direction_pos.X = 0, player_direction_pos.Y = 0;
	}
	if (IS_KEY_DOWN(0x4D) && can_open_map)
	{
		map_is_open = true;
		for (int x = 0; x < map_width; x++)
		{
			for (int y = 0; y < map_height; y++)
			{
				screen[(y + 1) * screen_width + x] = map[y * map_width + x];
			}
		}

		//设置玩家为绿色
		CHAR_INFO char_info;
		char_info.Attributes = FOREGROUND_GREEN;
		COORD player_char_pos = { (int)player.x ,(int)player.y + 1 };
		COORD char_size = { 1,1 };
		//没啥用，但是必须要有
		DWORD dw_byte_wirtten = 0;
		WriteConsoleOutputAttribute(*renderer.h_p_output, &char_info.Attributes, 1, player_char_pos, &dw_byte_wirtten);//设置字体颜色
		screen[((int)player.y + 1) * screen_width + (int)player.x] = L'P';
		//设置方向
		if (player_direction_pos.X < map_width && player_direction_pos.Y < map_height && player_direction_pos.X >= 0 && player_direction_pos.Y >= 0)
			screen[player_direction_pos.Y * screen_width + player_direction_pos.X] = 0x2588;
		//设置Blinky的颜色为红色
		if (!Blinky.is_death)
		{
			char_info.Attributes = FOREGROUND_RED;
			COORD Blinky_char_pos = { (int)Blinky.x ,(int)Blinky.y + 1 };
			WriteConsoleOutputAttribute(*renderer.h_p_output, &char_info.Attributes, 1, Blinky_char_pos, &dw_byte_wirtten);//设置字体颜色
		}
		//设置Tinky的颜色为粉色
		if (!Tinky.is_death)
		{
			char_info.Attributes = FOREGROUND_RED | FOREGROUND_BLUE | COMMON_LVB_LEADING_BYTE;
			COORD Tinky_char_pos = { (int)Tinky.x ,(int)Tinky.y + 1 };
			WriteConsoleOutputAttribute(*renderer.h_p_output, &char_info.Attributes, 1, Tinky_char_pos, &dw_byte_wirtten);//设置字体颜色
		}
	}
	if (IS_KEY_UP(0x4D))
	{
		map_is_open = false;
	}
}

//按下p键暂停
void MainGameClass::PauseGame()
{
	if (IS_KEY_DOWN(0x50))
	{
		main_game_state = GameState::MainGame::GS_GamePuase;
		Sleep(200);
	}
}

//设置猎人Blinky的AI
void MainGameClass::SetAIBlinky()
{
	if (!Blinky.is_death)
	{
		if (num_info_fragment > 0)
		{
			Blinky.thinking++;
			if (Blinky.thinking > 500)
			{
				Blinky.thinking = 0;
				Blinky_path.clear();
				std::vector<std::vector<int>>grid(map_height, std::vector<int>(map_width));
				//将地图转为能够显示状态（即是否有墙）的数组
				for (int i = 0; i < map_height; i++)
				{
					for (int j = 0; j < map_width; j++)
					{
						if (map[i * map_width + j] == '#')
							grid[i][j] = 1;
						else
							grid[i][j] = 0;
					}
				}

				Node* start = new Node((int)Blinky.x, (int)Blinky.y);
				Node* end = new Node((int)player.x, (int)player.y);
				AStar a_star(grid);
				//接收路径数组
				Blinky_path = a_star.FindPath(start, end);
			}

			Blinky.move_speed++;
			if (!Blinky_path.empty() && Blinky.move_speed > 200)
			{
				Blinky.move_speed = 0;
				Blinky.BMoveToNextPoint(Blinky_path.front()->x, Blinky_path.front()->y, map, map_width, map_height, is_info_fragment);
				Blinky_path.erase(Blinky_path.begin());
			}
		}
		if (num_info_fragment == 0)
		{
			Blinky.run_thinking++;
			if (Blinky.run_thinking > 180)
			{
				Blinky.run_thinking = 0;
				Blinky_path.clear();
				std::vector<std::vector<int>>grid(map_height, std::vector<int>(map_width));
				//将地图转为能够显示状态（即是否有墙）的数组
				for (int i = 0; i < map_height; i++)
				{
					for (int j = 0; j < map_width; j++)
					{
						if (map[i * map_width + j] == '#' || (i == (int)player.y && j == (int)player.x))
							grid[i][j] = 1;
						else
							grid[i][j] = 0;
					}
				}

				Node* start = new Node((int)Blinky.x, (int)Blinky.y);
				int new_x = rand() % map_width;
				int new_y = rand() % map_height;
				while (map[new_y * map_width + new_x] == '#' && sqrtf(pow(player.x - new_x, 2) + pow(player.y - new_y, 2)) < 8)
				{
					new_x = rand() % map_width;
					new_y = rand() % map_height;
				}
				Node* end = new Node(new_x, new_y);
				AStar a_star(grid);
				//接收路径数组
				Blinky_path = a_star.FindPath(start, end);
			}

			Blinky.run_speed++;
			if (!Blinky_path.empty() && Blinky.run_speed > 40)
			{
				Blinky.run_speed = 0;
				Blinky.BMoveToNextPoint(Blinky_path.front()->x, Blinky_path.front()->y, map, map_width, map_height, is_info_fragment);
				Blinky_path.erase(Blinky_path.begin());
			}
		}
	}
}

//设置猎人Tinky的AI
void MainGameClass::SetAITinky()
{
	if (!Tinky.is_death)
	{
		if (num_info_fragment != 0)
		{
			Tinky.thinking++;
			if (Tinky.thinking > 300)
			{
				Tinky.thinking = 0;
				if (!Tinky_path.empty())
					Tinky_path.clear();
				std::vector<std::vector<int>>grid(map_height, std::vector<int>(map_width));
				//将地图转为能够显示状态（即是否有墙）的数组
				for (int i = 0; i < map_height; i++)
				{
					for (int j = 0; j < map_width; j++)
					{
						if (map[i * map_width + j] == '#')
							grid[i][j] = 1;
						else
							grid[i][j] = 0;
					}
				}

				Node* start = new Node((int)Tinky.x, (int)Tinky.y);
				POINT target;

				target.x = player.x, target.y = player.y
					;
				switch (player_direction)
				{
				case 0:
				{

					if (((int)player.y - 2) < map_height && (int)player.y - 2 >= 0 && (int)player.x < map_width && (int)player.x >= 0)
					{
						if (map[((int)player.y - 1) * map_width + (int)player.x] != '#')
						{
							if (map[((int)player.y - 2) * map_width + (int)player.x] != '#')
							{
								target.x = (int)player.x, target.y = (int)player.y - 2;
							}
						}
					}
				}break;
				case 1:
				{
					if (((int)player.y + 2) < map_height && ((int)player.y + 2) >= 0 && (int)player.x < map_width && (int)player.x >= 0)
					{
						if (map[((int)player.y + 1) * map_width + (int)player.x] != '#')
						{
							if (map[((int)player.y + 2) * map_width + (int)player.x] != '#')
							{
								target.x = (int)player.x, target.y = (int)player.y + 2;
							}
						}
					}
				}break;
				case 2:
				{
					if ((int)player.y < map_height && (int)player.y >= 0 && (int)player.x - 2 < map_width && (int)player.x - 2 >= 0)
					{
						if (map[(int)player.y * map_width + (int)player.x - 1] != '#')
						{
							if (map[(int)player.y * map_width + (int)player.x - 2] != '#')
							{
								target.x = (int)player.x - 2, target.y = (int)player.y;
							}
						}
					}
				}break;
				case 3:
				{
					if ((int)player.y < map_height && (int)player.y >= 0 && (int)player.x + 2 < map_width && (int)player.x + 2 >= 0)
					{
						if (map[(int)player.y * map_width + (int)player.x + 1] != '#')
						{
							if (map[(int)player.y * map_width + (int)player.x + 2] != '#')
							{
								target.x = (int)player.x + 2, target.y = (int)player.y;
							}
						}
					}
				}break;
				}
				Node* end = new Node(target.x, target.y);
				AStar a_star(grid);
				//接收路径数组
				Tinky_path = a_star.FindPath(start, end);
			}

			Tinky.move_speed++;
			if (!Tinky_path.empty() && Tinky.move_speed > 150)
			{
				Tinky.move_speed = 0;
				Tinky.TMoveToNextPoint(Tinky_path.front()->x, Tinky_path.front()->y, map, map_width, map_height, is_info_fragment);
				Tinky_path.erase(Tinky_path.begin());
			}
		}
		if (num_info_fragment == 0)
		{
			Tinky.run_thinking++;
			if (Tinky.run_thinking > 180)
			{
				Tinky.run_thinking = 0;
				Tinky_path.clear();
				std::vector<std::vector<int>>grid(map_height, std::vector<int>(map_width));
				//将地图转为能够显示状态（即是否有墙）的数组
				for (int i = 0; i < map_height; i++)
				{
					for (int j = 0; j < map_width; j++)
					{
						if (map[i * map_height + j] == '#' || (i == (int)player.y && j == (int)player.x))
							grid[i][j] = 1;
						else
							grid[i][j] = 0;
					}
				}

				Node* start = new Node((int)Tinky.x, (int)Tinky.y);
				int new_x = rand() % map_width;
				int new_y = rand() % map_height;
				while (map[new_y * map_width + new_x] == '#' && sqrtf(pow(player.x - new_x, 2) + pow(player.y - new_y, 2)) < 10)
				{
					new_x = rand() % map_width;
					new_y = rand() % map_height;
				}
				Node* end = new Node(new_x, new_y);
				AStar a_star(grid);
				//接收路径数组
				Tinky_path = a_star.FindPath(start, end);
			}

			Tinky.run_speed++;
			if (!Tinky_path.empty() && Tinky.run_speed > 40)
			{
				Tinky.run_speed = 0;
				Tinky.TMoveToNextPoint(Tinky_path.front()->x, Tinky_path.front()->y, map, map_width, map_height, is_info_fragment);
				Tinky_path.erase(Tinky_path.begin());
			}
		}
	}
}

//猎人如果在玩家附近，播放心跳声
void MainGameClass::PlayJumpSound()
{
	if ((Blinky.CalculateDistance(player.x, player.y) >= 5 && Blinky.CalculateDistance(player.x, player.y) <= 10) || (Tinky.CalculateDistance(player.x, player.y) >= 5 && Tinky.CalculateDistance(player.x, player.y) <= 10))
	{
		is_less_10 = true;
		is_less_5 = false;
	}
	else if (Blinky.CalculateDistance(player.x, player.y) < 5 || Tinky.CalculateDistance(player.x, player.y) < 5)
	{
		is_less_5 = true;
		is_less_10 = false;
	}
	else
	{
		is_less_10 = false;
		is_less_5 = false;
	}
}

//如果猎人与玩家碰撞，游戏结束
void MainGameClass::JudgeGameOver()
{
	if (((int)Blinky.x == (int)player.x && (int)Blinky.y == (int)player.y) || ((int)Tinky.x == (int)player.x && (int)Tinky.y == (int)player.y))
	{
		if (num_info_fragment != 0)
		{
			is_player_die = true;
			Clear();
			MessageBox(0, L"逃跑失败！", L"糟糕", 0);
			Sleep(200);
			game_state = GameState::GS_GameOver;
		}
		else if (num_info_fragment == 0)
		{
			if (((int)Blinky.x == (int)player.x && (int)Blinky.y == (int)player.y))
			{
				if (num_hunter == 2)
					num_hunter = 1;
				else if (num_hunter == 1)
					num_hunter = 0;
				map[Blinky.y * map_width + Blinky.x] = ' ';
				Sleep(200);
				Blinky.is_death = true;
				Blinky.x = 0;
				Blinky.y = 0;
			}
			if (((int)Tinky.x == (int)player.x && (int)Tinky.y == (int)player.y))
			{
				if (num_hunter == 2)
					num_hunter = 1;
				else if (num_hunter == 1)
					num_hunter = 0;
				map[Tinky.y * map_width + Tinky.x] = ' ';
				Sleep(200);
				Tinky.is_death = true;
				Tinky.x = 0;
				Tinky.y = 0;
			}
		}
	}
	if (num_hunter == 0)
	{
		Clear();
		MessageBox(0, L"成功击杀猎人！", L"恭喜", 0);
		Sleep(200);
		game_state = GameState::GS_GameOver;
	}
}

//模拟玩家收集信息碎片
void MainGameClass::CollectInfoFragment()
{
	if (is_info_fragment[(int)player.y * map_height + (int)player.x] && num_info_fragment > 0)
	{
		map[(int)player.y * map_height + (int)player.x] = ' ';
		is_info_fragment[(int)player.y * map_height + (int)player.x] = false;
		num_info_fragment--;
		if (collect_info != 100)
			collect_info++;
		Game::is_play_get_info = true;
	}
	if (collect_info == 100)
		can_open_map = 1;
}

//游戏结束要做的一些清理工作
void MainGameClass::Clear()
{
	map_is_open = false;
	is_less_10 = false;
	is_less_5 = false;
	is_play_get_info = false;
	maze.clear();
	map.clear();
}

//游戏暂停
void MainGameClass::GamePause()
{
	while (1)
	{
		if (IS_KEY_DOWN(0x50))
		{
			main_game_state = GameState::MainGame::GS_GamePlaying;
			Sleep(200);
			break;
		}

		if (IS_KEY_DOWN(0x1B))
		{
			Clear();
			game_state = GameState::GS_GameOver;
			break;
		}
	}
}
//退出游戏
void MainGameClass::ExitGame()
{
	if (IS_KEY_DOWN(0x1B))
	{
		Clear();
		game_state = GameState::GS_GameOver;
	}
}

