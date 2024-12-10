#pragma once 

#include"MyConsoleEnegine.h"
#include"Menu.h"
#include"MainGame.h"

namespace Game
{
	namespace GameState
	{
		const int GS_Menu = 1;
		const int GS_MainGame = 2;
		const int GS_GameOver = 3;

		namespace StartMenu
		{
			const int GS_StartSelect = 10;
			const int GS_HelpSelect = 11;
			const int GS_ExitSelect = 12;
			const int GS_HelpMenu = 13;
		}

		namespace MainGame
		{
			const int GS_GameInit = 20;
			const int GS_GamePlaying = 21;
			const int GS_GamePuase = 22;
		}

		namespace GameOverMenu
		{
			const int GS_GameOverMenu = 30;
			const int GS_GameOverExit = 31;
		}
	}

	//屏幕的宽度和高度
	extern int screen_width;
	extern int screen_height;
	//游戏状态跟进
	extern int game_state;
	//游戏渲染
	extern Renderer renderer;
	//游戏进程
	extern MainGameClass main_game;
	//更进主要游戏的游戏状态
	extern int main_game_state;
	//距离标志
	extern bool is_less_10;
	extern bool is_less_5;
	//是否播放收集信息碎片的提示音
	extern bool is_play_get_info;
	//是否播放玩家死亡音效
	extern bool is_player_die;
	//判断菜单音乐是否播放
	extern bool is_play_menu_music;

	//初始化控制台属性
	void InitGlobal();
	//设置窗口大小
	void SetConsoleWindowSize(SHORT width, SHORT height);
	//更新窗口标题
	void UpdateWindowTitle();
	//使鼠标锁定在窗口
	void LockMouse();
	//游戏状态机驱动程序
	void GameStateSelect();


	//游戏主菜单
	void GameMenu();
	//游戏进行中
	void MainGame();
	//游戏结束菜单
	void GameOver();
}