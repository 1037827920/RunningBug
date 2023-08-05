#include"Game.h"

using namespace Game;

namespace Game
{
	//屏幕的宽度和高度
	int screen_width = 150;
	int screen_height = 45;
	//游戏状态跟进
	int game_state;
	//游戏渲染
	Renderer renderer;
	//游戏菜单
	Menu menu;
	//游戏进程
	MainGameClass main_game;
	//更进主要游戏的游戏状态
	int main_game_state;
	//距离标志
	bool is_less_10 = false;
	bool is_less_5 = false;
	//是否播放收集信息碎片的提示音
	bool is_play_get_info = false;
	//是否播放玩家死亡音效
	bool is_player_die = false;
	//判断菜单音乐是否播放
	bool is_play_menu_music = false;
	//获取控制台窗口大小的矩形
	RECT console_rect;
}

//初始化控制台属性
void Game::InitGlobal()
{
	game_state = GameState::GS_Menu;
	//设置窗口大小
	Game::SetConsoleWindowSize(screen_width, screen_height);
	//传入参数为双缓冲区的宽度和高度
	renderer.Init(screen_width, screen_height);
	mciSendString(L"open Media/music/menu_bgm.mp3", 0, 0, 0);
	mciSendString(L"play Media/music/menu_bgm.mp3 repeat", 0, 0, 0);
	is_play_menu_music = true;
}

//设置窗口大小
void Game::SetConsoleWindowSize(SHORT width, SHORT height)
{
	//调整窗口大小为150 45
	system("mode con cols=150 lines=45");
	//去除最大，最小化按钮
	HWND hwnd = GetConsoleWindow();
	LONG_PTR console_style = GetWindowLongPtrA(hwnd, GWL_STYLE);
	console_style = console_style & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX;
	SetWindowLongPtrA(hwnd, GWL_STYLE, console_style);
}

//更新窗口标题
void Game::UpdateWindowTitle()
{
	std::string title;
	title = "Running!BUG!";
	renderer.SetWindowTitle(title.c_str());
}

//游戏状态机驱动程序
void Game::GameStateSelect()
{
	//双缓冲实现
	renderer.SwitchBuffer();
	switch (game_state)
	{
	case GameState::GS_Menu:
	{
		GameMenu();
	}break;
	case GameState::GS_MainGame:
	{
		MainGame();
	}
	break;
	case GameState::GS_GameOver:
	{
		GameOver();
	}break;
	}
}

//游戏主菜单
void Game::GameMenu()
{
	menu.UpdateAndRenderMenu();
}

//游戏进行中
void Game::MainGame()
{
	main_game.UpdateAndRenderMainGame();
}

//游戏结束菜单
void Game::GameOver()
{
	mciSendString(L"close Media/music/game_bgm.mp3", 0, 0, 0);
	//播放音乐
	if (is_play_menu_music == false && is_player_die == false)
	{
		mciSendString(L"play Media/music/menu_bgm.mp3 repeat", 0, 0, 0);
		is_play_menu_music = true;
	}
	menu.UpdateAndRenderGameOverMenu();
}